/**
 * @file ParamOrganizerType.h
 *
 */

#ifndef PARAMORGANIZER_TYPE_H
#define PARAMORGANIZER_TYPE_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>



/***********************************************************************************************************************
 *      DEFINES
 **********************************************************************************************************************/
#ifndef NULL
  #define NULL          _NULL
#endif

#ifndef bool_t
  #define bool_t          bool
#endif

#define PARAMORGAN_BKPPAGE_EN                       ( (1u) << (0u) )
#define PARAMORGAN_RAW_MODE                         ( (1u) << (1u) )

#define PARAMORGAN_DIVISOR_PAGE_BACKUP             (2u)
#define PARAMORGAN_DIVISOR_PAGE_SIZE_BYTE          (16u)
#define PARAMORGAN_MIN_PAGE_SIZE_BYTE              (256u)
#define PARAMORGAN_MAX_PAGE_SIZE_BYTE              (16384u)

#define CRC_BASE_SEED                               ( 0xffffffffu )

#define PARAM_32_MAGIC_NUMBER                       (0xa5a5a5a5u)



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/
typedef enum t_paramOrgResult
{
    PARAMRES_ALLOK = 0,
    PARAMRES_ALLOK_BKP_RCVRD,
    PARAMRES_ALLOK_NEWVER_UPDT,
    PARAMRES_ALLOK_FIRSTINIT,
    PARAMRES_ALLOK_BUTNOTFIRSTINIT,
    PARAMRES_NOTVALIDPAGE,
    PARAMRES_BADPARAM,
    PARAMRES_BADPOINTER,
    PARAMRES_BADTYPE,
    PARAMRES_NOT_INIT,
    PARAMRES_ERRORREAD,
    PARAMRES_ERRORWRITE,
    PARAMRES_ERRORERASE,
}e_paramOrgResult;


typedef bool_t (*cb_readPage)      ( const uint32_t startPageId, const uint32_t pageOffset, const uint32_t pageSize, uint8_t* dataToRead );
typedef bool_t (*cb_writePage)     ( const uint32_t startPageId, const uint32_t pageOffset, const uint32_t pageSize, uint8_t* dataToWrite );
typedef bool_t (*cb_erasePage)     ( const uint32_t startPageId, const uint32_t pageOffset, const uint32_t pageSize );
typedef bool_t (*cb_calculateCrc32)( uint32_t* calculatedCrc, const uint32_t calcSize, const uint32_t seed );


typedef struct t_paramOrgInitParameter
{
    /* Page information */
    uint32_t pageSize;
    uint32_t nOfPages;
    uint32_t pageId;

    /* Param Handler behaviour */
    uint32_t paramHandlerFlag;

    /* CallBack Pointer */
    cb_readPage pToReadFunc;
    cb_writePage pToWriteFunc;
    cb_erasePage pToEraseFunc;
    cb_calculateCrc32 pToCrcFunc;

    /* MemPool information */
    uint8_t* memPoolPointer;
    uint32_t memPoolSize;
}s_paramOrgInitParameter;

typedef struct t_paramOrgContext
{
    /* Page information */
    uint32_t pageSize;
    uint32_t nOfPages;
    uint32_t pageId;

    /* Param Handler behaviour */
    uint32_t paramHandlerFlag;

    /* CallBack Pointer */
    cb_readPage pToReadFunc;
    cb_writePage pToWriteFunc;
    cb_erasePage pToEraseFunc;
    cb_calculateCrc32 pToCrcFunc;

    /* MemPool information */
    uint8_t* memPoolPointer;
    uint32_t memPoolSize;

    /* Init information */
    bool_t isInitialized;
}s_paramOrgContext;

typedef enum t_paramOrgType
{
    PARAMTYPE_INT8 = 0,
    PARAMTYPE_UINT8,
    PARAMTYPE_INT16,
    PARAMTYPE_UINT16,
    PARAMTYPE_INT32,
    PARAMTYPE_UINT32,
    PARAMTYPE_FLOAT,
    PARAMTYPE_UINT64
}e_paramOrgType;

typedef struct t_paramOrgSingleParam
{
    uint64_t    parameter;
    uint16_t    parameterID;
    uint8_t     parameterType;
    uint16_t    parameterTimeSetted;
    uint8_t     parameterMagicNumber;
    uint16_t    parameterVersion;
}s_paramOrgSingleParam;



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* PARAMORGANIZER_TYPE_H */
