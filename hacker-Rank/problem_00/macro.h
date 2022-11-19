#pragma once

#include "macro.hpp" // <- comment this line to disable macro

#ifdef ALLOW_MACRO
#define cout_dump()         std::cout << __MAKE_DUMP__ << DUMP_ENDL
#define cout_dump_msg(x)    std::cout << __MAKE_DUMP__ << " " << x << DUMP_ENDL
#define cout_dump_this()    std::cout << __MAKE_DUMP__ << " #" << this << DUMP_ENDL
#define dump_duration(x)    DumpDuration UNIQ_ID(__LINE__){x}
#else
#define cout_dump()
#define cout_dump_msg(x)
#define cout_dump_this()
#define dump_duration(x)
#endif
