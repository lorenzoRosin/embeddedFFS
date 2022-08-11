/**
 * @file Prv_eFSSUtils.c
 *
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "Prv_eFSSUtils.h"



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_Res getPagePrmFromBuff(const uint32_t pageSize, const uint8_t* pageBuff, s_prv_pagePrm* const pagePrm)
{
    /* Local variable */
    e_eFSS_Res returnVal;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff ) || ( NULL == pagePrm ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check for parameter validity */
        if( ( pageSize < EFSS_MIN_PAGE_SIZE_BYTE ) || ( pageSize > EFSS_MAX_PAGE_SIZE_BYTE ) )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Calculating offset */
            uint32_t offset1 = pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) + sizeof(uint32_t) );
            uint32_t offset2 = pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) );
            uint32_t offset3 = pageSize - ( sizeof(uint64_t) + sizeof(uint16_t) + sizeof(uint8_t) );
            uint32_t offset4 = pageSize - ( sizeof(uint64_t) + sizeof(uint16_t) );
            uint32_t offset5 = pageSize - ( sizeof(uint64_t) );
            uint32_t offset6 = pageSize - ( sizeof(uint32_t) );

            /* fill the parameter using the passed buffer */
            (void)memcpy( (uint8_t*)&pagePrm->pageType,               &pageBuff[offset2], sizeof(uint8_t)  );
            (void)memcpy( (uint8_t*)&pagePrm->allPageAlignmentNumber, &pageBuff[offset3], sizeof(uint8_t)  );
            (void)memcpy( (uint8_t*)&pagePrm->pageVersion,            &pageBuff[offset4], sizeof(uint16_t) );
            (void)memcpy( (uint8_t*)&pagePrm->pageMagicNumber,        &pageBuff[offset5], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&pagePrm->pageCrc,                &pageBuff[offset6], sizeof(uint32_t) );
            returnVal = EFSS_RES_OK;
        }
    }
    return returnVal;
}

e_eFSS_Res setPagePrmInBuff(const uint32_t pageSize, uint8_t* const pageBuff, const s_prv_pagePrm* pagePrm)
{
    /* Local variable */
    e_eFSS_Res returnVal;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff ) || ( NULL == pagePrm ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check for parameter validity */
        if( ( pageSize < EFSS_MIN_PAGE_SIZE_BYTE ) || ( pageSize > EFSS_MAX_PAGE_SIZE_BYTE ) )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Calculating offset */
            uint32_t offset1 = pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) + sizeof(uint32_t) );
            uint32_t offset2 = pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) );
            uint32_t offset3 = pageSize - ( sizeof(uint64_t) + sizeof(uint16_t) + sizeof(uint8_t) );
            uint32_t offset4 = pageSize - ( sizeof(uint64_t) + sizeof(uint16_t) );
            uint32_t offset5 = pageSize - ( sizeof(uint64_t) );
            uint32_t offset6 = pageSize - ( sizeof(uint32_t) );

            /* fill the paramebuffer using the passed parameter */
            (void)memcpy( &pageBuff[offset2], (const uint8_t*)&pagePrm->pageType,                 sizeof(uint8_t)  );
            (void)memcpy( &pageBuff[offset3], (const uint8_t*)&pagePrm->allPageAlignmentNumber,   sizeof(uint8_t)  );
            (void)memcpy( &pageBuff[offset4], (const uint8_t*)&pagePrm->pageVersion,              sizeof(uint16_t) );
            (void)memcpy( &pageBuff[offset5], (const uint8_t*)&pagePrm->pageMagicNumber,          sizeof(uint32_t) );
            (void)memcpy( &pageBuff[offset6], (const uint8_t*)&pagePrm->pageCrc,                  sizeof(uint32_t) );

            returnVal = EFSS_RES_OK;
        }
    }

    return returnVal;
}

e_eFSS_Res setCrcInPagePrmBuff(const uint32_t pageSize, uint8_t* const pageBuff, const uint32_t crcToSet)
{
    /* Local variable */
    e_eFSS_Res returnVal;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff ) || ( NULL == crcToSet ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check for parameter validity */
        if( ( pageSize < EFSS_MIN_PAGE_SIZE_BYTE ) || ( pageSize > EFSS_MAX_PAGE_SIZE_BYTE ) )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Calculating offset */
            uint32_t offset6 = pageSize - ( sizeof(uint32_t) );

            /* Copy CRC passed in to the ram buffer */
            (void)memcpy( &pageBuff[offset6], (const uint8_t*)&crcToSet, sizeof(uint32_t) );
            returnVal = EFSS_RES_OK;
        }
    }

    return returnVal;
}


