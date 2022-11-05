#ifndef __MACRO_H__
#define __MACRO_H__

#include <iostream>
#include <cstring>

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

#define cout_dump()  std::cout << __MAKE_DUMP__ << std::endl
#define cout_dump_msg(x)  std::cout << __MAKE_DUMP__ << " " << x << std::endl
#define checkpoint(x) std::cout << __MAKE_DUMP__ << " Checkpoint#" << #x << std::endl

#endif // __MACRO_H__
