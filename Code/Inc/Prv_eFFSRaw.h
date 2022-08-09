/**
 * @file Prv_eFFSRaw.h
 *
 */

#ifndef PRVPARAMORGANIZERRAW_H
#define PRVPARAMORGANIZERRAW_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFFSType.h"
#include "Prv_eFFSUtils.h"



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/

/**
 * Init memory layer, check version value, recover backup, re-init corrupt page
 * @param prmCntx Context of the module
 * @return e_eFSS_Res result
 */
e_eFSS_Res initParamMemoryRaw(s_eFSS_Ctx* prmCntx);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* PRVPARAMORGANIZERRAW_H */
