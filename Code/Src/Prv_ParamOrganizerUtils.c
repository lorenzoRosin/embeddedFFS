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
e_paramOrgResult getPagePrmFromRamBuff(s_paramOrgContext* prmCntx, uint8_t* pageBuff, s_prv_paramOrgPageParam* prmPage)
{
    /* Local variable */
    e_paramOrgResult returnVal;

    /* Check for NULL pointer */
    if( ( NULL == prmCntx ) || ( NULL == pageBuff ) || ( NULL == prmPage ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        /* Check if initialized */
        if( false == prmCntx->isInitializedParams )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            /* Copy data in ram buffer in to the structure */
            uint32_t offset1 = prmCntx->pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) + sizeof(uint32_t) );
            uint32_t offset2 = prmCntx->pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) );
            uint32_t offset3 = prmCntx->pageSize - ( sizeof(uint64_t) + sizeof(uint16_t) + sizeof(uint8_t) );
            uint32_t offset4 = prmCntx->pageSize - ( sizeof(uint64_t) + sizeof(uint16_t) );
            uint32_t offset5 = prmCntx->pageSize - ( sizeof(uint64_t) );
            uint32_t offset6 = prmCntx->pageSize - ( sizeof(uint32_t) );

            (void)memcpy( (uint8_t*)&prmPage->pageTimeSetted,         &pageBuff[offset1], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&prmPage->pageType,               &pageBuff[offset2], sizeof(uint8_t)  );
            (void)memcpy( (uint8_t*)&prmPage->allPageAlignmentNumber, &pageBuff[offset3], sizeof(uint8_t)  );
            (void)memcpy( (uint8_t*)&prmPage->pageVersion,            &pageBuff[offset4], sizeof(uint16_t) );
            (void)memcpy( (uint8_t*)&prmPage->pageMagicNumber,        &pageBuff[offset5], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&prmPage->pageCrc,                &pageBuff[offset6], sizeof(uint32_t) );
            returnVal = PARAMRES_ALLOK;
        }
    }

    return returnVal;
}

e_paramOrgResult setPagePrmInRamBuff(s_paramOrgContext* prmCntx, uint8_t* pageBuff, s_prv_paramOrgPageParam* prmPage)
{
    /* Local variable */
    e_paramOrgResult returnVal;

    /* Check for NULL pointer */
    if( ( NULL == prmCntx ) || ( NULL == pageBuff ) || ( NULL == prmPage ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        /* Check if initialized */
        if( false == prmCntx->isInitializedParams )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            /* Copy data in structure in to the ram buffer */
            uint32_t offset1 = prmCntx->pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) + sizeof(uint32_t) );
            uint32_t offset2 = prmCntx->pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) );
            uint32_t offset3 = prmCntx->pageSize - ( sizeof(uint64_t) + sizeof(uint16_t) + sizeof(uint8_t) );
            uint32_t offset4 = prmCntx->pageSize - ( sizeof(uint64_t) + sizeof(uint16_t) );
            uint32_t offset5 = prmCntx->pageSize - ( sizeof(uint64_t) );
            uint32_t offset6 = prmCntx->pageSize - ( sizeof(uint32_t) );

            (void)memcpy( &pageBuff[offset1], (uint8_t*)&prmPage->pageTimeSetted,           sizeof(uint32_t) );
            (void)memcpy( &pageBuff[offset2], (uint8_t*)&prmPage->pageType,                 sizeof(uint8_t)  );
            (void)memcpy( &pageBuff[offset3], (uint8_t*)&prmPage->allPageAlignmentNumber,   sizeof(uint8_t)  );
            (void)memcpy( &pageBuff[offset4], (uint8_t*)&prmPage->pageVersion,              sizeof(uint16_t) );
            (void)memcpy( &pageBuff[offset5], (uint8_t*)&prmPage->pageMagicNumber,          sizeof(uint32_t) );
            (void)memcpy( &pageBuff[offset6], (uint8_t*)&prmPage->pageCrc,                  sizeof(uint32_t) );

            returnVal = PARAMRES_ALLOK;
        }
    }

    return returnVal;
}

