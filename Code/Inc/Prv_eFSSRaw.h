/**
 * @file Prv_eFSSRaw.h
 *
 */

#ifndef PRVEFSSRAW_H
#define PRVEFSSRAW_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eFSSType.h"
#include "Prv_eFSSUtils.h"



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * Init memory layer, check version value, recover backup, re-init corrupt page
 * @param prmCntx Context of the module
 * @return e_eFSS_Res result
 */
e_eFSS_Res initEFSSRaw(const s_eFSS_Ctx* prmCntx);

#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* PRVEFSSRAW_H */
