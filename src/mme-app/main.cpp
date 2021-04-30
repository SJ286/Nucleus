/*
 * Copyright (c) 2020  Great Software Laboratory Pvt. Ltd.
 * Copyright 2020-present Open Networking Foundation
 * Copyright (c) 2019, Infosys Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <assert.h>
#include <iostream>
#include <pthread.h>
#include <thread>
#include <string.h>
#include <sys/stat.h>
#include "epc/epctools.h"
#include "epc/elogger.h"
#include "epc/etypes.h"
#include <blockingCircularFifo.h>
#include <msgBuffer.h>
#include "err_codes.h"
#include <interfaces/mmeIpcInterface.h>
#include <mmeStates/stateFactory.h>
#include "mme_app.h"
#include "msgType.h"
#include "stateMachineEngine.h"
#include <sys/types.h>
#include "mmeThreads.h"
#include "log.h"
#include "json_data.h"
#include "monitorSubscriber.h"
#include "timeoutManager.h"
#include <utils/mmeTimerUtils.h>
#include "mmeStatsPromClient.h"
#include <csignal>

using namespace std;
using namespace mme;

mmeConfig *mme_tables = nullptr;
/*********************************************************
 *
 * Circular FIFOs for sender IPC and Reader IPC threads
 *
 **********************************************************/
cmn::utils::BlockingCircularFifo<cmn::IpcEventMessage, fifoQSize_c> mmeIpcIngressFifo_g;
cmn::utils::BlockingCircularFifo<cmn::IpcEventMessage, fifoQSize_c> mmeIpcEgressFifo_g;

/*********************************************************
 *
 * Externs
 *
 **********************************************************/
extern "C"
{
#include "thread_pool.h"
int init_sock();
}

extern JobFunction monitorConfigFunc_fpg;
extern void init_parser(char *path);
extern void parse_done();
extern int parse_mme_conf(mme_config *config);
extern void* RunServer(void * data);

extern char processName[255];
int g_unix_fd = 0;
struct thread_pool *g_tpool;
pthread_t acceptUnix_t;

mme_config_t *mme_cfg = NULL;
pthread_t stage_tid[5];

MmeIpcInterface* mmeIpcIf_g = NULL;
TimeoutManager* timeoutMgr_g = NULL;

using namespace std::placeholders;

void signalHandler( int signum ) {
   std::cout << "Interrupt signal (" << signum << ") received.\n";

}

static void
init_signal_handler(void)
{
	{
		sigset_t sigset;
		/* mask SIGALRM in all threads by default */
		sigemptyset(&sigset);
		sigaddset(&sigset, SIGRTMIN);
		sigaddset(&sigset, SIGUSR1);
		sigprocmask(SIG_BLOCK, &sigset, NULL);
	}

	struct sigaction sa;

	/* Setup the signal handler */
	sa.sa_handler = signalHandler;
	sa.sa_flags = SA_RESTART;
	sigfillset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1) {}
	if (sigaction(SIGTERM, &sa, NULL) == -1) {}
}

void setThreadName(std::thread* thread, const char* threadName)
{
   	auto handle = thread->native_handle();
	pthread_setname_np(handle,threadName);
}

int load_file()
{
	EGetOpt opt;
	EString optfile;
	try {
		opt.loadFile("conf/log.json");
	} catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
		return -1;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	load_file();
	init_signal_handler();

	memcpy (processName, argv[0], strlen(argv[0]));

	char *hp = getenv("MMERUNENV");
	if (hp && (strcmp(hp, "container") == 0)) {
		init_logging((char*)("container"), NULL);
	}
	else { 
		init_logging((char*)("hostbased"), (char*)("/tmp/mmelogs.txt"));
	}
	
	init_backtrace(argv[0]);

	srand(time(0));

    auto cb = std::bind(&MmeTimerUtils::onTimeout, _1);
    timeoutMgr_g = new TimeoutManager(cb);

	StateFactory::Instance()->initialize();

	mmeIpcIf_g = new MmeIpcInterface();
	mmeIpcIf_g->setup();

    mme_cfg = new (mme_config_t);
    assert(mme_cfg != NULL);

    mme_tables = new mmeConfig();
    mmeConfig::mme_parse_config_new(mme_cfg);

    std::thread prom(mmeStatsSetupPrometheusThread, mme_cfg->prom_port);
    prom.detach();

	/* Lets apply logging setting */
	set_logging_level(mme_cfg->logging);

	register_config_updates();

	MmeIngressIpcProducerThread ipcReader;
	std::thread t1(ipcReader);
	setThreadName(&t1, "IpcReader");
	t1.detach();

	MmeIngressIpcConsumerThread msgHandlerThread;
	std::thread t2(msgHandlerThread);
	setThreadName(&t2, "MMEMsgHandlerThread");
	t2.detach();

	MmeEgressIpcConsumerThread ipcWriter;
	std::thread t3(ipcWriter);
	setThreadName(&t3, "IpcWriter");
	t3.detach();

	// start gRPC server
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&stage_tid[0], &attr, &RunServer, NULL);
	pthread_attr_destroy(&attr);

	monitorConfigFunc_fpg = &(MonitorSubscriber::handle_monitor_processing);

    if (init_sock() != SUCCESS)
    {
        log_msg(LOG_ERROR, "Error in initializing unix domain socket server.");
        return -E_FAIL_INIT;
    }

	while(1)
	{
		SM::StateMachineEngine::Instance()->run();
	}

	return 0;
}
