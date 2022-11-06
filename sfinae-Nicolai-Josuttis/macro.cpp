#include "macro.h"

BEGIN_NAMESPACE_CCN

std::ostream& operator << (std::ostream& os, const conststr& s) {

    return s.m_pBuff ? os.write(s.m_pBuff, s.m_nSize) : os << "\"nullptr\"";

}

END_NAMESPACE_CCN
