/**
 * @file Prv_eFSSUtilsTest.h
 *
 */

#ifndef PRVEFSSUTILSTEST_H
#define PRVEFSSUTILSTEST_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "Prv_eFSSUtils.h"



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
 * Test all Prv_eFSSUtils function
 * @return e_eFSS_Res result
 */
bool_t Prv_eFSSUtilsTestFunc(void);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* PRVEFSSUTILSTEST_H */
