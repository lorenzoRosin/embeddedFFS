/**
 * @file ParamOrganizerType.h
 *
 */

#ifndef PARAMORGANIZER_TYPE_H
#define PARAMORGANIZER_TYPE_H



#ifdef __cplusplus
extern "C" {
#endif




/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stdbool.h>

/*********************
 *      DEFINES
 *********************/
#ifndef NULL
  #define NULL          _NULL
#endif

#ifndef bool_t
  #define bool_t          bool
#endif

#define PARAMORGAN_BKPPAGE_EN                       ( (1u) << (0u) )
#define PARAMORGAN_RAW_MODE                         ( (1u) << (1u) )

#define PARAMORGAN_MIN_PAGE_SIZE                    (256u)
#define PARAMORGAN_MAX_PAGE_SIZE                    (16384u)

#define PARAM_8_MAGIC_NUMBER                        (0xa5)
#define PARAM_16_MAGIC_NUMBER                       (0xa5a5)
#define PARAM_32_MAGIC_NUMBER                       (0xa5a5a5a5)
#define PARAM_64_MAGIC_NUMBER                       (0xa5a5a5a5a5a5a5a5)

/**********************
 *      TYPEDEFS
 **********************/
typedef enum t_paramOrgResult
{
    PARAMRES_ALLOK = 0,
    PARAMRES_ALLOK_BKP_RCVRD,
    PARAMRES_ALLOK_NEWVER_UPDT,
    PARAMRES_ALLOK_FIRSTINIT,
    PARAMRES_BADPARAM,
    PARAMRES_BADPOINTER,
    PARAMRES_BADTYPE,
    PARAMRES_ERRORREAD,
    PARAMRES_ERRORWRITE,
    PARAMRES_ERRORERASE,
}e_paramOrgResult;


typedef bool_t (*fc_readPage)      ( const uint32_t startPageId, const uint32_t pageOffset, const uint32_t pageSize, uint8_t* dataToRead );
typedef bool_t (*fc_writePage)     ( const uint32_t startPageId, const uint32_t pageOffset, const uint32_t pageSize, uint8_t* dataToWrite );
typedef bool_t (*fc_erasePage)     ( const uint32_t startPageId, const uint32_t pageOffset, const uint32_t pageSize );
typedef bool_t (*fc_calculateCrc32)( uint32_t* calculatedCrc, const uint32_t seed );


typedef struct t_paramOrgContext
{
    /* Page information */
    uint32_t pageSize;
    uint32_t nOfPages;
    uint32_t pageId;

    /* Param Handler behaviour */
    uint32_t paramHandlerFlag;

    /* CallBack Pointer */
    void* pToReadFunc;
    void* pToWriteFunc;
    void* pToEraseFunc;
    void* pToCrcFunc;

    /* Init information */
    bool_t isPageInfoInitialized;
    bool_t isCallBackInitialized;
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
