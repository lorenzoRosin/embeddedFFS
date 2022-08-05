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

/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
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


/**********************
 *   STATIC FUNCTIONS
 **********************/
