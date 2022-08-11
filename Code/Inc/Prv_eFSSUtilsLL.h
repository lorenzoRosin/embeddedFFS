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
 * Erase a memory page
 * @param pginfo Information about memory and pages
 * @param cb Information about callbacks and callbacks pointer
 * @param pageIndx page index to erase
 * @return e_eFSS_Res result
 */
e_eFSS_Res erasePageLL( const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cb, const uint32_t pageIndx );

/**
 * Erase a memory page
 * @param pginfo Information about memory and pages
 * @param cb Information about callbacks and callbacks pointer
 * @param pageIndx page index to erase
 * @return e_eFSS_Res result
 */
e_eFSS_Res writePageLL( const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cb, const uint32_t pageIndx, const uint8_t* dataW,
                      uint8_t* const supportMemory );

/**
 * Set CRC in a page already loaded in a buffer
 * @param pageSize Size of the page loaded in the buffer
 * @param pageBuff Pointer to a page loaded in a buffer
 * @param crcToSet CRC we need to set
 * @return e_eFSS_Res result
 */
e_eFSS_Res readPageLL( const s_eFSS_PgInfo pginfo, const s_eFSS_Cb cb, const uint32_t pageIndx, uint8_t* const dataR );

/**
 * Set CRC in a page already loaded in a buffer
 * @param pageSize Size of the page loaded in the buffer
 * @param pageBuff Pointer to a page loaded in a buffer
 * @param crcToSet CRC we need to set
 * @return e_eFSS_Res result
 */
e_eFSS_Res calcCrcLL(const s_eFSS_Cb cb, uint32_t* const crc, const uint8_t* data, const uint32_t dataLen);


/**
 * Set CRC in a page already loaded in a buffer
 * @param pageSize Size of the page loaded in the buffer
 * @param pageBuff Pointer to a page loaded in a buffer
 * @param crcToSet CRC we need to set
 * @return e_eFSS_Res result
 */
e_eFSS_Res calcCrcSeedLL(const s_eFSS_Cb cb, uint32_t* const crc, const uint8_t* data, const uint32_t dataLen,
                       const uint32_t seed );

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* PRVEFSSUTILSLL_H */
