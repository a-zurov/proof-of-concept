#include "macro.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_NAMESPACE_CXX

//////////////////////////////////////////////////////////////////////////

std::ostream& operator << (std::ostream& os, const constString& cstr) {

    return cstr.m_pBuff ? os.write(cstr.m_pBuff, cstr.m_nSize) : os << std::quoted("nullptr");

}

//////////////////////////////////////////////////////////////////////////

END_NAMESPACE_CXX

//////////////////////////////////////////////////////////////////////////
