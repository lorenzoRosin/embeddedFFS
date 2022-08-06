/**
 * @file ParamOrganizer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ParamOrganizer.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      PRIVATE TYPEDEFS
 **********************/
typedef struct t_paramOrgPageParam
{
    uint32_t    pageTimeSetted;
    uint32_t    pageType;
    uint32_t    pageMagicNumber;
    uint32_t    pageCrc;
}t_paramOrgPageParam;

/**********************
 *   PRIVATE STATIC FUNCTIONS PROTOTYPES
 **********************/
e_paramOrgResult getPrgPageParam(s_paramOrgContext* prmCntx, uint32_t pageOffset, t_paramOrgPageParam* prmPage);
e_paramOrgResult isValidPageData(s_paramOrgContext* prmCntx, uint32_t pageOffset);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

e_paramOrgResult initParamSettings(s_paramOrgContext* prmCntx, const uint32_t pageSize, const uint32_t nOfPages,
                                   const uint32_t pageId, const uint32_t paramHandlerFlag)
{
    e_paramOrgResult returnVal;

    if( NULL == prmCntx )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        if( true == prmCntx->isPageInfoInitialized )
        {
            returnVal = PARAMRES_BADPARAM;
        }
        else
        {
            if( ( pageSize < PARAMORGAN_MIN_PAGE_SIZE ) || ( pageSize > PARAMORGAN_MAX_PAGE_SIZE ) || ( 0u == nOfPages ) )
            {
                returnVal = PARAMRES_BADPARAM;
                prmCntx->isPageInfoInitialized = false;
            }
            else
            {
                prmCntx->pageSize = pageSize;
                prmCntx->nOfPages = nOfPages;
                prmCntx->pageId = pageId;
                prmCntx->paramHandlerFlag = paramHandlerFlag;
                prmCntx->isPageInfoInitialized = true;

                returnVal = PARAMRES_ALLOK;
            }
        }
    }

    return returnVal;
}

