/**
 * @file eFSS.h
 *
 */

#ifndef EFSS_H
#define EFSS_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSSType.h"



/***********************************************************************************************************************
 *      DEFINES
 **********************************************************************************************************************/



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/

/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
e_eFSS_Res initParamSettings(s_eFSS_Ctx* prmCntx, const s_eFSS_InitParam* prmInitVal);



#if 0

/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
e_eFSS_Res initializeParamMemory();




/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
e_eFSS_Res saveParamInMemory();


/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
e_eFSS_Res readParamFromMemory();

/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
e_eFSS_Res saveRawParamInMemory();


/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
e_eFSS_Res readRawParamFromMemory();



#endif


#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* EFSS_H */
