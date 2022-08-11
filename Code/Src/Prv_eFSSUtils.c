/**
 * @file Prv_eFSSUtils.c
 *
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "Prv_eFSSUtils.h"
#include "Prv_eFSSUtilsLL.h"


/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_Res getPagePrmFromBuff(const s_eFSS_PgInfo pginfo, const uint8_t* pageBuff, s_prv_pagePrm* const pagePrm)
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
        if( ( pginfo.pageSize < EFSS_MIN_PAGE_SIZE_BYTE ) || ( pginfo.pageSize > EFSS_MAX_PAGE_SIZE_BYTE ) )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Calculating offset */
            uint32_t offset1 = pginfo.pageSize - ( sizeof(uint64_t) + sizeof(uint64_t) );
            uint32_t offset2 = pginfo.pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) + sizeof(uint16_t) );
            uint32_t offset3 = pginfo.pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) );
            uint32_t offset4 = pginfo.pageSize - ( sizeof(uint32_t) + sizeof(uint32_t) );
            uint32_t offset5 = pginfo.pageSize - ( sizeof(uint32_t) );

            /* fill the parameter using the passed buffer */
            (void)memcpy( (uint8_t*)&pagePrm->pageType,               &pageBuff[offset1], sizeof(uint16_t) );
            (void)memcpy( (uint8_t*)&pagePrm->allPageAlignmentNumber, &pageBuff[offset2], sizeof(uint16_t) );
            (void)memcpy( (uint8_t*)&pagePrm->pageVersion,            &pageBuff[offset3], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&pagePrm->pageMagicNumber,        &pageBuff[offset4], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&pagePrm->pageCrc,                &pageBuff[offset5], sizeof(uint32_t) );
            returnVal = EFSS_RES_OK;
        }
    }
    return returnVal;
}

e_eFSS_Res setPagePrmInBuff(const s_eFSS_PgInfo pginfo, uint8_t* const pageBuff, const s_prv_pagePrm* pagePrm)
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
        if( ( pginfo.pageSize < EFSS_MIN_PAGE_SIZE_BYTE ) || ( pginfo.pageSize > EFSS_MAX_PAGE_SIZE_BYTE ) )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Calculating offset */
            uint32_t offset1 = pginfo.pageSize - ( sizeof(uint64_t) + sizeof(uint64_t) );
            uint32_t offset2 = pginfo.pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) + sizeof(uint16_t) );
            uint32_t offset3 = pginfo.pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) );
            uint32_t offset4 = pginfo.pageSize - ( sizeof(uint32_t) + sizeof(uint32_t) );
            uint32_t offset5 = pginfo.pageSize - ( sizeof(uint32_t) );

            /* fill the paramebuffer using the passed parameter */
            (void)memcpy( &pageBuff[offset1], (const uint8_t*)&pagePrm->pageType,                 sizeof(uint16_t) );
            (void)memcpy( &pageBuff[offset2], (const uint8_t*)&pagePrm->allPageAlignmentNumber,   sizeof(uint16_t) );
            (void)memcpy( &pageBuff[offset3], (const uint8_t*)&pagePrm->pageVersion,              sizeof(uint32_t) );
            (void)memcpy( &pageBuff[offset4], (const uint8_t*)&pagePrm->pageMagicNumber,          sizeof(uint32_t) );
            (void)memcpy( &pageBuff[offset5], (const uint8_t*)&pagePrm->pageCrc,                  sizeof(uint32_t) );

            returnVal = EFSS_RES_OK;
        }
    }

    return returnVal;
}

e_eFSS_Res setCrcInPagePrmBuff(const s_eFSS_PgInfo pginfo, uint8_t* const pageBuff, const uint32_t crcToSet)
{
    /* Local variable */
    e_eFSS_Res returnVal;

    /* Check for NULL pointer */
    if( NULL == pageBuff )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check for parameter validity */
        if( ( pginfo.pageSize < EFSS_MIN_PAGE_SIZE_BYTE ) || ( pginfo.pageSize > EFSS_MAX_PAGE_SIZE_BYTE ) )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Calculating offset */
            uint32_t offset5 = pginfo.pageSize - ( sizeof(uint32_t) );

            /* Copy CRC passed in to the ram buffer */
            (void)memcpy( &pageBuff[offset5], (const uint8_t*)&crcToSet, sizeof(uint32_t) );
            returnVal = EFSS_RES_OK;
        }
    }

    return returnVal;
}

e_eFSS_Res calcPagePrmCrcInBuff(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, const uint8_t* pageBuff,
                                uint32_t* const crcCalc)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t pageCrcSizeToCalc;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff ) || ( NULL == crcCalc ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check for parameter validity */
        if( ( pginfo.pageSize < EFSS_MIN_PAGE_SIZE_BYTE ) || ( pginfo.pageSize > EFSS_MAX_PAGE_SIZE_BYTE ) )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Esclude from the calculation the CRC, that is placed in the last 4 byte */
            pageCrcSizeToCalc = pginfo.pageSize - sizeof(uint32_t);

            /* Calculate CRC */
            returnVal = calcCrcLL(cbHld, crcCalc, pageBuff, pageCrcSizeToCalc);
        }
    }

    return returnVal;
}

