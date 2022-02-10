// tests
/*
	test script to run unit tests for Capstone RDBMS project
*/


#include "pch.h"
#include "CppUnitTest.h"
#include <string>

// headers to test
//#include "../DBMSProject/commandHandler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(Tests)
	{
	public:
		
		TEST_METHOD(testHelpMenu)
		{
			int provided = 0;
			int expected = 1;

			Assert::AreEqual(provided, expected);
		}
	};
}