e_paramOrgResult setCrcInPagePrmRamBuff(s_paramOrgContext* prmCntx, uint8_t* pageBuff, uint32_t crcToSet)
{
    /* Local variable */
    e_paramOrgResult returnVal;

    /* Check for NULL pointer */
    if( ( NULL == prmCntx ) || ( NULL == pageBuff ) || ( NULL == crcToSet ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        /* Check if initialized */
        if( false == prmCntx->isInitializedParams )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            /* Copy CRC passed in to the ram buffer */
            uint32_t offset6 = prmCntx->pageSize - ( sizeof(uint32_t) );
            (void)memcpy( &pageBuff[offset6], (uint8_t*)crcToSet, sizeof(uint32_t) );
            returnVal = PARAMRES_ALLOK;
        }
    }

    return returnVal;
}


e_paramOrgResult calcPagePrmCrcInRamBuff(s_paramOrgContext* prmCntx, uint8_t* pageBuff, uint32_t* crcCalculated)
{
    /* Local variable */
    e_paramOrgResult returnVal;
    uint32_t pageCrcSizeToCalc;

    /* Check for NULL pointer */
    if( ( NULL == prmCntx ) || ( NULL == pageBuff ) || ( NULL == crcCalculated ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        /* Check if initialized */
        if( false == prmCntx->isInitializedParams )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            /* Esclude from the calculation the CRC, that is placed in the last 4 byte */
            pageCrcSizeToCalc = prmCntx->pageSize - sizeof(uint32_t);
            if( false == (*(prmCntx->pToCrcFunc))(crcCalculated, pageBuff, pageCrcSizeToCalc, CRC_BASE_SEED) )
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


e_paramOrgResult setPagePrmInRamBuffNCrcUp(s_paramOrgContext* prmCntx, uint8_t* pageBuff, s_prv_paramOrgPageParam* prmPage)
{
    /* Local variable */
    e_paramOrgResult returnVal;
    uint32_t pageCrcSizeToCalc;
    uint32_t crcCalculated;

    /* Check for NULL pointer */
    if( ( NULL == prmCntx ) || ( NULL == pageBuff ) || ( NULL == prmPage ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        /* Check if initialized */
        if( false == prmCntx->isInitializedParams )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            /* Set the parameter in the ram buffer */
            returnVal = setPagePrmInRamBuff(prmCntx, pageBuff, prmPage);

            if( PARAMRES_ALLOK == returnVal)
            {
                returnVal = calcPagePrmCrcInRamBuff(prmCntx, pageBuff, &crcCalculated);

                if( PARAMRES_ALLOK == returnVal)
                {
                    /* Crc calculated, set it */
                    returnVal = setCrcInPagePrmRamBuff(prmCntx, pageBuff, crcCalculated);
                }
            }
        }
    }

    return returnVal;
}



e_paramOrgResult isValidDataInPageRamBuff(s_paramOrgContext* prmCntx, uint8_t* pageBuff)
{
    /* Local variable */
    e_paramOrgResult returnVal;
    s_prv_paramOrgPageParam prmPage;
    uint32_t crcCalculated;

    /* Check for NULL pointer */
    if( ( NULL == prmCntx ) || ( NULL == pageBuff ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        /* Check if initialized */
        if( false == prmCntx->isInitializedParams )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            /* Get pageBuff param from the ram pageBuff */
            returnVal = getPagePrmFromRamBuff(prmCntx, pageBuff, &prmPage);

            if( PARAMRES_ALLOK == returnVal )
            {
                /* Calculate the pageBuff param CRC */
                returnVal = calcPagePrmCrcInRamBuff(prmCntx, pageBuff, &crcCalculated);

                if( PARAMRES_ALLOK == returnVal )
                {
                    /* Verify if calculated CRC is equal to the stored one, check if magic number is equal to the
                     * setted one also */
                    if( (crcCalculated == prmPage.pageCrc) && ( PARAM_32_MAGIC_NUMBER == prmPage.pageMagicNumber ) )
                    {
                        /* Verify if the pageBuff type is what we are aspecting */
                        if( prmPage.pageType == ( (uint8_t)prmCntx->organizationType ) )
                        {
                            returnVal = PARAMRES_ALLOK;
                        }
                        else
                        {
                            returnVal = PARAMRES_NOTVALIDPAGE;
                        }
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

e_paramOrgResult isValidDataInPage(s_paramOrgContext* prmCntx, const uint32_t pageOffset)
{
    /* Local variable */
    e_paramOrgResult returnVal;
    s_prv_paramOrgPageParam prmPage;
    uint32_t crcCalculated;

    /* Check for NULL pointer */
    if( NULL == prmCntx )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        /* Check if initialized */
        if( false == prmCntx->isInitializedParams )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            /* Get pageBuff */
            if( false == (*(prmCntx->pToReadFunc))(prmCntx->pageId, pageOffset, prmCntx->pageSize, prmCntx->memPoolPointer) )
            {
                returnVal = PARAMRES_ERRORREAD;
            }
            else
            {
                /* now verify */
                returnVal = isValidDataInPageRamBuff(prmCntx, prmCntx->memPoolPointer);
            }
        }
    }

    return returnVal;
}

e_paramOrgResult writePageNPrmNUpdateCrc(s_paramOrgContext* prmCntx, const uint32_t pageOffset, uint8_t* pageBuff, s_prv_paramOrgPageParam* prmPage)
{
    /* Local variable */
    e_paramOrgResult returnVal;
    uint32_t pageCrcSizeToCalc;
    uint32_t crcCalculated;

    /* Check for NULL pointer */
    if( ( NULL == prmCntx ) || ( NULL == pageBuff ) || ( NULL == prmPage ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        /* Check if initialized */
        if( false == prmCntx->isInitializedParams )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            returnVal = setPagePrmInRamBuffNCrcUp(prmCntx, pageBuff, prmPage);
            if( PARAMRES_ALLOK == returnVal )
            {
                /* Erase the pageBuff */
                if( false == (*(prmCntx->pToEraseFunc))(prmCntx->pageId, pageOffset, prmCntx->pageSize) )
                {
                    returnVal = PARAMRES_ERRORREAD;
                }
                else
                {
                    /* Write a pageBuff in memory */
                    if( false == (*(prmCntx->pToWriteFunc))(prmCntx->pageId, pageOffset, prmCntx->pageSize, pageBuff) )
                    {
                        returnVal = PARAMRES_ERRORREAD;
                    }
                    else
                    {
                        returnVal = PARAMRES_ALLOK;
                    }
                }
            }
            returnVal = PARAMRES_ALLOK;
        }
    }

    return returnVal;
}


e_paramOrgResult readPageNPrm(s_paramOrgContext* prmCntx, const uint32_t pageOffset, uint8_t* pageBuff, s_prv_paramOrgPageParam* prmPage)
{
    /* Local variable */
    e_paramOrgResult returnVal;
    uint32_t pageCrcSizeToCalc;
    uint32_t crcCalculated;

    /* Check for NULL pointer */
    if( ( NULL == prmCntx ) || ( NULL == pageBuff ) || ( NULL == prmPage ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        /* Check if initialized */
        if( false == prmCntx->isInitializedParams )
        {
            returnVal = PARAMRES_NOT_INIT;
        }
        else
        {
            /* Get pageBuff */
            if( false == (*(prmCntx->pToReadFunc))(prmCntx->pageId, pageOffset, prmCntx->pageSize, pageBuff) )
            {
                returnVal = PARAMRES_ERRORREAD;
            }
            else
            {
                /* now verify */
                returnVal = getPagePrmFromRamBuff(prmCntx, pageBuff, prmPage);
            }
        }
    }

    return returnVal;
}