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
}s_paramOrgPageParam;

typedef enum t_paramOrgPageType
{
    PARAMRES_TYPE_PARAM = 0,
    PARAMRES_TYPE_RAW,
}e_paramOrgPageType;

/**********************
 *   PRIVATE STATIC FUNCTIONS PROTOTYPES
 **********************/
e_paramOrgResult getPrgRamPageParam(s_paramOrgContext* prmCntx, uint8_t* page, s_paramOrgPageParam* prmPage);
e_paramOrgResult setPrgRamPageParam(s_paramOrgContext* prmCntx, uint8_t* page, s_paramOrgPageParam* prmPage);
e_paramOrgResult setCrcPrgRamPageParam(s_paramOrgContext* prmCntx, uint8_t* page, uint32_t* crcToSet);
e_paramOrgResult calcPrgPageParamCrc(s_paramOrgContext* prmCntx, uint8_t* page, uint32_t* crcCalculated);
e_paramOrgResult isValidDataInPage(s_paramOrgContext* prmCntx, uint32_t pageOffsetFromId);

e_paramOrgResult verifyAndGeneratePageIntegrityNoBkup(s_paramOrgContext* prmCntx, uint32_t pageOffsetFromId);
e_paramOrgResult verifyAndGeneratePageIntegrityBkup(s_paramOrgContext* prmCntx, uint32_t pageOffsetFromId);





e_paramOrgResult verifyAndGenerateAllPageIntegrityRaw(s_paramOrgContext* prmCntx);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

