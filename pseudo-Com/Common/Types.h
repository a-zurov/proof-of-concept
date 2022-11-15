//////////////////////////////////////////////////////////////////////////
// Module: Types.h - definitions of global pseudo-Com namespace xcom::
//////////////////////////////////////////////////////////////////////////

#ifndef __XCOM_TYPES_H__
#define __XCOM_TYPES_H__

#include <cstring>
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

#define XCOM_NAMESPACE_BEGIN namespace xcom { // global pseudo-Com namespace
#define XCOM_NAMESPACE_END }

//////////////////////////////////////////////////////////////////////////

XCOM_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////

typedef int8_t      INT8_t;
typedef uint8_t     UINT8_t;
typedef int16_t     INT16_t;
typedef uint16_t    UINT16_t;
typedef int32_t     INT32_t;
typedef uint32_t    UINT32_t;
typedef int64_t     INT64_t;
typedef uint64_t    UINT64_t;
typedef float       FLOAT_t;
typedef double      DOUBLE_t;

//////////////////////////////////////////////////////////////////////////

#define XCOM_DATA_TYPE                                                  \
    X( XCOM_DATA_TYPE_UNKNOWN = 0, "XCOM_DATA_TYPE_UNKNOWN" )           \
    X( XCOM_DATA_TYPE_INT32,       "XCOM_DATA_TYPE_INT32"   )           \
    X( XCOM_DATA_TYPE_UINT32,      "XCOM_DATA_TYPE_UINT32"  )           \
    X( XCOM_DATA_TYPE_INT64,       "XCOM_DATA_TYPE_INT64"   )           \
    X( XCOM_DATA_TYPE_UINT64,      "XCOM_DATA_TYPE_UINT64"  )           \
    X( XCOM_DATA_TYPE_FLOAT,       "XCOM_DATA_TYPE_FLOAT"   )           \
    X( XCOM_DATA_TYPE_DOUBLE,      "XCOM_DATA_TYPE_DOUBLE"  )           \
    X( XCOM_DATA_TYPE_BOOLEAN,     "XCOM_DATA_TYPE_BOOLEAN" )           \
    X( XCOM_DATA_TYPE_TIME,        "XCOM_DATA_TYPE_TIME"    )           \
    X( XCOM_DATA_TYPE_DATE,        "XCOM_DATA_TYPE_DATE"    )           \
    X( XCOM_DATA_TYPE_DATE_TIME,   "XCOM_DATA_TYPE_DATE_TIME" )         \
    X( XCOM_DATA_TYPE_STRING,      "XCOM_DATA_TYPE_STRING"  )           \
    X( XCOM_DATA_TYPE_BINARY,      "XCOM_DATA_TYPE_BINARY"  )           \
    X( XCOM_DATA_TYPE_PASSWORD,    "XCOM_DATA_TYPE_PASSWORD" )

#define XCOM_DATA_TYPE_UNKNOWN_STR      "0"
#define XCOM_DATA_TYPE_INT32_STR        "1"
#define XCOM_DATA_TYPE_UINT32_STR       "2"
#define XCOM_DATA_TYPE_INT64_STR        "3"
#define XCOM_DATA_TYPE_UINT64_STR       "4"
#define XCOM_DATA_TYPE_FLOAT_STR        "5"
#define XCOM_DATA_TYPE_DOUBLE_STR       "6"
#define XCOM_DATA_TYPE_BOOLEAN_STR      "7"
#define XCOM_DATA_TYPE_TIME_STR         "8"
#define XCOM_DATA_TYPE_DATE_STR         "9"
#define XCOM_DATA_TYPE_DATE_TIME_STR    "10"
#define XCOM_DATA_TYPE_STRING_STR       "11"
#define XCOM_DATA_TYPE_BINARY_STR       "12"
#define XCOM_DATA_TYPE_PASSWORD_STR     "13"

#define X( a, b ) a,
enum DataType { XCOM_DATA_TYPE };
#undef X

//////////////////////////////////////////////////////////////////////////

template< typename data_info_prop_t, data_info_prop_t prop > struct type_adapter;