e_eFSS_Res setPagePrmInBuffNCrcUp(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, uint8_t* const pageBuff,
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
        returnVal = setPagePrmInBuff(pginfo, pageBuff, pagePrm);

        if( EFSS_RES_OK == returnVal)
        {
            /* Calc crc value of the page */
            returnVal = calcPagePrmCrcInBuff(pginfo, cbHld, pageBuff, &crcCalc);

            if( EFSS_RES_OK == returnVal)
            {
                /* Crc calculated, set it */
                returnVal = setCrcInPagePrmBuff(pginfo, pageBuff, crcCalc);
            }
        }
    }

    return returnVal;
}

e_eFSS_Res isValidPageInBuff(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, const uint8_t* pageBuff)
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
        returnVal = getPagePrmFromBuff(pginfo, pageBuff, &prmPage);

        if( EFSS_RES_OK == returnVal )
        {
            /* Calculate the pageBuff param CRC */
            returnVal = calcPagePrmCrcInBuff(pginfo, cbHld, pageBuff, &crcCalc);

            if( EFSS_RES_OK == returnVal )
            {
                /* Verify if calculated CRC is equal to the stored one, check if magic number is equal to the
                * setted one also */
                if( (crcCalc == prmPage.pageCrc) && ( PARAM_32_MAGIC_NUMBER == prmPage.pageMagicNumber ) )
                {
                    /* Verify if the pageBuff type is what we are aspecting */
                    if( prmPage.pageType == ( (uint16_t)pginfo.pageType ) )
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

e_eFSS_Res isValidPage(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, uint8_t* const suppBuff,
                       const uint32_t pageIndx)
{
    /* Local variable */
    e_eFSS_Res returnVal;

    /* Check for NULL pointer */
    if( NULL == suppBuff )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check for parameter validity */
        if( pageIndx >= pginfo.nOfPages )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Get the page to check in the support ram buffer */
            if( EFSS_RES_OK == readPageLL( pginfo, cbHld, pageIndx, suppBuff ) )
            {
                /* now verify the page loaded in the buffer */
                returnVal = isValidPageInBuff(pginfo, cbHld, suppBuff);
            }
        }
    }

    return returnVal;
}

e_eFSS_Res writePageNPrmNUpdateCrc(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, uint8_t* const pageBuff,
                                   uint8_t* const suppBuff, const uint32_t pageIndx, const s_prv_pagePrm* prmPage)
{
    /* Local variable */
    e_eFSS_Res returnVal;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff ) || ( NULL == prmPage ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check for parameter validity */
        if( pageIndx >= pginfo.nOfPages )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Set the page param and CRC in the page buffer */
            returnVal = setPagePrmInBuffNCrcUp(pginfo, cbHld, pageBuff, prmPage);
            if( EFSS_RES_OK == returnVal )
            {
                /* Erase physical page */
                returnVal = erasePageLL(pginfo, cbHld, pageIndx);
                if( EFSS_RES_OK == returnVal )
                {
                    /* Write the pageBuff in the physical page */
                    returnVal = writePageLL(pginfo, cbHld, pageIndx, pageBuff, suppBuff );
                }
            }
        }
    }

    return returnVal;
}

e_eFSS_Res writeNPageNPrmNUpdateCrc(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, uint8_t* const pageBuff,
                                    uint8_t* const suppBuff, const uint32_t nPageToWrite, const uint32_t startPageIndx,
                                    const s_prv_pagePrm* prmPage)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t iterator;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff )|| ( NULL == suppBuff ) || ( NULL == prmPage ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check for parameter validity */
        if( ( 0u == nPageToWrite ) || ( startPageIndx >= pginfo.nOfPages ) ||
          ( ( startPageIndx + nPageToWrite - 1u ) >= pginfo.nOfPages ) )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Set the page param and CRC in the page buffer */
            returnVal = setPagePrmInBuffNCrcUp(pginfo, cbHld, pageBuff, prmPage);
            if( EFSS_RES_OK == returnVal )
            {
                iterator = 0u;
                while( ( iterator < nPageToWrite ) && ( EFSS_RES_OK == returnVal ) )
                {
                    /* Erase physical page */
                    returnVal = erasePageLL(pginfo, cbHld, ( startPageIndx + iterator ) );
                    if( EFSS_RES_OK == returnVal )
                    {
                        /* Write the pageBuff in the physical page */
                        returnVal = writePageLL(pginfo, cbHld, ( startPageIndx + iterator ), pageBuff, suppBuff );
                    }

                    iterator++;
                }
            }
        }
    }

    return returnVal;
}

e_eFSS_Res readPageNPrm(const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cbHld, uint8_t* const pageBuff,
                        const uint32_t pageIndx, s_prv_pagePrm* const pagePrm)
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
        if( pageIndx >= pginfo.nOfPages )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Get pageBuff */
            returnVal = readPageLL( pginfo, cbHld, pageIndx, pageBuff );

            if( EFSS_RES_OK == returnVal )
            {
                /* Fill even param only for comodity */
                returnVal = getPagePrmFromBuff(pginfo, pageBuff, pagePrm);
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