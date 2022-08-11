/**
 * @file eFSS.c
 *
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSS.h"
#include "Prv_eFSSUtils.h"

/***********************************************************************************************************************
 *      DEFINES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *   PRIVATE STATIC FUNCTIONS PROTOTYPES
 **********************/
e_eFSS_Res verifyAndGeneratePageIntegrityNoBkup(s_eFSS_Ctx* prmCntx, uint32_t pageOffsetFromId);
e_eFSS_Res verifyAndGeneratePageIntegrityBkup(s_eFSS_Ctx* prmCntx, uint32_t pageOffsetFromId);

e_eFSS_Res verifyAndGenerateAllPageIntegrityRaw(s_eFSS_Ctx* prmCntx);

/***********************************************************************************************************************
 *  STATIC VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/

e_eFSS_Res initParamSettings(s_eFSS_Ctx* prmCntx, const s_eFSS_InitParam* prmInitVal)
{
    e_eFSS_Res returnVal = EFSS_RES_OK;

    if( ( NULL == prmCntx ) || ( NULL == prmInitVal ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        if( true == prmCntx->isInitializedParams )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            if( ( prmInitVal->pageSize < EFSS_MIN_PAGE_SIZE_BYTE ) ||
                ( prmInitVal->pageSize > EFSS_MAX_PAGE_SIZE_BYTE ) || ( 0u == prmInitVal->nOfPages ) )
            {
                returnVal = EFSS_RES_BADPARAM;
                prmCntx->isInitializedParams = false;
            }

            if( EFSS_RES_OK == returnVal )
            {
                if( 0u != ( prmInitVal->pageSize % EFSS_DIVISOR_PAGE_SIZE_BYTE )  )
                {
                    returnVal = EFSS_RES_BADPARAM;
                    prmCntx->isInitializedParams = false;
                }
            }

            // if( EFSS_RES_OK == returnVal )
            // {
            //     if( 0u != ( prmInitVal->paramHandlerFlag & ( EFSS_BKPPAGE_EN | EFSS_RAW_MODE ) )  )
            //     {
            //         returnVal = EFSS_RES_BADPARAM;
            //         prmCntx->isInitializedParams = false;
            //     }
            // }

            if( EFSS_RES_OK == returnVal )
            {
                if( EFSS_BKPPAGE_EN == ( prmInitVal->paramHandlerFlag & EFSS_BKPPAGE_EN )  )
                {
                    if( 0u != ( prmInitVal->nOfPages % EFSS_DIVISOR_PAGE_BACKUP ) )
                    {
                        returnVal = EFSS_RES_BADPARAM;
                        prmCntx->isInitializedParams = false;
                    }
                }
            }

            if( EFSS_RES_OK == returnVal )
            {
                if( ( NULL == prmInitVal->cbHolder.pReadPg  ) || ( NULL == prmInitVal->cbHolder.pWritePg ) ||
                    ( NULL == prmInitVal->cbHolder.pErasePg ) || ( NULL == prmInitVal->cbHolder.pCrc32   ) )
                {
                    returnVal = EFSS_RES_BADPARAM;
                    prmCntx->isInitializedParams = false;
                }
            }

            if( EFSS_RES_OK == returnVal )
            {
                if( ( NULL == prmInitVal->memPoolPointer  ) || ( prmInitVal->memPoolSize < prmInitVal->pageSize ) )
                {
                    returnVal = EFSS_RES_BADPARAM;
                    prmCntx->isInitializedParams = false;
                }
            }

            if( EFSS_RES_OK == returnVal )
            {
                prmCntx->pageSize = prmInitVal->pageSize;
                prmCntx->nOfPages = prmInitVal->nOfPages;
                prmCntx->pageId = prmInitVal->pageId;

                prmCntx->paramHandlerFlag = prmInitVal->paramHandlerFlag;

                prmCntx->cbHolder.pReadPg  = prmInitVal->cbHolder.pReadPg;
                prmCntx->cbHolder.pWritePg = prmInitVal->cbHolder.pWritePg;
                prmCntx->cbHolder.pErasePg = prmInitVal->cbHolder.pErasePg;
                prmCntx->cbHolder.pCrc32   = prmInitVal->cbHolder.pCrc32;

                prmCntx->memPoolPointer = prmInitVal->memPoolPointer;
                prmCntx->memPoolSize = prmInitVal->memPoolSize;

                prmCntx->isInitializedParams = true;
            }
        }
    }

    return returnVal;
}





/***********************************************************************************************************************
 *   PRIVATE STATIC FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_Res verifyAndGeneratePageIntegrityNoBkup(s_eFSS_Ctx* prmCntx, uint32_t pageOffsetFromId)
{
    e_eFSS_Res returnVal;
    e_eFSS_Res returnValValidPage;
    s_prv_pagePrm pagePrm;

    uint32_t iterator = 0u;
    uint32_t howManyReInit = 0u;
    uint32_t crcCalculated = 0u;

    bool_t canContinueToIterate = true;

    if( NULL == prmCntx )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        if( false == prmCntx->isInitializedParams )
        {
            returnVal = EFSS_RES_NOT_INIT;
        }
        else
        {
            while( ( iterator < prmCntx->nOfPages ) && ( true == canContinueToIterate ) )
            {
                if( false == (*(prmCntx->cbHolder.pReadPg))(prmCntx->pageId, pageOffsetFromId, prmCntx->pageSize, prmCntx->memPoolPointer) )
                {
                    returnVal = EFSS_RES_ERRORREAD;
                }

                returnValValidPage = isValidPageInBuff(prmCntx->pageSize, prmCntx->memPoolPointer, prmCntx->cbHolder, prmCntx->pageType);
                switch(returnValValidPage)
                {
                    case(EFSS_RES_OK):
                    {
                        canContinueToIterate = true;
                    }break;

                    case(EFSS_RES_NOTVALIDPAGE):
                    {
                        howManyReInit++;

                        memset(prmCntx->memPoolPointer, 0, prmCntx->pageSize);

                        pagePrm.pageType = EFSS_PAGETYPE_RAW;
                        pagePrm.pageMagicNumber = PARAM_32_MAGIC_NUMBER;
                        pagePrm.pageCrc = 0u;

                        returnVal = setPagePrmInBuff(prmCntx->pageSize, prmCntx->memPoolPointer, &pagePrm);

                        if( EFSS_RES_OK == returnVal)
                        {
                            returnVal = calcPagePrmCrcInBuff(prmCntx->pageSize, prmCntx->memPoolPointer, prmCntx->cbHolder, &crcCalculated);
                            if( EFSS_RES_OK == returnVal)
                            {
                                setCrcInPagePrmBuff(prmCntx->pageSize, prmCntx->memPoolPointer, crcCalculated);
                                if( EFSS_RES_OK == returnVal)
                                {
                                    if( false == (*(prmCntx->cbHolder.pWritePg))(prmCntx->pageId, iterator,
                                                                            prmCntx->pageSize, prmCntx->memPoolPointer) )
                                    {
                                        returnVal = EFSS_RES_ERRORREAD;
                                        canContinueToIterate = true;
                                    }
                                    else
                                    {
                                        returnVal = EFSS_RES_OK;
                                    }
                                }
                                else
                                {
                                    canContinueToIterate = false;
                                }
                            }
                            else
                            {
                                canContinueToIterate = false;
                            }
                        }
                        else
                        {
                            canContinueToIterate = false;
                        }
                    }break;

                    default:
                    {
                        canContinueToIterate = false;
                    }
                }

                iterator++;
            }
        }
    }

    return returnVal;
}


e_eFSS_Res verifyAndGeneratePageIntegrityBkup(s_eFSS_Ctx* prmCntx, uint32_t pageOffsetFromId)
{


}












































































e_eFSS_Res verifyAndGenerateAllPageIntegrityRaw(s_eFSS_Ctx* prmCntx)
{
    e_eFSS_Res returnVal;
    e_eFSS_Res returnValValidPage;
    s_prv_pagePrm pagePrm;

    uint32_t iterator = 0u;
    uint32_t howManyReInit = 0u;
    uint32_t crcCalculated = 0u;

    bool_t canContinueToIterate = true;

    if( NULL == prmCntx )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        if( false == prmCntx->isInitializedParams )
        {
            returnVal = EFSS_RES_NOT_INIT;
        }
        else
        {
            while( ( iterator < prmCntx->nOfPages ) && ( true == canContinueToIterate ) )
            {
                if( false == (*(prmCntx->cbHolder.pReadPg))(prmCntx->pageId, iterator, prmCntx->pageSize, prmCntx->memPoolPointer) )
                {
                    returnVal = EFSS_RES_ERRORREAD;
                }

                returnValValidPage = isValidPageInBuff(prmCntx->pageSize, prmCntx->memPoolPointer, prmCntx->cbHolder, prmCntx->pageType);
                switch(returnValValidPage)
                {
                    case(EFSS_RES_OK):
                    {
                        canContinueToIterate = true;
                    }break;

                    case(EFSS_RES_NOTVALIDPAGE):
                    {
                        howManyReInit++;

                        memset(prmCntx->memPoolPointer, 0, prmCntx->pageSize);

                        pagePrm.pageType = EFSS_PAGETYPE_RAW;
                        pagePrm.pageMagicNumber = PARAM_32_MAGIC_NUMBER;
                        pagePrm.pageCrc = 0u;

                        returnVal = setPagePrmInBuff(prmCntx->pageSize, prmCntx->memPoolPointer, &pagePrm);

                        if( EFSS_RES_OK == returnVal)
                        {
                            returnVal = calcPagePrmCrcInBuff(prmCntx->pageSize, prmCntx->memPoolPointer, prmCntx->cbHolder, &crcCalculated);
                            if( EFSS_RES_OK == returnVal)
                            {
                                setCrcInPagePrmBuff(prmCntx->pageSize, prmCntx->memPoolPointer, crcCalculated);
                                if( EFSS_RES_OK == returnVal)
                                {
                                    if( false == (*(prmCntx->cbHolder.pWritePg))(prmCntx->pageId, iterator,
                                                                            prmCntx->pageSize, prmCntx->memPoolPointer) )
                                    {
                                        returnVal = EFSS_RES_ERRORREAD;
                                        canContinueToIterate = true;
                                    }
                                    else
                                    {
                                        returnVal = EFSS_RES_OK;
                                    }
                                }
                                else
                                {
                                    canContinueToIterate = false;
                                }
                            }
                            else
                            {
                                canContinueToIterate = false;
                            }
                        }
                        else
                        {
                            canContinueToIterate = false;
                        }
                    }break;

                    default:
                    {
                        canContinueToIterate = false;
                    }
                }

                iterator++;
            }
        }
    }

    return returnVal;
}