e_paramOrgResult initParamSettings(s_paramOrgContext* prmCntx, const s_paramOrgInitParameter* prmInitVal)
{
    e_paramOrgResult returnVal = PARAMRES_ALLOK;

    if( ( NULL == prmCntx ) || ( NULL == prmInitVal ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        if( true == prmCntx->isInitialized )
        {
            returnVal = PARAMRES_BADPARAM;
        }
        else
        {
            if( ( prmInitVal->pageSize < PARAMORGAN_MIN_PAGE_SIZE_BYTE ) ||
                ( prmInitVal->pageSize > PARAMORGAN_MAX_PAGE_SIZE_BYTE ) || ( 0u == prmInitVal->nOfPages ) )
            {
                returnVal = PARAMRES_BADPARAM;
                prmCntx->isInitialized = false;
            }

            if( PARAMRES_ALLOK == returnVal )
            {
                if( 0u != ( prmInitVal->pageSize % PARAMORGAN_DIVISOR_PAGE_SIZE_BYTE )  )
                {
                    returnVal = PARAMRES_BADPARAM;
                    prmCntx->isInitialized = false;
                }
            }

            if( PARAMRES_ALLOK == returnVal )
            {
                if( 0u != ( prmInitVal->paramHandlerFlag & ( PARAMORGAN_BKPPAGE_EN | PARAMORGAN_RAW_MODE ) )  )
                {
                    returnVal = PARAMRES_BADPARAM;
                    prmCntx->isInitialized = false;
                }
            }

            if( PARAMRES_ALLOK == returnVal )
            {
                if( PARAMORGAN_BKPPAGE_EN == ( prmInitVal->paramHandlerFlag & PARAMORGAN_BKPPAGE_EN )  )
                {
                    if( 0u != ( prmInitVal->nOfPages % PARAMORGAN_DIVISOR_PAGE_BACKUP ) )
                    {
                        returnVal = PARAMRES_BADPARAM;
                        prmCntx->isInitialized = false;
                    }
                }
            }

            if( PARAMRES_ALLOK == returnVal )
            {
                if( ( NULL == prmInitVal->pToReadFunc  ) || ( NULL == prmInitVal->pToWriteFunc ) ||
                    ( NULL == prmInitVal->pToEraseFunc ) || ( NULL == prmInitVal->pToCrcFunc   ) )
                {
                    returnVal = PARAMRES_BADPARAM;
                    prmCntx->isInitialized = false;
                }
            }

            if( PARAMRES_ALLOK == returnVal )
            {
                if( ( NULL == prmInitVal->memPoolPointer  ) || ( prmInitVal->memPoolSize < prmInitVal->pageSize ) )
                {
                    returnVal = PARAMRES_BADPARAM;
                    prmCntx->isInitialized = false;
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

                prmCntx->isInitialized = true;
            }
        }
    }

    return returnVal;
}





/**********************
 *   PRIVATE STATIC FUNCTIONS
 **********************/
e_paramOrgResult getPrgRamPageParam(s_paramOrgContext* prmCntx, uint8_t* page, s_paramOrgPageParam* prmPage)
{
    e_paramOrgResult returnVal;

    if( ( NULL == prmCntx ) || ( NULL == page ) || ( NULL == prmPage ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        if( false == prmCntx->isInitialized )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            uint32_t offset1 = sizeof(s_paramOrgPageParam);
            uint32_t offset2 = sizeof(s_paramOrgPageParam) - sizeof(uint32_t);
            uint32_t offset3 = sizeof(s_paramOrgPageParam) - sizeof(uint32_t)  - sizeof(uint32_t);
            uint32_t offset4 = sizeof(s_paramOrgPageParam) - sizeof(uint32_t)  - sizeof(uint32_t) - sizeof(uint32_t);

            (void)memcpy( (uint8_t*)&prmPage->pageTimeSetted,  &page[prmCntx->pageSize - offset1], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&prmPage->pageType,        &page[prmCntx->pageSize - offset2], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&prmPage->pageMagicNumber, &page[prmCntx->pageSize - offset3], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&prmPage->pageCrc,         &page[prmCntx->pageSize - offset4], sizeof(uint32_t) );

            returnVal = PARAMRES_ALLOK;
        }
    }

    return returnVal;
}

e_paramOrgResult setPrgRamPageParam(s_paramOrgContext* prmCntx, uint8_t* page, s_paramOrgPageParam* prmPage)
{
    e_paramOrgResult returnVal;

    if( ( NULL == prmCntx ) || ( NULL == page ) || ( NULL == prmPage ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        if( false == prmCntx->isInitialized )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            uint32_t offset1 = sizeof(s_paramOrgPageParam);
            uint32_t offset2 = sizeof(s_paramOrgPageParam) - sizeof(uint32_t);
            uint32_t offset3 = sizeof(s_paramOrgPageParam) - sizeof(uint32_t)  - sizeof(uint32_t);
            uint32_t offset4 = sizeof(s_paramOrgPageParam) - sizeof(uint32_t)  - sizeof(uint32_t) - sizeof(uint32_t);

            (void)memcpy( (uint8_t*)&prmPage->pageTimeSetted,  &page[prmCntx->pageSize - offset1], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&prmPage->pageType,        &page[prmCntx->pageSize - offset2], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&prmPage->pageMagicNumber, &page[prmCntx->pageSize - offset3], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&prmPage->pageCrc,         &page[prmCntx->pageSize - offset4], sizeof(uint32_t) );

            returnVal = PARAMRES_ALLOK;
        }
    }

    return returnVal;
}

e_paramOrgResult setCrcPrgRamPageParam(s_paramOrgContext* prmCntx, uint8_t* page, uint32_t* crcToSet)
{
    e_paramOrgResult returnVal;

    if( ( NULL == prmCntx ) || ( NULL == page ) || ( NULL == crcToSet ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        if( false == prmCntx->isInitialized )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            (void)memcpy( &page[prmCntx->pageSize - sizeof(uint32_t)], crcToSet, sizeof(uint32_t) );
            returnVal = PARAMRES_ALLOK;
        }
    }

    return returnVal;
}


e_paramOrgResult calcPrgPageParamCrc(s_paramOrgContext* prmCntx, uint8_t* page, uint32_t* crcCalculated)
{
    e_paramOrgResult returnVal;
    uint32_t pageCrcSizeToCalc;

    if( ( NULL == prmCntx ) || ( NULL == page ) || ( NULL == crcCalculated ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        if( false == prmCntx->isInitialized )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            pageCrcSizeToCalc = prmCntx->pageSize - sizeof(uint32_t);
            if( false == (*(prmCntx->pToCrcFunc))(crcCalculated, pageCrcSizeToCalc, CRC_BASE_SEED) )
            {
                returnVal = PARAMRES_BADPARAM;
            }
            else
            {
                returnVal = PARAMRES_ALLOK;
            }
        }
    }

    return returnVal;
}





e_paramOrgResult isValidDataInPage(s_paramOrgContext* prmCntx, uint32_t pageOffsetFromId)
{
    e_paramOrgResult returnVal;
    s_paramOrgPageParam prmPage;
    uint32_t crcCalculated;

    if( NULL == prmCntx )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        if( false == prmCntx->isInitialized )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            if( pageOffsetFromId >= prmCntx->nOfPages )
            {
                returnVal = PARAMRES_BADPARAM;
            }
            else
            {
                if( false == (*(prmCntx->pToReadFunc))(prmCntx->pageId, pageOffsetFromId, prmCntx->pageSize, prmCntx->memPoolPointer) )
                {
                    returnVal = PARAMRES_ERRORREAD;
                }
                else
                {
                    returnVal = getPrgRamPageParam(prmCntx, prmCntx->memPoolPointer, &prmPage);

                    if( PARAMRES_ALLOK == returnVal )
                    {
                        returnVal = calcPrgPageParamCrc(prmCntx, prmCntx->memPoolPointer, &crcCalculated);

                        if( PARAMRES_ALLOK == returnVal )
                        {
                            if( (crcCalculated == prmPage.pageCrc) && ( PARAM_32_MAGIC_NUMBER == prmPage.pageMagicNumber ) )
                            {
                                returnVal = PARAMRES_ALLOK;
                            }
                            else
                            {
                                returnVal = PARAMRES_NOTVALIDPAGE;
                            }
                        }
                    }
                }
            }
        }
    }

    return returnVal;
}