e_paramOrgResult initParamCallBack(s_paramOrgContext* prmCntx, cb_readPage rpClb, cb_writePage wpClb,
                                         cb_erasePage epClb, cb_calculateCrc32 cc32Clb)
{
    e_paramOrgResult returnVal;

    if( ( NULL == prmCntx ) || ( NULL == rpClb ) || ( NULL == wpClb ) || ( NULL == epClb ) || ( NULL == cc32Clb ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        if( true == prmCntx->isCallBackInitialized )
        {
            returnVal = PARAMRES_BADPARAM;
        }
        else
        {
            prmCntx->pToReadFunc = rpClb;
            prmCntx->pToWriteFunc = wpClb;
            prmCntx->pToEraseFunc = epClb;
            prmCntx->pToCrcFunc = cc32Clb;
            prmCntx->isCallBackInitialized = true;

            returnVal = PARAMRES_ALLOK;
        }
    }

    return returnVal;
}

e_paramOrgResult initLibMemory(s_paramOrgContext* prmCntx, uint8_t* memPool, uint32_t memPoolSize)
{
    e_paramOrgResult returnVal;

    if( ( NULL == prmCntx ) || ( NULL == memPool ) || ( NULL == wpClb ) || ( NULL == epClb ) || ( NULL == cc32Clb ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        if( true == prmCntx->isMemPoolInitialized )
        {
            returnVal = PARAMRES_BADPARAM;
        }
        else
        {
            prmCntx->pToReadFunc = rpClb;
            prmCntx->pToWriteFunc = wpClb;
            prmCntx->pToEraseFunc = epClb;
            prmCntx->pToCrcFunc = cc32Clb;
            prmCntx->isCallBackInitialized = true;

            returnVal = PARAMRES_ALLOK;
        }
    }

    return returnVal;
}



/**********************
 *   PRIVATE STATIC FUNCTIONS
 **********************/
e_paramOrgResult getPrgPageParam(uint8_t* page, t_paramOrgPageParam* prmPage)
{
    e_paramOrgResult returnVal;

    if( ( NULL == prmCntx ) || ( NULL == prmPage) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        if( (false == prmCntx->isCallBackInitialized) || (false == prmCntx->isPageInfoInitialized) )
        {
            returnVal = PARAMRES_BADPARAM;
        }
        else
        {
            if( pageOffset >= prmCntx->nOfPages )
            {
                returnVal = PARAMRES_BADPARAM;
            }
            else
            {
                prmCntx->pToReadFunc = rpClb;
                prmCntx->pToWriteFunc = wpClb;
                prmCntx->pToEraseFunc = epClb;
                prmCntx->pToCrcFunc = cc32Clb;
                prmCntx->isCallBackInitialized = true;

                returnVal = PARAMRES_ALLOK;
            }
        }
    }

    return returnVal;
}











e_paramOrgResult isValidPageData(s_paramOrgContext* prmCntx, uint32_t pageOffset)
{
    e_paramOrgResult returnVal;

    if( NULL == prmCntx )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        if( (false == prmCntx->isCallBackInitialized) || (false == prmCntx->isPageInfoInitialized) )
        {
            returnVal = PARAMRES_BADPARAM;
        }
        else
        {
            if( pageOffset >= nOfPages )
            {
                returnVal = PARAMRES_BADPARAM;
            }
            else
            {
                prmCntx->pToReadFunc = rpClb;
                prmCntx->pToWriteFunc = wpClb;
                prmCntx->pToEraseFunc = epClb;
                prmCntx->pToCrcFunc = cc32Clb;
                prmCntx->isCallBackInitialized = true;

                returnVal = PARAMRES_ALLOK;
            }
        }
    }

    return returnVal;
}




{
   bool isPageValid = false;

   DEVICE_SCHEDJOB_FLASH* presentJob = (DEVICE_SCHEDJOB_FLASH*) m_SupportPage;
   memset(presentJob, 0, sizeof(DEVICE_SCHEDJOB_FLASH));

   uint64_t calculatedCrc = 0;

   /* Calculate CRC */
   if(isMainPage)
   {
	   calculatedCrc = UtilsFlashManager::getFlashParameterAreaCrc(PARTITION_TYPE_DEVICE_JOB, 0, sizeof(DEVICE_SCHEDJOB_FLASH));
   }
   else
   {
	   calculatedCrc = UtilsFlashManager::getFlashParameterAreaCrc(PARTITION_TYPE_DEVICE_JOB_BKUP, 0, sizeof(DEVICE_SCHEDJOB_FLASH));
   }

   /* Get stored CRC */
   bool readed = false;

   if(isMainPage)
   {
	   readed = UtilsFlashManager::getFlashArea(PARTITION_TYPE_DEVICE_JOB, 0, (uint8_t*)presentJob, sizeof(DEVICE_SCHEDJOB_FLASH));
   }
   else
   {
	   readed = UtilsFlashManager::getFlashArea(PARTITION_TYPE_DEVICE_JOB_BKUP, 0, (uint8_t*)presentJob, sizeof(DEVICE_SCHEDJOB_FLASH));
   }

   if(readed == true)
   {
	   if(presentJob->crc.crc != calculatedCrc)
	   {
	        /* Crc are different... */
		    ERROR_LOG("Different crc in ScheJob page");
			isPageValid = false;
	   }
		else
	   {
	      /* Crc are valid, check padding magic number */
	      if(presentJob->crc.magicNumber == UINT64_MAGIC_NUMBER)
	      {
	         /* Even magic number are ok*/
	         isPageValid = true;
	      }
	      else
	      {
	         /* Magic number not OK */
	    	 ERROR_LOG("Different magic number in ScheJob page");
	         isPageValid = false;
	      }
	   }
   }
   else
   {
	   ERROR_LOG("Error reading ScheJob partition");
	   isPageValid = false;
   }


   return isPageValid;
}
