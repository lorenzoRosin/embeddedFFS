/**
 * @file Prv_ParamOrganizerUtils.h
 *
 */

#ifndef PRVPARAMORGANIZERUTILS_H
#define PRVPARAMORGANIZERUTILS_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "ParamOrganizerType.h"



/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/
typedef struct t_prv_pagePrm
{
    uint32_t    pageTimeSetted;
    uint8_t     pageType;
    uint8_t     allPageAlignmentNumber;
    uint16_t    pageVersion;
    uint32_t    pageMagicNumber;
    uint32_t    pageCrc;
}s_prv_pagePrm;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/

/**
 * Get s_prv_pagePrm from a page already loaded in a Buffer
 * @param pageSize Size of the page loaded in the buffer
 * @param pageBuff Pointer to a page readed in a buffer
 * @param pagePrm Pointer to a s_prv_pagePrm where we will copy the data readed from the buffer
 * @return e_eFSS_Res result
 */
e_eFSS_Res getPagePrmFromBuff(const uint32_t pageSize, const uint8_t* pageBuff, s_prv_pagePrm* const pagePrm);

/**
 * Set s_prv_pagePrm in a page already loaded in in a Buffer
 * @param pageSize Size of the page loaded in the buffer
 * @param pageBuff Pointer to a page readed in a buffer
 * @param pagePrm Pointer to s_prv_pagePrm that must be copied in the buffer
 * @return e_eFSS_Res result
 */
e_eFSS_Res setPagePrmInBuff(const uint32_t pageSize, uint8_t* const pageBuff, const s_prv_pagePrm* pagePrm);

/**
 * Set CRC in a page already loaded in RAM
 * @param prmCntx Context of the module
 * @param page Pointer of a page loaded in RAM
 * @param crcToSet CRC we need to set
 * @return e_eFSS_Res result
 */
e_eFSS_Res setCrcInPagePrmBuff(uint32_t pageSize, uint8_t* pageBuff, uint32_t crcToSet);

/**
 * Calculate the CRC of an already loaded Page in RAM, excluding the CRC itself from the calc
 * @param prmCntx Context of the module
 * @param page Pointer of a page loaded in RAM
 * @param crcCalc CRC calculated
 * @return e_eFSS_Res result
 */
e_eFSS_Res calcPagePrmCrcInBuff(uint32_t pageSize, uint8_t* pageBuff, s_eFSS_Cb cbHld, uint32_t* crcCalc);

/**
 * Set s_prv_pagePrm in a page already loaded in RAM, and set a fresh n' recalculated CRC
 * @param prmCntx Context of the module
 * @param page Pointer of a page loaded in RAM
 * @param pagePrm To set data (s_prv_pagePrm)
 * @return e_eFSS_Res result
 */
e_eFSS_Res setPagePrmInBuffNCrcUp(uint32_t pageSize, uint8_t* pageBuff, s_eFSS_Cb cbHld, s_prv_pagePrm* pagePrm);

/**
 * Verify if the data present in a loaded RAM page has CRC magic and type numbers coherent with all the calculated value
 * @param prmCntx Context of the module
 * @param page Pointer of a page loaded in RAM
 * @return e_eFSS_Res result
 */
e_eFSS_Res isValidPageInBuff(uint32_t pageSize,  uint8_t* pageBuff, s_eFSS_Cb cbHld, e_eFSS_PageType pageType);

/**
 * Verify if the data present in a loaded RAM page has CRC magic and type numbers coherent with all the calculated value
 * @param prmCntx Context of the module
 * @param page Pointer of a page loaded in RAM
 * @return e_eFSS_Res result
 */
e_eFSS_Res isValidPage( uint32_t pageSize, uint8_t* pageBuff, const uint32_t pageId, const uint32_t pageOffset, s_eFSS_Cb cbHld, e_eFSS_PageType pageType);

/**
 * Erase and write a page passed, but before automatically calculate the crc to write in the s_prv_pagePrm
 * @param prmCntx Context of the module
 * @param page Pointer of a page loaded in RAM
 * @param pagePrm To set data (s_prv_pagePrm)
 * @return e_eFSS_Res result
 */
e_eFSS_Res writePageNPrmNUpdateCrc(uint32_t pageSize, uint8_t* pageBuff, const uint32_t pageId, const uint32_t pageOffset, s_prv_pagePrm* prmPage, s_eFSS_Cb cbHld);

/**
 * Read a page, but before automatically calculate the crc to write in the s_prv_pagePrm
 * @param prmCntx Context of the module
 * @param page Pointer of a page loaded in RAM
 * @param pagePrm To set data (s_prv_pagePrm)
 * @return e_eFSS_Res result
 */
e_eFSS_Res readPageNPrm(uint32_t pageSize, uint8_t* pageBuff, const uint32_t pageId, s_eFSS_Ctx* prmCntx, const uint32_t pageOffset, s_eFSS_Cb cbHld, s_prv_pagePrm* pagePrm);


#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* PRVPARAMORGANIZERUTILS_H */
