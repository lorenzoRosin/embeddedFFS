/**
 * @file Prv_ParamOrganizerUtils.h
 *
 */

#ifndef PRVPARAMORGANIZERUTILS_H
#define PRVPARAMORGANIZERUTILS_H



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
typedef struct t_paramOrgPageParam
{
    uint32_t    pageTimeSetted;
    uint32_t    pageType;
    uint32_t    pageMagicNumber;
    uint32_t    pageCrc;
}s_paramOrgPageParam;

typedef enum t_paramOrgPageType
{
    PARAMRES_TYPE_PARAM = 0,
    PARAMRES_TYPE_RAW,
}e_paramOrgPageType;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
e_paramOrgResult getPrgRamPageParam(s_paramOrgContext* prmCntx, uint8_t* page, s_paramOrgPageParam* prmPage);


/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
e_paramOrgResult setPrgRamPageParam(s_paramOrgContext* prmCntx, uint8_t* page, s_paramOrgPageParam* prmPage);


/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
e_paramOrgResult setCrcPrgRamPageParam(s_paramOrgContext* prmCntx, uint8_t* page, uint32_t* crcToSet);


/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
e_paramOrgResult calcPrgPageParamCrc(s_paramOrgContext* prmCntx, uint8_t* page, uint32_t* crcCalculated);



/**
 * Return with a pointer to the active screen
 * @param disp pointer to display which active screen should be get. (NULL to use the default
 * screen)
 * @return pointer to the active screen object (loaded by 'lv_scr_load()')
 */
e_paramOrgResult isValidDataInPage(s_paramOrgContext* prmCntx, uint32_t pageOffsetFromId);


#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* PRVPARAMORGANIZERUTILS_H */
