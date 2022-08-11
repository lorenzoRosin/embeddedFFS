/**
 * @file Prv_eFSSRaw.c
 *
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "Prv_eFSSRaw.h"



/***********************************************************************************************************************
 *   PRIVATE FUNCTIONS DECLARATION
 **********************************************************************************************************************/
static e_eFSS_Res integrityCreatorRawWithBkp(const s_eFSS_Ctx* prmCntx);
static e_eFSS_Res integrityCreatorRawNoBkp(const s_eFSS_Ctx* prmCntx);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_Res initParamMemoryRaw(const s_eFSS_Ctx* prmCntx)
{
    /* Local variable */
    e_eFSS_Res returnVal;

    /* Check for NULL pointer */
    if( NULL == prmCntx )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        if( true == prmCntx->pageInfo.pageBkp )
        {
            /* Back up page init */
            returnVal = integrityCreatorRawWithBkp(prmCntx);
        }
        else
        {
            /* Init without backup page */
            returnVal = integrityCreatorRawNoBkp(prmCntx);
        }
    }
    return returnVal;
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_Res integrityCreatorRawNoBkp(const s_eFSS_Ctx* prmCntx)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t iterator;
    uint32_t nonValidPageCounter;
    uint32_t timeWritedPageFound;
    uint8_t aligmenentNumberFound;
    bool_t allAlignmentAreok;
    s_prv_pagePrm pagePrm;

    /* Check for NULL pointer */
    if( NULL == prmCntx )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check if it is first init: all page are corrupted  */
        /* Check if it is all ok: No corruption and all pages has same allPageAlignmentNumber */
        /* Check if there is something corrupted: all ok but some pages has different allPageAlignmentNumber, or
         *                                        some pages are invalid */
        iterator = 0u;
        nonValidPageCounter = 0u;
        aligmenentNumberFound = 0u;
        returnVal = EFSS_RES_OK;
        allAlignmentAreok = true;

        while( ( iterator < prmCntx->pageInfo.nOfPages ) && ( EFSS_RES_OK == returnVal ) )
        {
            returnVal = isValidPage( prmCntx->pageInfo, prmCntx->cbHolder, prmCntx->memPoolPointer, iterator);

            if( EFSS_RES_NOTVALIDPAGE == returnVal)
            {
                nonValidPageCounter++;
                returnVal = EFSS_RES_OK;
            }
            else if( EFSS_RES_OK == returnVal )
            {
                /* Page is Ok, check if alignment is still ok*/
                if( ( true == allAlignmentAreok ) && ( 0u == nonValidPageCounter ) )
                {
                    returnVal = getPagePrmFromBuff( prmCntx->pageInfo, prmCntx->memPoolPointer, &pagePrm);

                    if( EFSS_RES_OK == returnVal )
                    {
                        /* Save first alignment number */
                        if( 0u == iterator )
                        {
                            aligmenentNumberFound = pagePrm.allPageAlignmentNumber;
                        }

                        /* Check if alignment number are always equals */
                        if(pagePrm.allPageAlignmentNumber != aligmenentNumberFound)
                        {
                            allAlignmentAreok = false;
                        }
                    }
                }
            }

            iterator++;
        }

        /* Information parsing procedure ended, analyze */
        if( EFSS_RES_OK == returnVal)
        {
            /* no major error found */
            if( ( prmCntx->pageInfo.nOfPages == nonValidPageCounter ) || ( 0u != nonValidPageCounter ) ||
                ( false == allAlignmentAreok ) )
            {
                /* Set all memory to zero */
                memset(prmCntx->memPoolPointer, 0u, prmCntx->pageInfo.pageSize);

                /* Set page parameter */
                pagePrm.pageType = EFSS_PAGETYPE_RAW;
                pagePrm.allPageAlignmentNumber = 1u;
                pagePrm.pageVersion = prmCntx->pageLogVer.rawPageVersion;
                pagePrm.pageMagicNumber = PARAM_32_MAGIC_NUMBER;

                /* Write all pages */
                returnVal =  writeNPageNPrmNUpdateCrc(prmCntx->pageInfo, prmCntx->cbHolder, prmCntx->memPoolPointer, prmCntx->memPoolPointer,
                                                      prmCntx->pageInfo.nOfPages, 0u, &pagePrm);
                if( EFSS_RES_OK == returnVal )
                {
                    if( prmCntx->pageInfo.nOfPages == nonValidPageCounter )
                    {
                        /* All pages are corrupted, this is first init  */
                        returnVal = EFSS_RES_OK_FIRSTINIT;
                    }
                    else if( 0u != nonValidPageCounter )
                    {
                        /* Some pages are corrupted */
                        returnVal = EFSS_RES_OK_BUTNOTFIRSTINIT;
                    }
                    else if( false == allAlignmentAreok )
                    {
                        /* Alignment number not aligned */
                        returnVal = EFSS_RES_OK_BUTNOTFIRSTINIT;
                    }
                }
            }
        }
    }

    return returnVal;
}





