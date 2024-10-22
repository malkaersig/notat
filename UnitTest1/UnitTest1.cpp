#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

bool func()
{
	int gay = 23532423 + 234234234;
	return true;
}

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(UnitMethod1)
		{
			
			Assert::AreEqual(true, true);
		}
	};
}
