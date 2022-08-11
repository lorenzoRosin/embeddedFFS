/**
 * @file Prv_eFSSUtilsLL.h
 *
 */

#ifndef PRVEFSSUTILSLL_H
#define PRVEFSSUTILSLL_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "Prv_eFSSUtils.h"



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
e_eFSS_Res erasePage( const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cb, const uint32_t pageIndx );

/**
 * Set s_prv_pagePrm in a page already loaded in a Buffer
 * @param pageSize Size of the page loaded in the buffer
 * @param pageBuff Pointer to a page loaded in a buffer
 * @param pagePrm Pointer to s_prv_pagePrm that must be copied in the buffer
 * @return e_eFSS_Res result
 */
e_eFSS_Res writePage( const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cb, const uint32_t pageIndx, const uint8_t* dataW);

/**
 * Set CRC in a page already loaded in a buffer
 * @param pageSize Size of the page loaded in the buffer
 * @param pageBuff Pointer to a page loaded in a buffer
 * @param crcToSet CRC we need to set
 * @return e_eFSS_Res result
 */
e_eFSS_Res readPage( const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cb, const uint32_t pageIndx, uint8_t* const dataR );

/**
 * Set CRC in a page already loaded in a buffer
 * @param pageSize Size of the page loaded in the buffer
 * @param pageBuff Pointer to a page loaded in a buffer
 * @param crcToSet CRC we need to set
 * @return e_eFSS_Res result
 */
e_eFSS_Res calcCrc(const s_eFSS_Cb cb, uint32_t* const crc, const uint8_t* data, const uint32_t dataLen);


/**
 * Set CRC in a page already loaded in a buffer
 * @param pageSize Size of the page loaded in the buffer
 * @param pageBuff Pointer to a page loaded in a buffer
 * @param crcToSet CRC we need to set
 * @return e_eFSS_Res result
 */
e_eFSS_Res calcCrcSeed(const s_eFSS_Cb cb, uint32_t* const crc, const uint8_t* data, const uint32_t dataLen,
                       const uint32_t seed );

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* PRVEFSSUTILSLL_H */
