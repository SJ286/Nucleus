  
/*
 * Copyright 2019-present Infosys Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */
 
/**************************************
 * intraS1HoStart.cpp
 * This is an auto generated file.
 * Please do not edit this file.
 * All edits to be made through template source file
 * <TOP-DIR/scripts/SMCodeGen/templates/stateMachineTmpls/state.cpp.tt>
 **************************************/

#include "mmeSmDefs.h"
#include "actionTable.h"
#include "actionHandlers/actionHandlers.h"

#include "mmeStates/intraS1HoStart.h"	
#include "mmeStates/s1HoWfHoRequestAck.h"

using namespace mme;
using namespace SM;

/******************************************************************************
* Constructor
******************************************************************************/
IntraS1HoStart::IntraS1HoStart():State(intra_s1_ho_start)
{
}

/******************************************************************************
* Destructor
******************************************************************************/
IntraS1HoStart::~IntraS1HoStart()
{
}

/******************************************************************************
* creates and returns static instance
******************************************************************************/
IntraS1HoStart* IntraS1HoStart::Instance()
{
        static IntraS1HoStart state;
        return &state;
}

/******************************************************************************
* initializes eventToActionsMap
******************************************************************************/
void IntraS1HoStart::initialize()
{
        {
                ActionTable actionTable;
                actionTable.addAction(&ActionHandlers::send_ho_request_to_target_enb);
                actionTable.setNextState(S1HoWfHoRequestAck::Instance());
                eventToActionsMap.insert(pair<uint16_t, ActionTable>(INTRA_S1HO_START, actionTable));
        }
}
