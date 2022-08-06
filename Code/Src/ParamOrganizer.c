/**
 * @file ParamOrganizer.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "ParamOrganizer.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      PRIVATE TYPEDEFS
 **********************/
typedef struct t_paramOrgPageParam
{
    uint32_t    pageTimeSetted;
    uint32_t    pageType;
    uint32_t    pageMagicNumber;
    uint32_t    pageCrc;
}t_paramOrgPageParam;

/**********************
 *   PRIVATE STATIC FUNCTIONS PROTOTYPES
 **********************/
e_paramOrgResult getPrgPageParam(s_paramOrgContext* prmCntx, uint8_t* page, t_paramOrgPageParam* prmPage);
e_paramOrgResult calcPrgPageParamCrc(s_paramOrgContext* prmCntx, uint8_t* page, uint32_t* crcCalculated);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

e_paramOrgResult initParamSettings(s_paramOrgContext* prmCntx, const s_paramOrgInitParameter* prmInitVal)
{
    e_paramOrgResult returnVal = PARAMRES_ALLOK;

    if( ( NULL == prmCntx ) || ( NULL == prmInitVal ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        if( true == prmCntx->isInitialized )
        {
            returnVal = PARAMRES_BADPARAM;
        }
        else
        {
            if( PARAMRES_ALLOK == returnVal )
            {
                if( ( prmInitVal->pageSize < PARAMORGAN_MIN_PAGE_SIZE_BYTE ) ||
                    ( prmInitVal->pageSize > PARAMORGAN_MAX_PAGE_SIZE_BYTE ) || ( 0u == prmInitVal->nOfPages ) )
                {
                    returnVal = PARAMRES_BADPARAM;
                    prmCntx->isInitialized = false;
                }
            }

            if( PARAMRES_ALLOK == returnVal )
            {
                if( 0u != ( prmInitVal->paramHandlerFlag & ( PARAMORGAN_BKPPAGE_EN | PARAMORGAN_RAW_MODE ) )  )
                {
                    returnVal = PARAMRES_BADPARAM;
                    prmCntx->isInitialized = false;
                }
            }

            if( PARAMRES_ALLOK == returnVal )
            {
                if( PARAMORGAN_BKPPAGE_EN == ( prmInitVal->paramHandlerFlag & PARAMORGAN_BKPPAGE_EN )  )
                {
                    if( 0u != ( prmInitVal->nOfPages % PARAMORGAN_DIVISOR_PAGE_BACKUP ) )
                    {
                        returnVal = PARAMRES_BADPARAM;
                        prmCntx->isInitialized = false;
                    }
                }
            }

            if( PARAMRES_ALLOK == returnVal )
            {
                if( ( NULL == prmInitVal->pToReadFunc  ) || ( NULL == prmInitVal->pToWriteFunc ) ||
                    ( NULL == prmInitVal->pToEraseFunc ) || ( NULL == prmInitVal->pToCrcFunc   ) )
                {
                    returnVal = PARAMRES_BADPARAM;
                    prmCntx->isInitialized = false;
                }
            }

            if( PARAMRES_ALLOK == returnVal )
            {
                if( ( NULL == prmInitVal->memPoolPointer  ) || ( prmInitVal->memPoolSize < prmInitVal->pageSize ) )
                {
                    returnVal = PARAMRES_BADPARAM;
                    prmCntx->isInitialized = false;
                }
            }

            if( PARAMRES_ALLOK == returnVal )
            {
                prmCntx->pageSize = prmInitVal->pageSize;
                prmCntx->nOfPages = prmInitVal->nOfPages;
                prmCntx->pageId = prmInitVal->pageId;

                prmCntx->paramHandlerFlag = prmInitVal->paramHandlerFlag;

                prmCntx->pToReadFunc = prmInitVal->pToReadFunc;
                prmCntx->pToWriteFunc = prmInitVal->pToWriteFunc;
                prmCntx->pToEraseFunc = prmInitVal->pToEraseFunc;
                prmCntx->pToCrcFunc = prmInitVal->pToCrcFunc;

                prmCntx->memPoolPointer = prmInitVal->memPoolPointer;
                prmCntx->memPoolSize = prmInitVal->memPoolSize;

                prmCntx->isInitialized = true;
            }
        }
    }

    return returnVal;
}





/**********************
 *   PRIVATE STATIC FUNCTIONS
 **********************/
e_paramOrgResult getPrgPageParam(s_paramOrgContext* prmCntx, uint8_t* page, t_paramOrgPageParam* prmPage)
{
    e_paramOrgResult returnVal;

    if( ( NULL == prmCntx ) || ( NULL == page ) || ( NULL == prmPage ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        uint32_t offset1 = sizeof(t_paramOrgPageParam);
        uint32_t offset2 = sizeof(t_paramOrgPageParam) - sizeof(uint32_t);
        uint32_t offset3 = sizeof(t_paramOrgPageParam) - sizeof(uint32_t)  - sizeof(uint32_t);
        uint32_t offset4 = sizeof(t_paramOrgPageParam) - sizeof(uint32_t)  - sizeof(uint32_t) - sizeof(uint32_t);

        memcpy(&prmPage->pageTimeSetted,  page[prmCntx->pageSize - offset1], sizeof(uint32_t) );
        memcpy(&prmPage->pageType,        page[prmCntx->pageSize - offset2], sizeof(uint32_t) );
        memcpy(&prmPage->pageMagicNumber, page[prmCntx->pageSize - offset3], sizeof(uint32_t) );
        memcpy(&prmPage->pageCrc,         page[prmCntx->pageSize - offset4], sizeof(uint32_t) );

        returnVal = PARAMRES_ALLOK;
    }

    return returnVal;
}


e_paramOrgResult calcPrgPageParamCrc(s_paramOrgContext* prmCntx, uint8_t* page, uint32_t* crcCalculated)
{
    e_paramOrgResult returnVal;

    if( ( NULL == prmCntx ) || ( NULL == page ) || ( NULL == prmPage ) )
    {
        returnVal = PARAMRES_BADPOINTER;
    }
    else
    {
        if( false == (*cb_calculateCrc32)( crcCalculated, prmCntx->pageSize - sizeof(uint32_t), CRC_BASE_SEED) )
        {
            returnVal = PARAMRES_BADPARAM;
        }
        else
        {
            returnVal = PARAMRES_ALLOK;
        }
    }

    return returnVal;
}














