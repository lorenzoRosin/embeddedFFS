/**
 * @file Prv_ParamOrganizerRaw.c
 *
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "Prv_ParamOrganizerRaw.h"



/***********************************************************************************************************************
 *   PRIVATE FUNCTIONS DECLARATION
 **********************************************************************************************************************/
e_eFSS_Res integrityCreatorRawNoBkp(s_eFSS_Ctx* prmCntx);
e_eFSS_Res integrityCreatorRawNoBkp(s_eFSS_Ctx* prmCntx);


/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_Res initParamMemoryRaw(s_eFSS_Ctx* prmCntx)
{
    /* Local variable */
    e_eFSS_Res returnVal;


    return returnVal;
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_Res integrityCreatorRawNoBkp(s_eFSS_Ctx* prmCntx)
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