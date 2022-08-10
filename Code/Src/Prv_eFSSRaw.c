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
        if( EFSS_BKPPAGE_EN == ( prmCntx->paramHandlerFlag & EFSS_BKPPAGE_EN) )
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

        while( ( iterator < prmCntx->nOfPages ) && ( EFSS_RES_OK == returnVal ) )
        {
            returnVal = isValidPage( prmCntx->pageSize, prmCntx->memPoolPointer, prmCntx->pageId,
                                               iterator, prmCntx->cbHolder, prmCntx->pageType);

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
                    returnVal = getPagePrmFromBuff(prmCntx->pageSize, prmCntx->memPoolPointer, &pagePrm);
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

        /* Information parsing procedure ended, analize */
        if( EFSS_RES_OK == returnVal)
        {
            /* no major error found */
            if( prmCntx->nOfPages == nonValidPageCounter )
            {
                /* All pages are corrupted, this is first init  */
                returnVal = EFSS_RES_OK_FIRSTINIT;

                /* Set all memory to zero */
                memset(prmCntx->memPoolPointer, 0u, prmCntx->pageSize);

                /* Set page parameter */
                pagePrm.pageTimeSetted = 1u;
                pagePrm.pageType = EFSS_PAGETYPE_RAW;
                pagePrm.allPageAlignmentNumber = 1u;
                pagePrm.pageVersion = prmCntx->rawPageVersion;
                pagePrm.pageMagicNumber = PARAM_32_MAGIC_NUMBER;

                iterator = 0u;
                while( ( iterator < prmCntx->nOfPages ) && ( EFSS_RES_OK_FIRSTINIT == returnVal ) )
                {
                    /* Write a page in memory */
                    returnVal = writePageNPrmNUpdateCrc(prmCntx->pageSize, prmCntx->memPoolPointer, prmCntx->pageId,
                                                        iterator, &pagePrm, prmCntx->cbHolder );
                    if( EFSS_RES_OK == returnVal )
                    {
                        returnVal = EFSS_RES_OK_FIRSTINIT;
                    }

                    iterator++;
                }
            }
            else if( 0u != nonValidPageCounter )
            {
                /* Some pages are corrupted */
                returnVal = EFSS_RES_OK_BUTNOTFIRSTINIT;

                /* Set all memory to zero */
                memset(prmCntx->memPoolPointer, 0u, prmCntx->pageSize);

                /* Set page parameter */
                pagePrm.pageTimeSetted = 1u;
                pagePrm.pageType = EFSS_PAGETYPE_RAW;
                pagePrm.allPageAlignmentNumber = 1u;
                pagePrm.pageVersion = prmCntx->rawPageVersion;
                pagePrm.pageMagicNumber = PARAM_32_MAGIC_NUMBER;

                iterator = 0u;
                while( ( iterator < prmCntx->nOfPages ) && ( EFSS_RES_OK_FIRSTINIT == returnVal ) )
                {
                    /* Write a page in memory */
                    returnVal = writePageNPrmNUpdateCrc(prmCntx->pageSize, prmCntx->memPoolPointer, prmCntx->pageId,
                                                        iterator, &pagePrm, prmCntx->cbHolder );
                    if( EFSS_RES_OK == returnVal )
                    {
                        returnVal = EFSS_RES_OK_BUTNOTFIRSTINIT;
                    }

                    iterator++;
                }
            }
            else
            {
                /* No page corrupted, check if are all aligned  */
                if( false == allAlignmentAreok )
                {
                    /* Alignment number not aligned */
                    returnVal = EFSS_RES_OK_BUTNOTFIRSTINIT;

                    /* Set all memory to zero */
                    memset(prmCntx->memPoolPointer, 0u, prmCntx->pageSize);

                    /* Set page parameter */
                    pagePrm.pageTimeSetted = 1u;
                    pagePrm.pageType = EFSS_PAGETYPE_RAW;
                    pagePrm.allPageAlignmentNumber = 1u;
                    pagePrm.pageVersion = prmCntx->rawPageVersion;
                    pagePrm.pageMagicNumber = PARAM_32_MAGIC_NUMBER;

                    iterator = 0u;
                    while( ( iterator < prmCntx->nOfPages ) && ( EFSS_RES_OK_FIRSTINIT == returnVal ) )
                    {
                        /* Write a page in memory */
                        returnVal = writePageNPrmNUpdateCrc(prmCntx->pageSize, prmCntx->memPoolPointer, prmCntx->pageId,
                                                            iterator, &pagePrm, prmCntx->cbHolder );
                        if( EFSS_RES_OK == returnVal )
                        {
                            returnVal = EFSS_RES_OK_BUTNOTFIRSTINIT;
                        }

                        iterator++;
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
    e_eFSS_Res returnVal = EFSS_RES_OK;
    e_eFSS_Res returnValintermedie;
    uint32_t iterator = 0;
    uint32_t firstInitCounter = 0;
    bool_t allAlignmentAreok = true;
    uint8_t aligmenentNumberFound = 0;
    s_prv_pagePrm pagePrm;

    /* Check for NULL pointer */
    if( NULL == prmCntx )
    {
        returnVal = EFSS_RES_BADPOINTER;
    }
    else
    {
        /* Check if it is first init */
        while( (iterator < prmCntx->nOfPages) && ( EFSS_RES_OK == returnVal) )
        {
            /* Read a page from memory */
            if( false == (*(prmCntx->cbHolder.pReadPg))(prmCntx->pageId, iterator, prmCntx->pageSize, prmCntx->memPoolPointer) )
            {
                returnVal = EFSS_RES_ERRORREAD;
            }
            else
            {
                returnValintermedie = isValidPageInBuff(prmCntx->pageSize, prmCntx->memPoolPointer, prmCntx->cbHolder, prmCntx->pageType);

                if( EFSS_RES_NOTVALIDPAGE == returnValintermedie )
                {
                    firstInitCounter++;
                    allAlignmentAreok = false;
                }
                else if( EFSS_RES_OK != returnValintermedie )
                {
                    returnVal = returnValintermedie;
                    allAlignmentAreok = false;
                }
                else
                {
                    if( true == allAlignmentAreok )
                    {
                        getPagePrmFromBuff(prmCntx->pageSize, prmCntx->memPoolPointer, &pagePrm);
                        if( 0u == iterator )
                        {
                            aligmenentNumberFound = pagePrm.allPageAlignmentNumber;
                        }

                        if(pagePrm.allPageAlignmentNumber != aligmenentNumberFound)
                        {
                            allAlignmentAreok = false;
                        }
                    }
                }
            }
            iterator++;
        }

        if( EFSS_RES_OK == returnVal)
        {
            if( (prmCntx->nOfPages == firstInitCounter) && (0u != firstInitCounter))
            {
                if(prmCntx->nOfPages == firstInitCounter)
                {
                    /* First time initializing this section */
                    returnVal = EFSS_RES_OK_FIRSTINIT;
                }
                else
                {
                    /* Some page initialized and other not, reinit everything */
                    returnVal = EFSS_RES_OK_BUTNOTFIRSTINIT;
                }

                /* Set all memory to zero */
                memset(prmCntx->memPoolPointer, 0, prmCntx->pageSize);

                /* Set page footer */
                pagePrm.pageTimeSetted = 1u;
                pagePrm.pageType = EFSS_PAGETYPE_RAW;
                pagePrm.allPageAlignmentNumber = 1u;
                pagePrm.pageVersion = prmCntx->rawPageVersion;
                pagePrm.pageMagicNumber = PARAM_32_MAGIC_NUMBER;

                iterator = 0;
                while( (iterator < prmCntx->nOfPages) && ( EFSS_RES_OK_FIRSTINIT == returnVal) )
                {
                    /* Write a page in memory */
                  if( false == writePageNPrmNUpdateCrc(prmCntx->pageSize, prmCntx->memPoolPointer, prmCntx->pageId, iterator, &pagePrm, prmCntx->cbHolder ) )
                    {
                        returnVal = EFSS_RES_ERRORREAD;
                    }

                    iterator++;
                }
            }
            else
            {
                /* Everithing was initialized, check if all page have the same allPageAlignmentNumber */
                if( false == allAlignmentAreok )
                {
                    /* Sbianca tutto */
                }
            }

        }
    }

    return returnVal;
}