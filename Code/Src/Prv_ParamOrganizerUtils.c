/**
 * @file Prv_ParamOrganizerUtils.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "Prv_ParamOrganizerUtils.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      PRIVATE TYPEDEFS
 **********************/


/**********************
 *   PRIVATE STATIC FUNCTIONS PROTOTYPES
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





/**********************
 *   PRIVATE STATIC FUNCTIONS
 **********************/
e_paramOrgResult getPagePrmFromRamBuff(s_paramOrgContext* prmCntx, uint8_t* page, s_prv_paramOrgPageParam* prmPage)
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
            uint32_t offset1 = sizeof(s_prv_paramOrgPageParam);
            uint32_t offset2 = sizeof(s_prv_paramOrgPageParam) - sizeof(uint32_t);
            uint32_t offset3 = sizeof(s_prv_paramOrgPageParam) - sizeof(uint32_t)  - sizeof(uint32_t);
            uint32_t offset4 = sizeof(s_prv_paramOrgPageParam) - sizeof(uint32_t)  - sizeof(uint32_t) - sizeof(uint32_t);

            (void)memcpy( (uint8_t*)&prmPage->pageTimeSetted,  &page[prmCntx->pageSize - offset1], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&prmPage->pageType,        &page[prmCntx->pageSize - offset2], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&prmPage->pageMagicNumber, &page[prmCntx->pageSize - offset3], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&prmPage->pageCrc,         &page[prmCntx->pageSize - offset4], sizeof(uint32_t) );

            returnVal = PARAMRES_ALLOK;
        }
    }

    return returnVal;
}

e_paramOrgResult setPagePrmInRamBuff(s_paramOrgContext* prmCntx, uint8_t* page, s_prv_paramOrgPageParam* prmPage)
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
            uint32_t offset1 = sizeof(s_prv_paramOrgPageParam);
            uint32_t offset2 = sizeof(s_prv_paramOrgPageParam) - sizeof(uint32_t);
            uint32_t offset3 = sizeof(s_prv_paramOrgPageParam) - sizeof(uint32_t)  - sizeof(uint32_t);
            uint32_t offset4 = sizeof(s_prv_paramOrgPageParam) - sizeof(uint32_t)  - sizeof(uint32_t) - sizeof(uint32_t);

            (void)memcpy( (uint8_t*)&prmPage->pageTimeSetted,  &page[prmCntx->pageSize - offset1], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&prmPage->pageType,        &page[prmCntx->pageSize - offset2], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&prmPage->pageMagicNumber, &page[prmCntx->pageSize - offset3], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&prmPage->pageCrc,         &page[prmCntx->pageSize - offset4], sizeof(uint32_t) );

            returnVal = PARAMRES_ALLOK;
        }
    }

    return returnVal;
}

e_paramOrgResult setCrcInPagePrmRamBuff(s_paramOrgContext* prmCntx, uint8_t* page, uint32_t* crcToSet)
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

e_paramOrgResult calcPagePrmCrcInRamBuff(s_paramOrgContext* prmCntx, uint8_t* page, uint32_t* crcCalculated)
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
    s_prv_paramOrgPageParam prmPage;
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
                    returnVal = getPagePrmFromRamBuff(prmCntx, prmCntx->memPoolPointer, &prmPage);

                    if( PARAMRES_ALLOK == returnVal )
                    {
                        returnVal = calcPagePrmCrcInRamBuff(prmCntx, prmCntx->memPoolPointer, &crcCalculated);

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