e_eFSS_Res calcPagePrmCrcInBuff(const uint32_t pageSize, const uint8_t* pageBuff, const s_eFSS_Cb cbHld,
                                uint32_t* const crcCalc)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t pageCrcSizeToCalc;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff ) || ( NULL == crcCalc ) || ( NULL == cbHld.pCrc32 ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check for parameter validity */
        if( ( pageSize < EFSS_MIN_PAGE_SIZE_BYTE ) || ( pageSize > EFSS_MAX_PAGE_SIZE_BYTE ) )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Esclude from the calculation the CRC, that is placed in the last 4 byte */
            pageCrcSizeToCalc = pageSize - sizeof(uint32_t);

            /* Calculate CRC */
            if( false == (*(cbHld.pCrc32))(crcCalc, pageBuff, pageCrcSizeToCalc, CRC_BASE_SEED) )
            {
                /* Generic CRC calculation Fail */
                returnVal = EFSS_RES_BADPARAM;
            }
            else
            {
                returnVal = EFSS_RES_OK;
            }
        }
    }

    return returnVal;
}


e_eFSS_Res setPagePrmInBuffNCrcUp(const uint32_t pageSize, uint8_t* const pageBuff, const s_eFSS_Cb cbHld,
                                  const s_prv_pagePrm* pagePrm)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t crcCalc;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff ) || ( NULL == pagePrm ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Set the parameter in the ram buffer */
        returnVal = setPagePrmInBuff(pageSize, pageBuff, pagePrm);

        if( EFSS_RES_OK == returnVal)
        {
            /* Calc crc value of the page */
            returnVal = calcPagePrmCrcInBuff(pageSize, pageBuff, cbHld, &crcCalc);

            if( EFSS_RES_OK == returnVal)
            {
                /* Crc calculated, set it */
                returnVal = setCrcInPagePrmBuff(pageSize, pageBuff, crcCalc);
            }
        }
    }

    return returnVal;
}



e_eFSS_Res isValidPageInBuff(const uint32_t pageSize,  const uint8_t* pageBuff, const s_eFSS_Cb cbHld,
                             const e_eFSS_PageType pageType)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    s_prv_pagePrm prmPage;
    uint32_t crcCalc;

    /* Check for NULL pointer */
    if( NULL == pageBuff )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Get pageBuff param from the ram pageBuff */
        returnVal = getPagePrmFromBuff(pageSize, pageBuff, &prmPage);

        if( EFSS_RES_OK == returnVal )
        {
            /* Calculate the pageBuff param CRC */
            returnVal = calcPagePrmCrcInBuff(pageSize, pageBuff, cbHld, &crcCalc);

            if( EFSS_RES_OK == returnVal )
            {
                /* Verify if calculated CRC is equal to the stored one, check if magic number is equal to the
                * setted one also */
                if( (crcCalc == prmPage.pageCrc) && ( PARAM_32_MAGIC_NUMBER == prmPage.pageMagicNumber ) )
                {
                    /* Verify if the pageBuff type is what we are aspecting */
                    if( prmPage.pageType == ( (uint8_t)pageType ) )
                    {
                        returnVal = EFSS_RES_OK;
                    }
                    else
                    {
                        returnVal = EFSS_RES_NOTVALIDPAGE;
                    }
                }
                else
                {
                    returnVal = EFSS_RES_NOTVALIDPAGE;
                }
            }
        }
    }

    return returnVal;
}

e_eFSS_Res isValidPage( const uint32_t pageSize, uint8_t* const pageBuff, const uint32_t pageId,
                        const uint32_t pageOffset, const s_eFSS_Cb cbHld, const e_eFSS_PageType pageType)
{
    /* Local variable */
    e_eFSS_Res returnVal;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff ) || ( NULL == cbHld.pReadPg ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Get pageBuff */
        if( false == (*(cbHld.pReadPg))(pageId, pageOffset, pageSize, pageBuff) )
        {
            returnVal = EFSS_RES_ERRORREAD;
        }
        else
        {
            /* now verify the page loaded in the buffer */
            returnVal = isValidPageInBuff(pageSize, pageBuff, cbHld, pageType);
        }
    }

    return returnVal;
}

