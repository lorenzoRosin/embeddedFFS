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
static e_eFSS_Res checkMemStatRawWithBkp(const s_eFSS_Ctx* prmCntx);
static e_eFSS_Res checkMemStatRawNoBkp(const s_eFSS_Ctx* prmCntx);



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_Res checkMemStatRaw(const s_eFSS_Ctx* prmCntx)
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
            returnVal = checkMemStatRawWithBkp(prmCntx);
        }
        else
        {
            /* Init without backup page */
            returnVal = checkMemStatRawNoBkp(prmCntx);
        }
    }
    return returnVal;
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_Res checkMemStatRawNoBkp(const s_eFSS_Ctx* prmCntx)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t iterator;
    uint32_t nonValidPageCounter;
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
        /* Check for parameter validity */
        if( ( 0u == prmCntx->pageInfo.nOfPages ) || ( EFSS_PAGETYPE_RAW != prmCntx->pageInfo.pageType ) ||
            ( NULL == memPoolPointer ) )
        {
            returnVal = EFSS_RES_BADPARAM;
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
                    /* Page is Ok, check if alignment is still ok */
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
                if( prmCntx->pageInfo.nOfPages == nonValidPageCounter )
                {
                    /* First init because not valid page found */
                    returnVal = EFSS_RES_NOTINITMEM;
                }
                else if( 0u != nonValidPageCounter )
                {
                    /* Some pages are corrupted */
                    returnVal = EFSS_RES_CORRUPTMEM;
                }
                else if( false == allAlignmentAreok )
                {
                    /* No pages corruption but alignment is not even, some corruption happened */
                    returnVal = EFSS_RES_CORRUPTMEM;
                }
                else
                {
                    /* All pages are ok, no curruption etc check pages version and type  */
                    if( EFSS_PAGETYPE_RAW != pagePrm.pageType )
                    {
                        returnVal = EFSS_RES_CORRUPTMEM;
                    }
                    else if( prmCntx->pageLogVer.rawPageVersion != pagePrm.pageVersion )
                    {
                        returnVal = EFSS_RES_NEWVERFOUND;
                    }
                }
            }
        }
    }

    return returnVal;
}





e_eFSS_Res checkMemStatRawWithBkp(const s_eFSS_Ctx* prmCntx)
{
    /* Local variable */
    e_eFSS_Res returnVal;
    uint32_t iterator;
    uint32_t nonValidPageCounter;
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
        /* Check for parameter validity */
        if( ( 0u == prmCntx->pageInfo.nOfPages ) || ( EFSS_PAGETYPE_RAW != prmCntx->pageInfo.pageType ) ||
            ( NULL == memPoolPointer ) )
        {
            returnVal = EFSS_RES_BADPARAM;
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
                    /* Page is Ok, check if alignment is still ok */
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
                if( prmCntx->pageInfo.nOfPages == nonValidPageCounter )
                {
                    /* First init because not valid page found */
                    returnVal = EFSS_RES_NOTINITMEM;
                }
                else if( 0u != nonValidPageCounter )
                {
                    /* Some pages are corrupted */
                    returnVal = EFSS_RES_CORRUPTMEM;
                }
                else if( false == allAlignmentAreok )
                {
                    /* No pages corruption but alignment is not even, some corruption happened */
                    returnVal = EFSS_RES_CORRUPTMEM;
                }
                else
                {
                    /* All pages are ok, no curruption etc check pages version and type  */
                    if( EFSS_PAGETYPE_RAW != pagePrm.pageType )
                    {
                        returnVal = EFSS_RES_CORRUPTMEM;
                    }
                    else if( prmCntx->pageLogVer.rawPageVersion != pagePrm.pageVersion )
                    {
                        returnVal = EFSS_RES_NEWVERFOUND;
                    }
                }
            }
        }
    }

    return returnVal;
}