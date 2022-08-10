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
e_eFSS_Res getPagePrmFromBuff(const uint32_t pageSize, const uint8_t* pageBuff, s_prv_pagePrm* const prmPage)
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
        if( pageSize < EFSS_MIN_PAGE_SIZE_BYTE )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Copy data in ram buffer in to the structure */
            uint32_t offset1 = pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) + sizeof(uint32_t) );
            uint32_t offset2 = pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) );
            uint32_t offset3 = pageSize - ( sizeof(uint64_t) + sizeof(uint16_t) + sizeof(uint8_t) );
            uint32_t offset4 = pageSize - ( sizeof(uint64_t) + sizeof(uint16_t) );
            uint32_t offset5 = pageSize - ( sizeof(uint64_t) );
            uint32_t offset6 = pageSize - ( sizeof(uint32_t) );

            (void)memcpy( (uint8_t*)&prmPage->pageTimeSetted,         &pageBuff[offset1], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&prmPage->pageType,               &pageBuff[offset2], sizeof(uint8_t)  );
            (void)memcpy( (uint8_t*)&prmPage->allPageAlignmentNumber, &pageBuff[offset3], sizeof(uint8_t)  );
            (void)memcpy( (uint8_t*)&prmPage->pageVersion,            &pageBuff[offset4], sizeof(uint16_t) );
            (void)memcpy( (uint8_t*)&prmPage->pageMagicNumber,        &pageBuff[offset5], sizeof(uint32_t) );
            (void)memcpy( (uint8_t*)&prmPage->pageCrc,                &pageBuff[offset6], sizeof(uint32_t) );
            returnVal = EFSS_RES_OK;
        }
    }
    return returnVal;
}

e_eFSS_Res setPagePrmInBuff(uint32_t pageSize, uint8_t* const pageBuff, const s_prv_pagePrm* prmPage)
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
        if( pageSize < EFSS_MIN_PAGE_SIZE_BYTE )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Copy data in structure in to the ram buffer */
            uint32_t offset1 = pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) + sizeof(uint32_t) );
            uint32_t offset2 = pageSize - ( sizeof(uint64_t) + sizeof(uint32_t) );
            uint32_t offset3 = pageSize - ( sizeof(uint64_t) + sizeof(uint16_t) + sizeof(uint8_t) );
            uint32_t offset4 = pageSize - ( sizeof(uint64_t) + sizeof(uint16_t) );
            uint32_t offset5 = pageSize - ( sizeof(uint64_t) );
            uint32_t offset6 = pageSize - ( sizeof(uint32_t) );

            (void)memcpy( &pageBuff[offset1], (uint8_t*)&prmPage->pageTimeSetted,           sizeof(uint32_t) );
            (void)memcpy( &pageBuff[offset2], (uint8_t*)&prmPage->pageType,                 sizeof(uint8_t)  );
            (void)memcpy( &pageBuff[offset3], (uint8_t*)&prmPage->allPageAlignmentNumber,   sizeof(uint8_t)  );
            (void)memcpy( &pageBuff[offset4], (uint8_t*)&prmPage->pageVersion,              sizeof(uint16_t) );
            (void)memcpy( &pageBuff[offset5], (uint8_t*)&prmPage->pageMagicNumber,          sizeof(uint32_t) );
            (void)memcpy( &pageBuff[offset6], (uint8_t*)&prmPage->pageCrc,                  sizeof(uint32_t) );

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
        /* Copy CRC passed in to the ram buffer */
        uint32_t offset6 = pageSize - ( sizeof(uint32_t) );
        (void)memcpy( &pageBuff[offset6], (uint8_t*)&crcToSet, sizeof(uint32_t) );
        returnVal = EFSS_RES_OK;
    }

    return returnVal;
}


e_eFSS_Res calcPagePrmCrcInBuff(const uint32_t pageSize, const uint8_t* pageBuff, const s_eFSS_Cb cbHld, uint32_t* const crcCalc)
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
        /* Esclude from the calculation the CRC, that is placed in the last 4 byte */
        pageCrcSizeToCalc = pageSize - sizeof(uint32_t);
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

    return returnVal;
}


e_eFSS_Res setPagePrmInBuffNCrcUp(uint32_t pageSize, uint8_t* pageBuff, s_eFSS_Cb cbHld, s_prv_pagePrm* prmPage)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t pageCrcSizeToCalc;
    uint32_t crcCalc;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff ) || ( NULL == prmPage ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Set the parameter in the ram buffer */
        returnVal = setPagePrmInBuff(pageSize, pageBuff, prmPage);

        if( EFSS_RES_OK == returnVal)
        {
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



e_eFSS_Res isValidPageInBuff(uint32_t pageSize,  uint8_t* pageBuff, s_eFSS_Cb cbHld, e_eFSS_PageType pageType)
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

e_eFSS_Res isValidPage(uint32_t pageSize, uint8_t* pageBuff, const uint32_t pageId, const uint32_t pageOffset, s_eFSS_Cb cbHld, e_eFSS_PageType pageType)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    s_prv_pagePrm prmPage;
    uint32_t crcCalc;

    /* Get pageBuff */
    if( false == (*(cbHld.pReadPg))(pageId, pageOffset, pageSize, pageBuff) )
    {
        returnVal = EFSS_RES_ERRORREAD;
    }
    else
    {
        /* now verify */
        returnVal = isValidPageInBuff(pageSize, pageBuff, cbHld, pageType);
    }

    return returnVal;
}

e_eFSS_Res writePageNPrmNUpdateCrc(uint32_t pageSize, uint8_t* pageBuff, const uint32_t pageId, const uint32_t pageOffset, s_prv_pagePrm* prmPage, s_eFSS_Cb cbHld)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t pageCrcSizeToCalc;
    uint32_t crcCalc;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff ) || ( NULL == prmPage ) )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        returnVal = setPagePrmInBuffNCrcUp(pageSize, pageBuff, cbHld, prmPage);
        if( EFSS_RES_OK == returnVal )
        {
            /* Erase the pageBuff */
            if( false == (*(cbHld.pErasePg))(pageId, pageOffset, pageSize) )
            {
                returnVal = EFSS_RES_ERRORREAD;
            }
            else
            {
                /* Write a pageBuff in memory */
                if( false == (*(cbHld.pWritePg))(pageId, pageOffset, pageSize, pageBuff) )
                {
                    returnVal = EFSS_RES_ERRORREAD;
                }
                else
                {
                    returnVal = EFSS_RES_OK;
                }
            }
        }
        returnVal = EFSS_RES_OK;
    }

    return returnVal;
}


e_eFSS_Res readPageNPrm(uint32_t pageSize, uint8_t* pageBuff, const uint32_t pageId, s_eFSS_Ctx* prmCntx, const uint32_t pageOffset, s_eFSS_Cb cbHld, s_prv_pagePrm* prmPage)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t pageCrcSizeToCalc;
    uint32_t crcCalc;

    /* Check for NULL pointer */
    if( ( NULL == pageBuff ) || ( NULL == prmPage ) )
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
            /* now verify */
            returnVal = getPagePrmFromBuff(pageSize, pageBuff, prmPage);
        }

    }

    return returnVal;
}