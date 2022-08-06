/**
 * @file ParamOrganizer.h
 *
 */

#ifndef PARAMORGANIZER_H
#define PARAMORGANIZER_H



#ifdef __cplusplus
extern "C" {
#endif




/*********************
 *      INCLUDES
 *********************/
#include "ParamOrganizerType.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
e_paramOrgResult initParamSettings(s_paramOrgContext* prmCntx, const uint32_t pageSize, const uint32_t nOfPages,
                                   const uint32_t pageId, const uint32_t paramHandlerFlag);




/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
e_paramOrgResult initParamCallBack(s_paramOrgContext* prmCntx, cb_readPage rpClb, cb_writePage wpClb,
                                         cb_erasePage epClb, cb_calculateCrc32 cc32Clb);


/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
e_paramOrgResult initLibMemory(s_paramOrgContext* prmCntx, uint8_t* memPool, uint32_t memPoolSize);

#if 0

/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
e_paramOrgResult initializeParamMemory();




/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
e_paramOrgResult saveParamInMemory();


/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
e_paramOrgResult readParamFromMemory();

/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
e_paramOrgResult saveRawParamInMemory();


/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
e_paramOrgResult readRawParamFromMemory();

/**********************
 *      MACROS
 **********************/

#endif


#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* PARAMORGANIZER_H */
