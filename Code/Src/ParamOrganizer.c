/**
 * @file ParamOrganizer.c
 *
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "ParamOrganizer.h"
#include "Prv_ParamOrganizerUtils.h"

/***********************************************************************************************************************
 *      DEFINES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *   PRIVATE STATIC FUNCTIONS PROTOTYPES
 **********************/
e_paramOrgResult verifyAndGeneratePageIntegrityNoBkup(s_paramOrgContext* prmCntx, uint32_t pageOffsetFromId);
e_paramOrgResult verifyAndGeneratePageIntegrityBkup(s_paramOrgContext* prmCntx, uint32_t pageOffsetFromId);

e_paramOrgResult verifyAndGenerateAllPageIntegrityRaw(s_paramOrgContext* prmCntx);

/***********************************************************************************************************************
 *  STATIC VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/

e_paramOrgResult initParamSettings(s_paramOrgContext* prmCntx, const s_paramOrgInitParameter* prmInitVal)
{
    e_paramOrgResult returnVal = PARAMRES_ALLOK;

    if( ( NULL == prmCntx ) || ( NULL == prmInitVal ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        if( true == prmCntx->isInitializedParams )
        {
            returnVal = PARAMRES_BADPARAM;
        }
        else
        {
            if( ( prmInitVal->pageSize < PARAMORGAN_MIN_PAGE_SIZE_BYTE ) ||
                ( prmInitVal->pageSize > PARAMORGAN_MAX_PAGE_SIZE_BYTE ) || ( 0u == prmInitVal->nOfPages ) )
            {
                returnVal = PARAMRES_BADPARAM;
                prmCntx->isInitializedParams = false;
            }

            if( PARAMRES_ALLOK == returnVal )
            {
                if( 0u != ( prmInitVal->pageSize % PARAMORGAN_DIVISOR_PAGE_SIZE_BYTE )  )
                {
                    returnVal = PARAMRES_BADPARAM;
                    prmCntx->isInitializedParams = false;
                }
            }

            // if( PARAMRES_ALLOK == returnVal )
            // {
            //     if( 0u != ( prmInitVal->paramHandlerFlag & ( PARAMORGAN_BKPPAGE_EN | PARAMORGAN_RAW_MODE ) )  )
            //     {
            //         returnVal = PARAMRES_BADPARAM;
            //         prmCntx->isInitializedParams = false;
            //     }
            // }

            if( PARAMRES_ALLOK == returnVal )
            {
                if( PARAMORGAN_BKPPAGE_EN == ( prmInitVal->paramHandlerFlag & PARAMORGAN_BKPPAGE_EN )  )
                {
                    if( 0u != ( prmInitVal->nOfPages % PARAMORGAN_DIVISOR_PAGE_BACKUP ) )
                    {
                        returnVal = PARAMRES_BADPARAM;
                        prmCntx->isInitializedParams = false;
                    }
                }
            }

            if( PARAMRES_ALLOK == returnVal )
            {
                if( ( NULL == prmInitVal->pToReadFunc  ) || ( NULL == prmInitVal->pToWriteFunc ) ||
                    ( NULL == prmInitVal->pToEraseFunc ) || ( NULL == prmInitVal->pToCrcFunc   ) )
                {
                    returnVal = PARAMRES_BADPARAM;
                    prmCntx->isInitializedParams = false;
                }
            }

            if( PARAMRES_ALLOK == returnVal )
            {
                if( ( NULL == prmInitVal->memPoolPointer  ) || ( prmInitVal->memPoolSize < prmInitVal->pageSize ) )
                {
                    returnVal = PARAMRES_BADPARAM;
                    prmCntx->isInitializedParams = false;
                }
            }

            if( PARAMRES_ALLOK == returnVal )
            {
                prmCntx->pageSize = prmInitVal->pageSize;
                prmCntx->nOfPages = prmInitVal->nOfPages;
                prmCntx->pageId = prmInitVal->pageId;

                prmCntx->paramHandlerFlag = prmInitVal->paramHandlerFlag;

                prmCntx->pToReadFunc = prmInitVal->pToReadFunc;
                prmCntx->pToWriteFunc = prmInitVal->pToWriteFunc;
                prmCntx->pToEraseFunc = prmInitVal->pToEraseFunc;
                prmCntx->pToCrcFunc = prmInitVal->pToCrcFunc;

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
e_paramOrgResult verifyAndGeneratePageIntegrityNoBkup(s_paramOrgContext* prmCntx, uint32_t pageOffsetFromId)
{
    e_paramOrgResult returnVal;
    e_paramOrgResult returnValValidPage;
    s_prv_paramOrgPageParam prmPage;

    uint32_t iterator = 0u;
    uint32_t howManyReInit = 0u;
    uint32_t crcCalculated = 0u;

    bool_t canContinueToIterate = true;

    if( NULL == prmCntx )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        if( false == prmCntx->isInitializedParams )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            while( ( iterator < prmCntx->nOfPages ) && ( true == canContinueToIterate ) )
            {
                if( false == (*(prmCntx->pToReadFunc))(prmCntx->pageId, pageOffsetFromId, prmCntx->pageSize, prmCntx->memPoolPointer) )
                {
                    returnVal = PARAMRES_ERRORREAD;
                }

                returnValValidPage = isValidDataInPageRamBuff(prmCntx, prmCntx->memPoolPointer);
                switch(returnValValidPage)
                {
                    case(PARAMRES_ALLOK):
                    {
                        canContinueToIterate = true;
                    }break;

                    case(PARAMRES_NOTVALIDPAGE):
                    {
                        howManyReInit++;

                        memset(prmCntx->memPoolPointer, 0, prmCntx->pageSize);

                        prmPage.pageTimeSetted = 1u;
                        prmPage.pageType = PARAMRES_PAGETYPE_RAW;
                        prmPage.pageMagicNumber = PARAM_32_MAGIC_NUMBER;
                        prmPage.pageCrc = 0u;

                        returnVal = setPagePrmInRamBuff(prmCntx, prmCntx->memPoolPointer, &prmPage);

                        if( PARAMRES_ALLOK == returnVal)
                        {
                            returnVal = calcPagePrmCrcInRamBuff(prmCntx, prmCntx->memPoolPointer, &crcCalculated);
                            if( PARAMRES_ALLOK == returnVal)
                            {
                                setCrcInPagePrmRamBuff(prmCntx, prmCntx->memPoolPointer, crcCalculated);
                                if( PARAMRES_ALLOK == returnVal)
                                {
                                    if( false == (*(prmCntx->pToWriteFunc))(prmCntx->pageId, iterator,
                                                                            prmCntx->pageSize, prmCntx->memPoolPointer) )
                                    {
                                        returnVal = PARAMRES_ERRORREAD;
                                        canContinueToIterate = true;
                                    }
                                    else
                                    {
                                        returnVal = PARAMRES_ALLOK;
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


e_paramOrgResult verifyAndGeneratePageIntegrityBkup(s_paramOrgContext* prmCntx, uint32_t pageOffsetFromId)
{


}












































































e_paramOrgResult verifyAndGenerateAllPageIntegrityRaw(s_paramOrgContext* prmCntx)
{
    e_paramOrgResult returnVal;
    e_paramOrgResult returnValValidPage;
    s_prv_paramOrgPageParam prmPage;

    uint32_t iterator = 0u;
    uint32_t howManyReInit = 0u;
    uint32_t crcCalculated = 0u;

    bool_t canContinueToIterate = true;

    if( NULL == prmCntx )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        if( false == prmCntx->isInitializedParams )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            while( ( iterator < prmCntx->nOfPages ) && ( true == canContinueToIterate ) )
            {
                if( false == (*(prmCntx->pToReadFunc))(prmCntx->pageId, iterator, prmCntx->pageSize, prmCntx->memPoolPointer) )
                {
                    returnVal = PARAMRES_ERRORREAD;
                }

                returnValValidPage = isValidDataInPageRamBuff(prmCntx, prmCntx->memPoolPointer);
                switch(returnValValidPage)
                {
                    case(PARAMRES_ALLOK):
                    {
                        canContinueToIterate = true;
                    }break;

                    case(PARAMRES_NOTVALIDPAGE):
                    {
                        howManyReInit++;

                        memset(prmCntx->memPoolPointer, 0, prmCntx->pageSize);

                        prmPage.pageTimeSetted = 1u;
                        prmPage.pageType = PARAMRES_PAGETYPE_RAW;
                        prmPage.pageMagicNumber = PARAM_32_MAGIC_NUMBER;
                        prmPage.pageCrc = 0u;

                        returnVal = setPagePrmInRamBuff(prmCntx, prmCntx->memPoolPointer, &prmPage);

                        if( PARAMRES_ALLOK == returnVal)
                        {
                            returnVal = calcPagePrmCrcInRamBuff(prmCntx, prmCntx->memPoolPointer, &crcCalculated);
                            if( PARAMRES_ALLOK == returnVal)
                            {
                                setCrcInPagePrmRamBuff(prmCntx, prmCntx->memPoolPointer, crcCalculated);
                                if( PARAMRES_ALLOK == returnVal)
                                {
                                    if( false == (*(prmCntx->pToWriteFunc))(prmCntx->pageId, iterator,
                                                                            prmCntx->pageSize, prmCntx->memPoolPointer) )
                                    {
                                        returnVal = PARAMRES_ERRORREAD;
                                        canContinueToIterate = true;
                                    }
                                    else
                                    {
                                        returnVal = PARAMRES_ALLOK;
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




