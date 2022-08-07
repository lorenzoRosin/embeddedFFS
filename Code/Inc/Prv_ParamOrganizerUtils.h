/**
 * @file Prv_ParamOrganizerUtils.h
 *
 */

#ifndef PRVPARAMORGANIZERUTILS_H
#define PRVPARAMORGANIZERUTILS_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "ParamOrganizerType.h"



/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/
typedef struct t_prv_paramOrgPageParam
{
    uint32_t    pageTimeSetted;
    uint32_t    pageType;
    uint32_t    pageMagicNumber;
    uint32_t    pageCrc;
}s_prv_paramOrgPageParam;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/

/**
 * Get s_prv_paramOrgPageParam from a page already loaded in RAM
 * @param prmCntx Context of the module
 * @param page Pointer of a page loaded in RAM
 * @param prmPage Readed data (s_prv_paramOrgPageParam)
 * @return e_paramOrgResult result
 */
e_paramOrgResult getPagePrmFromRamBuff(s_paramOrgContext* prmCntx, uint8_t* page, s_prv_paramOrgPageParam* prmPage);

/**
 * Set s_prv_paramOrgPageParam in a page already loaded in RAM
 * @param prmCntx Context of the module
 * @param page Pointer of a page loaded in RAM
 * @param prmPage To set data (s_prv_paramOrgPageParam)
 * @return e_paramOrgResult result
 */
e_paramOrgResult setPagePrmInRamBuff(s_paramOrgContext* prmCntx, uint8_t* page, s_prv_paramOrgPageParam* prmPage);

/**
 * Set CRC in a page already loaded in RAM
 * @param prmCntx Context of the module
 * @param page Pointer of a page loaded in RAM
 * @param crcToSet CRC we need to set
 * @return e_paramOrgResult result
 */
e_paramOrgResult setCrcInPagePrmRamBuff(s_paramOrgContext* prmCntx, uint8_t* page, uint32_t* crcToSet);

/**
 * Calculate the CRC of an already loaded Page in RAM, excluding the CRC itself from the calc
 * @param prmCntx Context of the module
 * @param page Pointer of a page loaded in RAM
 * @param crcCalculated CRC calculated
 * @return e_paramOrgResult result
 */
e_paramOrgResult calcPagePrmCrcInRamBuff(s_paramOrgContext* prmCntx, uint8_t* page, uint32_t* crcCalculated);

/**
 * Verify if the data present in a loaded RAM page has CRC and magic numbers coherent with all the calculated value
 * @param prmCntx Context of the module
 * @param page Pointer of a page loaded in RAM
 * @return e_paramOrgResult result
 */
e_paramOrgResult isValidDataInPageRam(s_paramOrgContext* prmCntx, uint8_t* page);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* PRVPARAMORGANIZERUTILS_H */
