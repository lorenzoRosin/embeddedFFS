/**
 * @file ParamOrganizer.h
 *
 */

#ifndef PARAMORGANIZER_H
#define PARAMORGANIZER_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "ParamOrganizerType.h"



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
e_paramOrgResult initParamSettings(s_paramOrgContext* prmCntx, const s_paramOrgInitParameter* prmInitVal);



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



#endif


#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* PARAMORGANIZER_H */
