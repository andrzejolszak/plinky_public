#include "pch.h"
#include "CppUnitTest.h"
#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

extern "C" {
#include "main.h"
}

namespace tests
{
	TEST_CLASS(tests)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{	
			enable_emu_audio = false;
			plinky_init();
			plinky_frame();
			SleepMillis(33);
            plinky_frame();

			Assert::AreEqual(3.32983e+09f, wang_hash(2)/1.f, 10000.f);
		}
	};
}
