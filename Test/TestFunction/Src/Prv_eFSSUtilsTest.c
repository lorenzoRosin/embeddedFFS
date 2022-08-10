/**
 * @file Prv_eFSSUtilsTest.c
 *
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "Prv_eFSSUtilsTest.h"



/***********************************************************************************************************************
 *   PRIVATE FUNCTIONS
 **********************************************************************************************************************/
bool_t crcSum ( uint32_t* const calculatedCrc, const uint8_t* dataBuffer, const uint32_t calcSize, const uint32_t seed );


/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
bool_t Prv_eFSSUtilsTestFunc(void)
{
    /* Local variable */
    bool_t retValue = true;
    uint8_t buffer1[256u];
    uint8_t buffer2[256u];

    /* Test value */
    uint32_t vale32 = 0x11111111u;
    uint16_t vale16 = 0x2222u;
    uint8_t  vale8  = 0x33u;
    uint32_t offset1 = sizeof(buffer1) - ( sizeof(uint64_t) + sizeof(uint32_t) + sizeof(uint32_t) );
    uint32_t offset2 = sizeof(buffer1) - ( sizeof(uint64_t) + sizeof(uint32_t) );
    uint32_t offset3 = sizeof(buffer1) - ( sizeof(uint64_t) + sizeof(uint16_t) + sizeof(uint8_t) );
    uint32_t offset4 = sizeof(buffer1) - ( sizeof(uint64_t) + sizeof(uint16_t) );
    uint32_t offset5 = sizeof(buffer1) - ( sizeof(uint64_t) );
    uint32_t offset6 = sizeof(buffer1) - ( sizeof(uint32_t) );

    /***************************************************************************************************** Test set 1 */
    {
        s_prv_pagePrm param;
        memset(buffer1, 0u, sizeof(buffer1));
        (void)memcpy( &buffer1[offset1], (uint8_t*)&vale32,  sizeof(uint32_t) );
        (void)memcpy( &buffer1[offset2], (uint8_t*)&vale8,   sizeof(uint8_t)  );
        (void)memcpy( &buffer1[offset3], (uint8_t*)&vale8,   sizeof(uint8_t)  );
        (void)memcpy( &buffer1[offset4], (uint8_t*)&vale16,  sizeof(uint16_t) );
        (void)memcpy( &buffer1[offset5], (uint8_t*)&vale32,  sizeof(uint32_t) );
        (void)memcpy( &buffer1[offset6], (uint8_t*)&vale32,  sizeof(uint32_t) );

        if( EFSS_RES_OK == getPagePrmFromBuff(sizeof(buffer1), buffer1, &param) )
        {
            if( (vale32 == param.pageTimeSetted) && (vale8 == param.pageType) && (vale16 == param.pageVersion) )
            {
                retValue = true;
            }
            else
            {
                retValue = false;
            }
        }
        else
        {
            retValue = false;
        }
    }

    /***************************************************************************************************** Test set 2 */
    if( true == retValue )
    {
        s_prv_pagePrm paramToWrite;
        s_prv_pagePrm paramToRead;

        memset(buffer1, 0u, sizeof(buffer1));
        memset(buffer2, 0u, sizeof(buffer2));

        paramToWrite.pageTimeSetted = 0x10u;
        paramToWrite.pageType = 0x12u;
        paramToWrite.allPageAlignmentNumber = 0x34;
        paramToWrite.pageVersion = 0x8796u;
        paramToWrite.pageMagicNumber = 0x83626;
        paramToWrite.pageCrc = 0xABCDEF;


        if( EFSS_RES_OK == setPagePrmInBuff(sizeof(buffer1), buffer1, &paramToWrite) )
        {
            if( EFSS_RES_OK == getPagePrmFromBuff(sizeof(buffer1), buffer1, &paramToRead) )
            {
                if( 0 == memcmp(&paramToWrite, &paramToRead, sizeof(paramToRead)) )
                {
                    retValue = true;
                }
                else
                {
                    retValue = false;
                }
            }
            else
            {
                retValue = false;
            }
        }
        else
        {
            retValue = false;
        }
    }

    /***************************************************************************************************** Test set 3 */
    if( true == retValue )
    {
        s_prv_pagePrm paramToWrite;
        s_prv_pagePrm paramToRead;
        uint32_t crcToSet = 0xFFFF;
        memset(buffer1, 0u, sizeof(buffer1));
        memset(buffer2, 0u, sizeof(buffer2));

        paramToWrite.pageTimeSetted = 0x10u;
        paramToWrite.pageType = 0x12u;
        paramToWrite.allPageAlignmentNumber = 0x34;
        paramToWrite.pageVersion = 0x8796u;
        paramToWrite.pageMagicNumber = 0x83626;
        paramToWrite.pageCrc = 0xABCDEF;


        if( EFSS_RES_OK == setPagePrmInBuff(sizeof(buffer1), buffer1, &paramToWrite) )
        {
            if( EFSS_RES_OK == setCrcInPagePrmBuff(sizeof(buffer1), buffer1, crcToSet) )
            {
                if( EFSS_RES_OK == getPagePrmFromBuff(sizeof(buffer1), buffer1, &paramToRead) )
                {
                    if( 0xFFFF == paramToRead.pageCrc )
                    {
                        retValue = true;
                    }
                    else
                    {
                        retValue = false;
                    }
                }
                else
                {
                    retValue = false;
                }
            }
            else
            {
                retValue = false;
            }
        }
        else
        {
            retValue = false;
        }
    }

    /***************************************************************************************************** Test set 4 */
    if( true == retValue )
    {
        uint32_t crcTocalc = 0xFFFF;
        s_eFSS_Cb cbHld;
        cbHld.pCrc32 = crcSum;

        memset(buffer1, 0u, sizeof(buffer1));

        buffer1[0]   = 1u;
        buffer1[50]  = 1u;
        buffer1[70]  = 1u;
        buffer1[200] = 1u;

        if( EFSS_RES_OK == calcPagePrmCrcInBuff(sizeof(buffer1), buffer1, cbHld, &crcTocalc) )
        {
            if( (4u + CRC_BASE_SEED) == crcTocalc )
            {
                retValue = true;
            }
            else
            {
                retValue = false;
            }
        }
        else
        {
            retValue = false;
        }
    }




	return retValue;
}



/***********************************************************************************************************************
 *   PRIVATE FUNCTIONS
 **********************************************************************************************************************/
bool_t crcSum ( uint32_t* const calculatedCrc, const uint8_t* dataBuffer, const uint32_t calcSize, const uint32_t seed )
{
    uint32_t calculatedSumCrc = 0;

    for(uint32_t i=0; i<calcSize; i++ )
    {
        calculatedSumCrc +=  dataBuffer[i];
    }

    calculatedSumCrc += seed;

    *calculatedCrc = calculatedSumCrc;
    return true;
}