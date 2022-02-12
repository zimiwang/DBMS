// tests
/*
	test script to run unit tests for Capstone RDBMS project
*/


#include "pch.h"
#include "CppUnitTest.h"
#include <string>

#define _CRT_SECURE_NO_WARNINGS		// error occurred because of security reasons. Needed to add this.

// headers to test
#include "../DBMSProject/commandHandler.h"

using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(Tests)
	{
	public:
		
		/// <summary>
		/// test the help menu
		/// </summary>
		TEST_METHOD(testHelpMenu)
		{
			CommandHandler *cmdHandler = new CommandHandler;

			int provided = cmdHandler->helpMenu();
			int expected = 1;

			Assert::AreEqual(expected, 1);
		}
	};
}
