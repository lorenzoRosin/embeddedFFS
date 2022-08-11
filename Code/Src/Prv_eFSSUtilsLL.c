/**
 * @file Prv_eFSSUtilsLL.c
 *
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "Prv_eFSSUtilsLL.h"



/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eFSS_Res erasePage( const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cb, const uint32_t pageIndx )
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


e_eFSS_Res writePage( const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cb, const uint32_t pageIndx, const uint8_t* dataW)
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



e_eFSS_Res readPage( const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cb, const uint32_t pageIndx, uint8_t* const dataR )
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



e_eFSS_Res calcCrc(const s_eFSS_Cb cb, uint32_t* const crc, const uint8_t* data, const uint32_t dataLen)
{
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




e_eFSS_Res calcCrcSeed(const s_eFSS_Cb cb, uint32_t* const crc, const uint8_t* data, const uint32_t dataLen,
                       const uint32_t seed )
{
            /* Calculate CRC */
            if( false == (*(cbHld.pCrc32))(crcCalc, pageBuff, pageCrcSizeToCalc, seed) )
            {
                /* Generic CRC calculation Fail */
                returnVal = EFSS_RES_BADPARAM;
            }
            else
            {
                returnVal = EFSS_RES_OK;
            }
}

