//////////////////////////////////////////////////////////////////////////
// Module: Types.h - definitions of Global Common namespace
//////////////////////////////////////////////////////////////////////////

#ifndef __GCN_TYPES_H__
#define __GCN_TYPES_H__

#include <string>
#include <iostream>

#ifdef _MSC_VER
#define __CXX_VER__ _MSVC_LANG
#define __PRETTY_FUNCTION__ __FUNCSIG__
#define __DELIM__ '\\'
#else
#define __CXX_VER__ __cplusplus
#define __DELIM__ '/'
#endif

// C++11 requires a space between literal and string macro
#define __FILENAME__ ( std::strrchr( "/" __FILE__, __DELIM__ ) + 1 )
#define __MAKE_DUMP__   __FILENAME__ << " : " << __LINE__ << " | " << __PRETTY_FUNCTION__

#define checkpoint(x) std::cout << __MAKE_DUMP__ << " Checkpoint#" << #x << std::endl

#ifdef MAKE_COUT_DUMP // NOT thread-safe
#define cout_dump()  std::cout << __MAKE_DUMP__ <<  std::endl
#define cout_dump_msg(x)  std::cout << __MAKE_DUMP__ << " " << x << std::endl
#define cout_dump_this()  std::cout << __MAKE_DUMP__ << " #" << this << std::endl
#else
#define cout_dump()
#define cout_dump_msg(x)
#define cout_dump_this()
#endif

//////////////////////////////////////////////////////////////////////////

#define GCN_NAMESPACE_BEGIN namespace gcn { // global common namespace
#define GCN_NAMESPACE_END }

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////

typedef int8_t              INT8_t;
typedef uint8_t             UINT8_t;
typedef int16_t             INT16_t;
typedef uint16_t            UINT16_t;
typedef int32_t             INT32_t;
typedef uint32_t            UINT32_t;
typedef int64_t             INT64_t;
typedef uint64_t            UINT64_t;
typedef float               FLOAT_t;
typedef double              DOUBLE_t;

//////////////////////////////////////////////////////////////////////////

#define GCN_DATA_TYPE                                                 \
    X( GCN_DATA_TYPE_UNKNOWN = 0, "GCN_DATA_TYPE_UNKNOWN" )           \
    X( GCN_DATA_TYPE_INT32,       "GCN_DATA_TYPE_INT32"   )           \
    X( GCN_DATA_TYPE_UINT32,      "GCN_DATA_TYPE_UINT32"  )           \
    X( GCN_DATA_TYPE_INT64,       "GCN_DATA_TYPE_INT64"   )           \
    X( GCN_DATA_TYPE_UINT64,      "GCN_DATA_TYPE_UINT64"  )           \
    X( GCN_DATA_TYPE_FLOAT,       "GCN_DATA_TYPE_FLOAT"   )           \
    X( GCN_DATA_TYPE_DOUBLE,      "GCN_DATA_TYPE_DOUBLE"  )           \
    X( GCN_DATA_TYPE_BOOLEAN,     "GCN_DATA_TYPE_BOOLEAN" )           \
    X( GCN_DATA_TYPE_TIME,        "GCN_DATA_TYPE_TIME"    )           \
    X( GCN_DATA_TYPE_DATE,        "GCN_DATA_TYPE_DATE"    )           \
    X( GCN_DATA_TYPE_DATE_TIME,   "GCN_DATA_TYPE_DATE_TIME" )         \
    X( GCN_DATA_TYPE_STRING,      "GCN_DATA_TYPE_STRING"  )           \
    X( GCN_DATA_TYPE_BINARY,      "GCN_DATA_TYPE_BINARY"  )           \
    X( GCN_DATA_TYPE_PASSWORD,    "GCN_DATA_TYPE_PASSWORD" )

#define GCN_DATA_TYPE_UNKNOWN_STR                                    "0"
#define GCN_DATA_TYPE_INT32_STR                                      "1"
#define GCN_DATA_TYPE_UINT32_STR                                     "2"
#define GCN_DATA_TYPE_INT64_STR                                      "3"
#define GCN_DATA_TYPE_UINT64_STR                                     "4"
#define GCN_DATA_TYPE_FLOAT_STR                                      "5"
#define GCN_DATA_TYPE_DOUBLE_STR                                     "6"
#define GCN_DATA_TYPE_BOOLEAN_STR                                    "7"
#define GCN_DATA_TYPE_TIME_STR                                       "8"
#define GCN_DATA_TYPE_DATE_STR                                       "9"
#define GCN_DATA_TYPE_DATE_TIME_STR                                  "10"
#define GCN_DATA_TYPE_STRING_STR                                     "11"
#define GCN_DATA_TYPE_BINARY_STR                                     "12"
#define GCN_DATA_TYPE_PASSWORD_STR                                   "13"