#define SET_VALUE_TYPE( __PropType, __Prop, __PropValueType ) template<> struct type_adapter< __PropType, __Prop > { enum{ prop = __Prop }; typedef __PropValueType prop_value_t; }

SET_VALUE_TYPE( DataType, XCOM_DATA_TYPE_INT32,     INT32_t );
SET_VALUE_TYPE( DataType, XCOM_DATA_TYPE_UINT32,    UINT32_t );
SET_VALUE_TYPE( DataType, XCOM_DATA_TYPE_INT64,     INT64_t );
SET_VALUE_TYPE( DataType, XCOM_DATA_TYPE_UINT64,    UINT64_t );
SET_VALUE_TYPE( DataType, XCOM_DATA_TYPE_FLOAT,     FLOAT_t );
SET_VALUE_TYPE( DataType, XCOM_DATA_TYPE_DOUBLE,    DOUBLE_t );
SET_VALUE_TYPE( DataType, XCOM_DATA_TYPE_BOOLEAN,   bool );
SET_VALUE_TYPE( DataType, XCOM_DATA_TYPE_TIME,      INT64_t );
SET_VALUE_TYPE( DataType, XCOM_DATA_TYPE_DATE,      INT64_t );
SET_VALUE_TYPE( DataType, XCOM_DATA_TYPE_DATE_TIME, INT64_t );
SET_VALUE_TYPE( DataType, XCOM_DATA_TYPE_STRING,    std::string );
SET_VALUE_TYPE( DataType, XCOM_DATA_TYPE_BINARY,    std::string );
SET_VALUE_TYPE( DataType, XCOM_DATA_TYPE_PASSWORD,  std::string );

template < int v = -1 >
struct Int2Type
{
    enum { value = v };
};

//////////////////////////////////////////////////////////////////////////

struct XCOM_UUID
{
    UINT32_t nData1;
    UINT16_t nData2;
    UINT16_t nData3;
    UINT8_t arrData[8];

    bool operator != ( const XCOM_UUID& uuid2 ) const
    {
        return  this->nData1 != uuid2.nData1 ||
                this->nData2 != uuid2.nData2 ||
                this->nData3 != uuid2.nData3 ||
                ( std::memcmp( this->arrData, uuid2.arrData, sizeof(this->arrData) ) != (int)NULL);
    }
};

inline bool EqualsUUID(const XCOM_UUID& uuid1, const XCOM_UUID& uuid2)
{
    bool bRes = (      uuid1.nData1 == uuid2.nData1
                    && uuid1.nData2 == uuid2.nData2
                    && uuid1.nData3 == uuid2.nData3
                    && memcmp( (const void*)uuid1.arrData, (const void*)uuid2.arrData, sizeof(uuid1.arrData) ) == (int)NULL );
    return bRes;
}

//////////////////////////////////////////////////////////////////////////

#define XCOM_RESULT_CODES                               \
    X( OK = 0,                  "OK"               )    \
    X( UNKNOWN_ERROR,           "UNKNOWN_ERROR"    )    \
    X( INIT_ERROR,              "INIT_ERROR"       )    \
    X( INVALID_PARAMETER,       "INVALID_PARAMETER")    \
    X( INVALID_HANDLE,          "INVALID_HANDLE"   )    \
    X( UNKNOWN_INTERFACE = 1024, "UNKNOWN_INTERFACE" )

#define X( a, b ) a,
enum ResultCode { XCOM_RESULT_CODES };
#undef X

//////////////////////////////////////////////////////////////////////////

#define XCOM_LOG_LEVEL                  \
    X( LL_FATAL = 2,    "LL_FATAL" )    \
    X( LL_ERROR,        "LL_ERROR" )    \
    X( LL_WARNING,      "LL_WARNING" )  \
    X( LL_INFO,         "LL_INFO" )     \
    X( LL_DEBUG,        "LL_DEBUG" )    \
    X( LL_PREBIND,      "LL_PREBIND" )

#define X( a, b ) a,
enum LogLevel { XCOM_LOG_LEVEL };
#undef X




//////////////////////////////////////////////////////////////////////////

XCOM_NAMESPACE_END

//////////////////////////////////////////////////////////////////////////

#endif //__XCOM_TYPES_H__
