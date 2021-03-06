 /*
 * Copyright 2021-present, Infosys Limited.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __MmeSmDeleteBearerProcCtxtManager__
#define __MmeSmDeleteBearerProcCtxtManager__
/******************************************************
* mmeSmDeleteBearerProcCtxtManager.h
 * This is an auto generated file.
 * Please do not edit this file.
 * All edits to be made through template source file
 * <TOP-DIR/scripts/SMCodeGen/templates/ctxtManagerTmpls/blockPoolManager.h.tt>
 ***************************************/
#include "memPoolManager.h"

namespace mme
{
	class MmeSmDeleteBearerProcCtxt;
	class MmeSmDeleteBearerProcCtxtManager
	{
		public:
			/****************************************
			* MmeSmDeleteBearerProcCtxtManager
			*  constructor
			****************************************/
			MmeSmDeleteBearerProcCtxtManager(int numOfBlocks);
			
			/****************************************
			* MmeSmDeleteBearerProcCtxtManager
			*    Destructor
			****************************************/
			~MmeSmDeleteBearerProcCtxtManager();
			
			/******************************************
			 * allocateMmeSmDeleteBearerProcCtxt
			 * allocate MmeSmDeleteBearerProcCtxt data block
			 ******************************************/
			MmeSmDeleteBearerProcCtxt* allocateMmeSmDeleteBearerProcCtxt();
			
			/******************************************
			 * deallocateMmeSmDeleteBearerProcCtxt
			 * deallocate a MmeSmDeleteBearerProcCtxt data block
			 ******************************************/
			void deallocateMmeSmDeleteBearerProcCtxt(MmeSmDeleteBearerProcCtxt* MmeSmDeleteBearerProcCtxtp );
	
		private:
			cmn::memPool::MemPoolManager<MmeSmDeleteBearerProcCtxt> poolManager_m;
	};
};

#endif
		
		