#define X( a, b ) a,
enum DataType { GCN_DATA_TYPE };
#undef X

//////////////////////////////////////////////////////////////////////////

template< typename data_info_prop_t, data_info_prop_t prop > struct type_adapter;

#define SET_VALUE_TYPE( __PropType, __Prop, __PropValueType ) template<> struct type_adapter< __PropType, __Prop > { enum{ prop = __Prop }; typedef __PropValueType prop_value_t; }

SET_VALUE_TYPE( DataType, GCN_DATA_TYPE_INT32,  INT32_t );
SET_VALUE_TYPE( DataType, GCN_DATA_TYPE_UINT32, UINT32_t );
SET_VALUE_TYPE( DataType, GCN_DATA_TYPE_INT64,  INT64_t );
SET_VALUE_TYPE( DataType, GCN_DATA_TYPE_UINT64, UINT64_t );
SET_VALUE_TYPE( DataType, GCN_DATA_TYPE_FLOAT,  FLOAT_t );
SET_VALUE_TYPE( DataType, GCN_DATA_TYPE_DOUBLE, DOUBLE_t );
SET_VALUE_TYPE( DataType, GCN_DATA_TYPE_BOOLEAN, bool );
SET_VALUE_TYPE( DataType, GCN_DATA_TYPE_TIME,   INT64_t );
SET_VALUE_TYPE( DataType, GCN_DATA_TYPE_DATE,   INT64_t );
SET_VALUE_TYPE( DataType, GCN_DATA_TYPE_DATE_TIME, INT64_t );
SET_VALUE_TYPE( DataType, GCN_DATA_TYPE_STRING, std::string );
SET_VALUE_TYPE( DataType, GCN_DATA_TYPE_BINARY, std::string );
SET_VALUE_TYPE( DataType, GCN_DATA_TYPE_PASSWORD, std::string );

template < int v = -1 >
struct Int2Type
{
    enum { value = v };
};

//////////////////////////////////////////////////////////////////////////

struct GCN_UUID
{
    UINT32_t nData1;
    UINT16_t nData2;
    UINT16_t nData3;
    UINT8_t arrData[8];

    bool operator != ( const GCN_UUID& uuid2 ) const
    {
        return  this->nData1 != uuid2.nData1 ||
                this->nData2 != uuid2.nData2 ||
                this->nData3 != uuid2.nData3 ||
                ( std::memcmp( this->arrData, uuid2.arrData, sizeof(this->arrData) ) != 0 );
    }
};

//////////////////////////////////////////////////////////////////////////

#define CC_RESULT_CODES                                         \
    X( CC_OK = 0,                   "CC_OK"                )    \
    X( CC_UNKNOWN_ERROR,            "CC_UNKNOWN_ERROR"     )    \
    X( CC_INIT_ERROR,               "CC_INIT_ERROR"        )    \
    X( CC_INVALID_PARAMETER,        "CC_INVALID_PARAMETER" )    \
    X( CC_INVALID_HANDLE,           "CC_INVALID_HANDLE"    )    \
    X( CC_UNKNOWN_INTERFACE = 1024, "CC_UNKNOWN_INTERFACE" )

#define X( a, b ) a,
enum ResultCode { CC_RESULT_CODES };
#undef X

//////////////////////////////////////////////////////////////////////////

#define GCN_LOG_LEVEL                                     \
    X( GCN_LL_FATAL = 2,        "GCN_LL_FATAL" )          \
    X( GCN_LL_ERROR,            "GCN_LL_ERROR" )          \
    X( GCN_LL_WARNING,          "GCN_LL_WARNING" )        \
    X( GCN_LL_INFO,             "GCN_LL_INFO" )           \
    X( GCN_LL_DEBUG,            "GCN_LL_DEBUG" )          \
    X( GCN_LL_FUNC,             "GCN_LL_FUNC" )

#define X( a, b ) a,
enum LogLevel { GCN_LOG_LEVEL };
#undef X

//////////////////////////////////////////////////////////////////////////

GCN_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif //__GCN_TYPES_H__