e_eFSS_Res writePageNPrmNUpdateCrc(const uint32_t pageSize, uint8_t* const pageBuff, const uint32_t pageId,
                                   const uint32_t pageOffset, const s_prv_pagePrm* prmPage, const s_eFSS_Cb cbHld)
{
    /* Local variable */
    e_eFSS_Res returnVal;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff ) || ( NULL == prmPage ) || ( NULL == cbHld.pErasePg ) || ( NULL == cbHld.pWritePg ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Set the page param and CRC in the page buffer */
        returnVal = setPagePrmInBuffNCrcUp(pageSize, pageBuff, cbHld, prmPage);
        if( EFSS_RES_OK == returnVal )
        {
            /* Erase physical page */
            if( false == (*(cbHld.pErasePg))(pageId, pageOffset, pageSize) )
            {
                returnVal = EFSS_RES_ERRORERASE;
            }
            else
            {
                /* Write the pageBuff in the physical page */
                if( false == (*(cbHld.pWritePg))(pageId, pageOffset, pageSize, pageBuff) )
                {
                    returnVal = EFSS_RES_ERRORWRITE;
                }
                else
                {
                    returnVal = EFSS_RES_OK;
                }
            }
        }
    }

    return returnVal;
}

e_eFSS_Res writeNPageNPrmNUpdateCrc(const uint32_t pageSize, uint8_t* const pageBuff, const uint32_t pageId,
                                    const uint32_t nOfPageToWrite, const uint32_t startOffset,
                                    const s_prv_pagePrm* prmPage, const s_eFSS_Cb cbHld)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t iterator;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff ) || ( NULL == prmPage ) || ( NULL == cbHld.pErasePg ) || ( NULL == cbHld.pWritePg ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Set the page param and CRC in the page buffer */
        returnVal = setPagePrmInBuffNCrcUp(pageSize, pageBuff, cbHld, prmPage);
        if( EFSS_RES_OK == returnVal )
        {
            iterator = 0;
            while( ( iterator < nOfPageToWrite ) && ( EFSS_RES_OK == returnVal ) )
            {
                /* Erase physical page */
                if( false == (*(cbHld.pErasePg))(pageId, iterator, pageSize) )
                {
                    returnVal = EFSS_RES_ERRORERASE;
                }
                else
                {
                    /* Write the pageBuff in the physical page */
                    if( false == (*(cbHld.pWritePg))(pageId, iterator, pageSize, pageBuff) )
                    {
                        returnVal = EFSS_RES_ERRORWRITE;
                    }
                    else
                    {
                        returnVal = EFSS_RES_OK;
                    }
                }
                iterator++;
            }
        }

    }

    return returnVal;
}

e_eFSS_Res readPageNPrm(const uint32_t pageSize, uint8_t* const pageBuff, const uint32_t pageId,
                        const uint32_t pageOffset, const s_eFSS_Cb cbHld, s_prv_pagePrm* const pagePrm)
{
    /* Local variable */
    e_eFSS_Res returnVal;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff ) || ( NULL == pagePrm ) || ( NULL == cbHld.pReadPg ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Get pageBuff */
        if( false == (*(cbHld.pReadPg))(pageId, pageOffset, pageSize, pageBuff) )
        {
            returnVal = EFSS_RES_ERRORREAD;
        }
        else
        {
            /* Fill even param only for comodity */
            returnVal = getPagePrmFromBuff(pageSize, pageBuff, pagePrm);
        }

    }

    return returnVal;
}

e_eFSS_Res cloneAPage(const uint32_t pageSize, uint8_t* const supportBuff, const uint32_t pageId,
                                    const uint32_t toCloneOffset, const uint32_t destinationOffset,
                                    const s_eFSS_Cb cbHld)
{
    /* Local variable */
    e_eFSS_Res returnVal;

    /* Check for NULL pointer */
    if( ( NULL == supportBuff ) || ( NULL == cbHld.pReadPg ) || ( NULL == cbHld.pErasePg ) || ( NULL == cbHld.pWritePg ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        if( false == (*(cbHld.pReadPg))(pageId, toCloneOffset, pageSize, supportBuff) )
        {
            returnVal = EFSS_RES_ERRORREAD;
        }
        else
        {
            /* Erase physical page */
            if( false == (*(cbHld.pErasePg))(pageId, destinationOffset, pageSize) )
            {
                returnVal = EFSS_RES_ERRORERASE;
            }
            else
            {
                /* Write the pageBuff in the physical page */
                if( false == (*(cbHld.pWritePg))(pageId, destinationOffset, pageSize, supportBuff) )
                {
                    returnVal = EFSS_RES_ERRORWRITE;
                }
                else
                {
                    returnVal = EFSS_RES_OK;
                }
            }
        }
    }

    return returnVal;
}

e_eFSS_Res verifyAndRipristinateBkup(const uint32_t pageSize, uint8_t* const pageBuff, const uint32_t pageId,
                                    const uint32_t origPage, const uint32_t backupPage, const s_eFSS_Cb cbHld)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t iterator;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff ) || ( NULL == cbHld.pReadPg ) || ( NULL == cbHld.pErasePg ) || ( NULL == cbHld.pWritePg ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        returnVal = EFSS_RES_OK;
    }

    return returnVal;
}