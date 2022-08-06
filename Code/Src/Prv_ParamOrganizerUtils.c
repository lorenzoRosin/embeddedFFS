/**
 * @file Prv_ParamOrganizerUtils.c
 *
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "Prv_ParamOrganizerUtils.h"



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_paramOrgResult getPagePrmFromRamBuff(s_paramOrgContext* prmCntx, uint8_t* page, s_prv_paramOrgPageParam* prmPage)
{
    /* Local variable */
    e_paramOrgResult returnVal;

    /* Check for NULL pointer */
    if( ( NULL == prmCntx ) || ( NULL == page ) || ( NULL == prmPage ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        /* Check if initialized */
        if( false == prmCntx->isInitialized )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            /* Copy data in ram buffer in to the structure */
            uint32_t offset1 = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);
            uint32_t offset2 = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);
            uint32_t offset3 = sizeof(uint32_t) + sizeof(uint32_t);
            uint32_t offset4 = sizeof(uint32_t);

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
    /* Local variable */
    e_paramOrgResult returnVal;

    /* Check for NULL pointer */
    if( ( NULL == prmCntx ) || ( NULL == page ) || ( NULL == prmPage ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        /* Check if initialized */
        if( false == prmCntx->isInitialized )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            /* Copy data in structure in to the ram buffer */
            uint32_t offset1 = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);
            uint32_t offset2 = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);
            uint32_t offset3 = sizeof(uint32_t) + sizeof(uint32_t);
            uint32_t offset4 = sizeof(uint32_t);

            (void)memcpy( &page[prmCntx->pageSize - offset1], (uint8_t*)&prmPage->pageTimeSetted,  sizeof(uint32_t) );
            (void)memcpy( &page[prmCntx->pageSize - offset2], (uint8_t*)&prmPage->pageType,        sizeof(uint32_t) );
            (void)memcpy( &page[prmCntx->pageSize - offset3], (uint8_t*)&prmPage->pageMagicNumber, sizeof(uint32_t) );
            (void)memcpy( &page[prmCntx->pageSize - offset4], (uint8_t*)&prmPage->pageCrc,         sizeof(uint32_t) );

            returnVal = PARAMRES_ALLOK;
        }
    }

    return returnVal;
}

e_paramOrgResult setCrcInPagePrmRamBuff(s_paramOrgContext* prmCntx, uint8_t* page, uint32_t* crcToSet)
{
    /* Local variable */
    e_paramOrgResult returnVal;

    /* Check for NULL pointer */
    if( ( NULL == prmCntx ) || ( NULL == page ) || ( NULL == crcToSet ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        /* Check if initialized */
        if( false == prmCntx->isInitialized )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            /* Copy CRC passed in to the ram buffer */
            uint32_t offset4 = sizeof(uint32_t);
            (void)memcpy( &page[prmCntx->pageSize - offset4], (uint8_t*)crcToSet, sizeof(uint32_t) );
            returnVal = PARAMRES_ALLOK;
        }
    }

    return returnVal;
}

e_paramOrgResult calcPagePrmCrcInRamBuff(s_paramOrgContext* prmCntx, uint8_t* page, uint32_t* crcCalculated)
{
    /* Local variable */
    e_paramOrgResult returnVal;
    uint32_t pageCrcSizeToCalc;

    /* Check for NULL pointer */
    if( ( NULL == prmCntx ) || ( NULL == page ) || ( NULL == crcCalculated ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        /* Check if initialized */
        if( false == prmCntx->isInitialized )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            /* Esclude from the calculation the CRC, that is placed in the last 4 byte */
            pageCrcSizeToCalc = prmCntx->pageSize - sizeof(uint32_t);
            if( false == (*(prmCntx->pToCrcFunc))(crcCalculated, pageCrcSizeToCalc, CRC_BASE_SEED) )
            {
                /* Generic CRC calculation Fail */
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

e_paramOrgResult isValidDataInPageRam(s_paramOrgContext* prmCntx, uint8_t* page)
{
    /* Local variable */
    e_paramOrgResult returnVal;
    s_prv_paramOrgPageParam prmPage;
    uint32_t crcCalculated;

    /* Check for NULL pointer */
    if( ( NULL == prmCntx ) || ( NULL == page ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        /* Check if initialized */
        if( false == prmCntx->isInitialized )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            /* Get page param from the ram page */
            returnVal = getPagePrmFromRamBuff(prmCntx, page, &prmPage);

            if( PARAMRES_ALLOK == returnVal )
            {
                /* Calculate the page param CRC */
                returnVal = calcPagePrmCrcInRamBuff(prmCntx, page, &crcCalculated);

                if( PARAMRES_ALLOK == returnVal )
                {
                    /* Verify if calculated CRC is equal to the stored one, check if magic number is equal to the
                     * setted one also */
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

    return returnVal;
}
