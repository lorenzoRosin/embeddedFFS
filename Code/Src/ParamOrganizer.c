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
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

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

e_paramOrgResult initializeParamCallBack(s_paramOrgContext* prmCntx, fc_readPage rpClb, fc_writePage wpClb,
                                         fc_erasePage epClb, fc_calculateCrc32 cc32Clb)
{
    e_paramOrgResult returnVal;

    if( NULL == prmCntx )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        returnVal = PARAMRES_ALLOK;
    }

    return returnVal;
}
/**********************
 *   STATIC FUNCTIONS
 **********************/