e_eFSS_Res integrityCreatorRawWithBkp(const s_eFSS_Ctx* prmCntx)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t iterator;
    uint32_t nonValidPageCounter;
    uint32_t timeWritedPageFound;
    uint8_t aligmenentNumberFound;
    bool_t allAlignmentAreok;
    s_prv_pagePrm pagePrm;

    /* Check for NULL pointer */
    if( NULL == prmCntx )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check if it is first init: all page are corrupted  */
        /* Check if it is all ok: No corruption and all pages has same allPageAlignmentNumber */
        /* Check if there is something corrupted: all ok but some pages has different allPageAlignmentNumber, or
         *                                        some pages are invalid */
        iterator = 0u;
        nonValidPageCounter = 0u;
        aligmenentNumberFound = 0u;
        returnVal = EFSS_RES_OK;
        allAlignmentAreok = true;

        while( ( iterator < prmCntx->pageInfo.nOfPages ) && ( EFSS_RES_OK == returnVal ) )
        {
            returnVal = isValidPage( prmCntx->pageInfo, prmCntx->cbHolder, prmCntx->memPoolPointer, iterator);

            if( EFSS_RES_NOTVALIDPAGE == returnVal)
            {
                nonValidPageCounter++;
                returnVal = EFSS_RES_OK;
            }
            else if( EFSS_RES_OK == returnVal )
            {
                /* Page is Ok, check if alignment is still ok*/
                if( ( true == allAlignmentAreok ) && ( 0u == nonValidPageCounter ) )
                {
                    returnVal = getPagePrmFromBuff( prmCntx->pageInfo, prmCntx->memPoolPointer, &pagePrm);
                    if( EFSS_RES_OK == returnVal )
                    {
                        /* Save first alignment number */
                        if( 0u == iterator )
                        {
                            aligmenentNumberFound = pagePrm.allPageAlignmentNumber;
                        }

                        /* Check if alignment number are always equals */
                        if(pagePrm.allPageAlignmentNumber != aligmenentNumberFound)
                        {
                            allAlignmentAreok = false;
                        }
                    }
                }
            }

            iterator++;
        }

        /* Information parsing procedure ended, analyze */
        if( EFSS_RES_OK == returnVal)
        {
            /* no major error found */
            if( ( prmCntx->pageInfo.nOfPages == nonValidPageCounter ) || ( 0u != nonValidPageCounter ) ||
                ( false == allAlignmentAreok ) )
            {
                /* Set all memory to zero */
                memset(prmCntx->memPoolPointer, 0u, prmCntx->pageInfo.pageSize);

                /* Set page parameter */
                pagePrm.pageType = EFSS_PAGETYPE_RAW;
                pagePrm.allPageAlignmentNumber = 1u;
                pagePrm.pageVersion = prmCntx->pageLogVer.rawPageVersion;
                pagePrm.pageMagicNumber = PARAM_32_MAGIC_NUMBER;

                /* Write all pages */
                returnVal =  writeNPageNPrmNUpdateCrc(prmCntx->pageInfo, prmCntx->cbHolder, prmCntx->memPoolPointer, prmCntx->memPoolPointer,
                                                      prmCntx->pageInfo.nOfPages, 0u, &pagePrm);
                if( EFSS_RES_OK == returnVal )
                {
                    if( prmCntx->pageInfo.nOfPages == nonValidPageCounter )
                    {
                        /* All pages are corrupted, this is first init  */
                        returnVal = EFSS_RES_OK_FIRSTINIT;
                    }
                    else if( 0u != nonValidPageCounter )
                    {
                        /* Some pages are corrupted */
                        returnVal = EFSS_RES_OK_BUTNOTFIRSTINIT;
                    }
                    else if( false == allAlignmentAreok )
                    {
                        /* Alignment number not aligned */
                        returnVal = EFSS_RES_OK_BUTNOTFIRSTINIT;
                    }
                }
            }
        }
    }

    return returnVal;
}