e_paramOrgResult verifyAndGeneratePageIntegrityNoBkup(s_paramOrgContext* prmCntx, uint32_t pageOffsetFromId)
{
    e_paramOrgResult returnVal;
    e_paramOrgResult returnValValidPage;
    s_paramOrgPageParam prmPage;

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
        if( false == prmCntx->isInitialized )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            while( ( iterator < prmCntx->nOfPages ) && ( true == canContinueToIterate ) )
            {
                returnValValidPage = isValidDataInPage(prmCntx, iterator);
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
                        prmPage.pageType = PARAMRES_TYPE_RAW;
                        prmPage.pageMagicNumber = PARAM_32_MAGIC_NUMBER;
                        prmPage.pageCrc = 0u;

                        returnVal = setPrgRamPageParam(prmCntx, prmCntx->memPoolPointer, &prmPage);

                        if( PARAMRES_ALLOK == returnVal)
                        {
                            returnVal = calcPrgPageParamCrc(prmCntx, prmCntx->memPoolPointer, &crcCalculated);
                            if( PARAMRES_ALLOK == returnVal)
                            {
                                setCrcPrgRamPageParam(prmCntx, prmCntx->memPoolPointer, &crcCalculated);
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
    s_paramOrgPageParam prmPage;

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
        if( false == prmCntx->isInitialized )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            while( ( iterator < prmCntx->nOfPages ) && ( true == canContinueToIterate ) )
            {
                returnValValidPage = isValidDataInPage(prmCntx, iterator);
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
                        prmPage.pageType = PARAMRES_TYPE_RAW;
                        prmPage.pageMagicNumber = PARAM_32_MAGIC_NUMBER;
                        prmPage.pageCrc = 0u;

                        returnVal = setPrgRamPageParam(prmCntx, prmCntx->memPoolPointer, &prmPage);

                        if( PARAMRES_ALLOK == returnVal)
                        {
                            returnVal = calcPrgPageParamCrc(prmCntx, prmCntx->memPoolPointer, &crcCalculated);
                            if( PARAMRES_ALLOK == returnVal)
                            {
                                setCrcPrgRamPageParam(prmCntx, prmCntx->memPoolPointer, &crcCalculated);
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




