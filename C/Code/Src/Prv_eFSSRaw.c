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
            ( NULL == prmCntx->memPoolPointer ) )
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
                    /* Page is Ok, where previous page ok? */
                    if( 0u != nonValidPageCounter )
                    {
                        /* Previous page where invalid, this page is valid, we dont need to proceeed: this is not
                         * the first init, and we have some corruption */
                        returnVal = EFSS_RES_CORRUPTMEM;
                    }
                    else
                    {
                        /* Page till now were correct, check param validity */
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
                                /* Page are not aligned, we have mem corruption */
                                returnVal = EFSS_RES_CORRUPTMEM;
                            }
                            else
                            {
                                /* Check every page validity */
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

                iterator++;
            }

            /* Information parsing procedure ended */
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
    uint32_t rcvredBkupPageCounter;
    uint8_t aligmenentNumberFound;
    s_prv_pagePrm pagePrm;

    /* Local variable offset */
    uint32_t backUpOffset;
    uint8_t* buff1;
    uint8_t* buff2;

    /* Check for NULL pointer */
    if( NULL == prmCntx )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check for parameter validity */
        if( ( prmCntx->pageInfo.nOfPages < 2u ) || ( EFSS_PAGETYPE_RAW != prmCntx->pageInfo.pageType ) ||
            ( NULL == prmCntx->memPoolPointer ) || ( 0 != ( prmCntx->pageInfo.nOfPages % 2u ) ) )
        {
            returnVal = EFSS_RES_BADPARAM;
        }
        else
        {
            /* Check if it is first init: all page are corrupted  */
            /* Check if it is all ok: No corruption and all pages has same allPageAlignmentNumber */
            /* Check if there is something corrupted: all ok but some pages has different allPageAlignmentNumber, or
            *                                        some pages are invalid */
            
            /* Init variable */
            iterator = 0u;
            nonValidPageCounter = 0u;
            aligmenentNumberFound = 0u;
            rcvredBkupPageCounter = 0u;
            returnVal = EFSS_RES_OK;

            /* Init offset */
            backUpOffset = prmCntx->pageInfo.nOfPages / 2u;
            buff1 = &prmCntx->memPoolPointer[0u];
            buff2 = &prmCntx->memPoolPointer[prmCntx->pageInfo.pageSize];

            while( ( iterator < prmCntx->pageInfo.nOfPages ) && ( EFSS_RES_OK == returnVal ) )
            {
                returnVal = verifyAndRipristinateBkup( prmCntx->pageInfo, prmCntx->cbHolder, buff1,
                                                       buff2, iterator, ( iterator + backUpOffset) );

                if( EFSS_RES_NOTVALIDPAGE == returnVal)
                {
                    /* Both origin and backup are invalid */
                    nonValidPageCounter++;
                    returnVal = EFSS_RES_OK;
                }
                else if( ( EFSS_RES_OK == returnVal ) || (EFSS_RES_OKBKPRCVRD == returnVal ) )
                {
                    if( (EFSS_RES_OKBKPRCVRD == returnVal )
                    {
                        /* Recovered a page, but it's all ok */
                        rcvredBkupPageCounter++;
                        returnVal = EFSS_RES_OK;
                    }

                    /* Page is Ok, where previous page ok? */
                    if( 0u != nonValidPageCounter )
                    {
                        /* Previous page where invalid, this page is valid, we dont need to proceeed: this is not
                         * the first init, and we have some corruption */
                        returnVal = EFSS_RES_CORRUPTMEM;
                    }
                    else
                    {
                        returnVal = readPageNPrm(prmCntx->pageInfo, prmCntx->cbHolder, buff1, iterator, &pagePrm);

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
                                /* Page are not aligned, we have mem corruption */
                                returnVal = EFSS_RES_CORRUPTMEM;
                            }
                            else
                            {
                                /* Check every page validity */
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

                iterator++;
            }

            /* Information parsing procedure ended */
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
            }
        }
    }

    return returnVal;
}