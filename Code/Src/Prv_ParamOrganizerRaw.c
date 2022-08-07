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
e_paramOrgResult integrityCreatorRawNoBkp(s_paramOrgContext* prmCntx);
e_paramOrgResult integrityCreatorRawNoBkp(s_paramOrgContext* prmCntx);


/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_paramOrgResult initParamMemoryRaw(s_paramOrgContext* prmCntx)
{
    /* Local variable */
    e_paramOrgResult returnVal;


    return returnVal;
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTIONS
 **********************************************************************************************************************/
e_paramOrgResult integrityCreatorRawNoBkp(s_paramOrgContext* prmCntx)
{
    /* Local variable */
    e_paramOrgResult returnVal = PARAMRES_ALLOK;
    e_paramOrgResult returnValintermedie;
    uint32_t iterator = 0;
    uint32_t firstInitCounter = 0;
    bool_t allAlignmentAreok = true;
    uint8_t aligmenentNumberFound = 0;
    s_prv_paramOrgPageParam prmPage;

    /* Check for NULL pointer */
    if( NULL == prmCntx )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        /* Check if it is first init */
        while( (iterator < prmCntx->nOfPages) && ( PARAMRES_ALLOK == returnVal) )
        {
            /* Read a page from memory */
            if( false == (*(prmCntx->pToReadFunc))(prmCntx->pageId, iterator, prmCntx->pageSize, prmCntx->memPoolPointer) )
            {
                returnVal = PARAMRES_ERRORREAD;
            }
            else
            {
                returnValintermedie = isValidDataInPageRam(prmCntx, prmCntx->memPoolPointer);

                if( PARAMRES_NOTVALIDPAGE == returnValintermedie )
                {
                    firstInitCounter++;
                    allAlignmentAreok = false;
                }
                else if( PARAMRES_ALLOK != returnValintermedie )
                {
                    returnVal = returnValintermedie;
                    allAlignmentAreok = false;
                }
                else
                {
                    if( true == allAlignmentAreok )
                    {
                        getPagePrmFromRamBuff(prmCntx, prmCntx->memPoolPointer, &prmPage);
                        if( 0u == iterator )
                        {
                            aligmenentNumberFound = prmPage.allPageAlignmentNumber;
                        }

                        if(prmPage.allPageAlignmentNumber != aligmenentNumberFound)
                        {
                            allAlignmentAreok = false;
                        }
                    }
                }
            }
            iterator++;
        }

        if( PARAMRES_ALLOK == returnVal)
        {
            if( (prmCntx->nOfPages == firstInitCounter) && (0u != firstInitCounter))
            {
                if(prmCntx->nOfPages == firstInitCounter)
                {
                    /* First time initializing this section */
                    returnVal = PARAMRES_ALLOK_FIRSTINIT;
                }
                else
                {
                    /* Some page initialized and other not, reinit everything */
                    returnVal = PARAMRES_ALLOK_BUTNOTFIRSTINIT;
                }

                /* Set all memory to zero */
                memset(prmCntx->memPoolPointer, 0, prmCntx->pageSize);

                /* Set page footer */
                prmPage.pageTimeSetted = 1u;
                prmPage.pageType = PARAMRES_PAGETYPE_RAW;
                prmPage.allPageAlignmentNumber = 1u;
                prmPage.pageVersion = prmCntx->rawPageVersion;
                prmPage.pageMagicNumber = PARAM_32_MAGIC_NUMBER;

                iterator = 0;
                while( (iterator < prmCntx->nOfPages) && ( PARAMRES_ALLOK_FIRSTINIT == returnVal) )
                {
                    /* Write a page in memory */
                    if( false == writePageFlashNUpdateCrc(prmCntx, iterator, prmCntx->pageSize, prmCntx->memPoolPointer) )
                    {
                        returnVal = PARAMRES_ERRORREAD;
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