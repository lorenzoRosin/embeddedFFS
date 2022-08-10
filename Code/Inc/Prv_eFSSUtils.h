/**
 * @file Prv_eFSSUtils.h
 *
 */

#ifndef PRVEFSSUTILS_H
#define PRVEFSSUTILS_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSSType.h"



/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/
typedef struct
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
 * @param pageBuff Pointer to a page loaded in a buffer
 * @param pagePrm Pointer to a s_prv_pagePrm where we will copy the data readed from the buffer
 * @return e_eFSS_Res result
 */
e_eFSS_Res getPagePrmFromBuff(const uint32_t pageSize, const uint8_t* pageBuff, s_prv_pagePrm* const pagePrm);

/**
 * Set s_prv_pagePrm in a page already loaded in a Buffer
 * @param pageSize Size of the page loaded in the buffer
 * @param pageBuff Pointer to a page loaded in a buffer
 * @param pagePrm Pointer to s_prv_pagePrm that must be copied in the buffer
 * @return e_eFSS_Res result
 */
e_eFSS_Res setPagePrmInBuff(const uint32_t pageSize, uint8_t* const pageBuff, const s_prv_pagePrm* pagePrm);

/**
 * Set CRC in a page already loaded in a buffer
 * @param pageSize Size of the page loaded in the buffer
 * @param pageBuff Pointer to a page loaded in a buffer
 * @param crcToSet CRC we need to set
 * @return e_eFSS_Res result
 */
e_eFSS_Res setCrcInPagePrmBuff(const uint32_t pageSize, uint8_t* const pageBuff, const uint32_t crcToSet);

/**
 * Calculate the CRC of an already loaded Page in a buffer, excluding the CRC itself from the calc
 * @param pageSize Size of the page loaded in the buffer
 * @param pageBuff Pointer to a page loaded in a buffer
 * @param cbHld    Struct containing all callback reference
 * @param crcCalc  CRC calculated
 * @return e_eFSS_Res result
 */
e_eFSS_Res calcPagePrmCrcInBuff(const uint32_t pageSize, const uint8_t* pageBuff, const s_eFSS_Cb cbHld,
                                uint32_t* const crcCalc);

/**
 * Set s_prv_pagePrm in a page already loaded in a buffer, and set a fresh n' recalculated CRC
 * @param pageSize Size of the page loaded in the buffer
 * @param pageBuff Pointer to a page loaded in a buffer
 * @param cbHld    Struct containing all callback reference
 * @param pagePrm  To set data (s_prv_pagePrm)
 * @return e_eFSS_Res result
 */
e_eFSS_Res setPagePrmInBuffNCrcUp(const uint32_t pageSize, uint8_t* const pageBuff, const s_eFSS_Cb cbHld,
                                  const s_prv_pagePrm* pagePrm);

/**
 * Verify if the data present in a buffer page has CRC, magic number, and page type coherent with all the calculated
 * value
 * @param pageSize Size of the page loaded in the buffer
 * @param pageBuff Pointer to a page loaded in a buffer
 * @param cbHld    Struct containing all callback reference
 * @param e_eFSS_PageType    PageType we are aspecting
 * @return e_eFSS_Res result
 */
e_eFSS_Res isValidPageInBuff(const uint32_t pageSize,  const uint8_t* pageBuff, const s_eFSS_Cb cbHld,
                             const e_eFSS_PageType pageType);

/**
 * Verify if the data present in a page has CRC, magic number, and page type coherent with all the calculated
 * value
 * @param pageSize Size of the page loaded in the buffer
 * @param pageBuff Pointer to a buffer used for calculation
 * @param pageId   Id associated with the first page of the context
 * @param pageOffset   Offset from the start page indicated from the pageId
 * @param cbHld    Struct containing all callback reference
 * @param e_eFSS_PageType    PageType we are aspecting
 * @return e_eFSS_Res result
 */
e_eFSS_Res isValidPage( const uint32_t pageSize, uint8_t* const pageBuff, const uint32_t pageId,
                        const uint32_t pageOffset, const s_eFSS_Cb cbHld, const e_eFSS_PageType pageType);

/**
 * Erase and write a page with the param passed, and automatically update the CRC of the page
 * @param pageSize Size of the page
 * @param pageBuff Pointer to a buffer containing the data that we want to write in the page
 * @param pageId   Id associated with the first page of the context
 * @param pageOffset   Offset from the start page indicated from the pageId
 * @param prmPage   Param that we want to write in the page
 * @param cbHld    Struct containing all callback reference
 * @return e_eFSS_Res result
 */
e_eFSS_Res writePageNPrmNUpdateCrc(const uint32_t pageSize, uint8_t* const pageBuff, const uint32_t pageId,
                                   const uint32_t pageOffset, const s_prv_pagePrm* prmPage, const s_eFSS_Cb cbHld);

/**
 * Erase and write Tot numbers of page with the param passed, and automatically update the CRC of the page
 * @param pageSize Size of the page
 * @param pageBuff Pointer to a buffer containing the data that we want to write in the page
 * @param pageId   Id associated with the first page of the context
 * @param nOfPageToWrite Number of page to write
 * @param startOffset start page offset for the write process
 * @param prmPage Param that we want to write in the page
 * @param cbHld Struct containing all callback reference
 * @return e_eFSS_Res result
 */
e_eFSS_Res writeNPageNPrmNUpdateCrc(const uint32_t pageSize, uint8_t* const pageBuff, const uint32_t pageId,
                                    const uint32_t nOfPageToWrite, const uint32_t startOffset,
                                    const s_prv_pagePrm* prmPage, const s_eFSS_Cb cbHld);

/**
 * Read a page, and copy data param in pagePrm
 * @param pageSize Size of the page
 * @param pageBuff Pointer to a buffer that will be filled with data from the page
 * @param pageId   Id associated with the first page of the context
 * @param pageOffset   Offset from the start page indicated from the pageId
 * @param cbHld    Struct containing all callback reference
 * @param pagePrm   Param that we want to read
 * @return e_eFSS_Res result
 */
e_eFSS_Res readPageNPrm(const uint32_t pageSize, uint8_t* const pageBuff, const uint32_t pageId,
                        const uint32_t pageOffset, const s_eFSS_Cb cbHld, s_prv_pagePrm* const pagePrm);


#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* PRVEFSSUTILS_H */
