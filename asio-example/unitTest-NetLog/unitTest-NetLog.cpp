#include "pch.h"
#include "CppUnitTest.h"

#include "./../NetLog/NetLog.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unitTestNetLog
{
    TEST_CLASS(unitTestNetLog)
    {
    public:

        TEST_METHOD(TestAdd)
        {
            Assert::AreEqual(4, olc2::Add(2, 2));
        }

        TEST_METHOD(TestPing)
        {
            olc2::NetLog::instance().Init();
            olc2::NetLog::instance().PingServer();
            olc2::NetLog::instance().FireLogMessage(__LINE__, __FILENAME__, __PRETTY_FUNCTION__);
            olc2::NetLog::instance().Stop();
        }

    };
}
