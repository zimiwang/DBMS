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

int main(int argc, char** argv);

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

			//main(1, commandStringArray);
			CommandHandler *cmdHandler = new CommandHandler;

			// call the help menu
			int provided = cmdHandler->helpMenu();
			int expected = 1;

			Assert::AreEqual(expected, provided);
		}


		/// <summary>
		/// test the create database script
		/// </summary>
		TEST_METHOD(createDatabase)
		{
			// this test uses the commandHandler header
			CommandHandler* cmdHandler = new CommandHandler;

			Database *exampleDB = NULL;
			string exampleCurrentDBname;

			// call the createDatabase() function
			int provided = cmdHandler->createDatabase(exampleCurrentDBname, exampleDB, "create database exampleDatabaseName;");
			
			// check the new open database name
			string currentDB = cmdHandler->current_db_name;

			string expected = "exampleDatabaseName";

			Assert::AreEqual(expected, currentDB);
		}



		/// <summary>
		/// test open database script
		/// </summary>
		TEST_METHOD(openDatabase)
		{
			// this test uses the commandHandler header
			CommandHandler* cmdHandler = new CommandHandler;

			Database* exampleDB = NULL;
			string exampleCurrentDBname;

			// call the openDatabase() function
			cmdHandler->openDatabase(exampleCurrentDBname, exampleDB, "open database exampleDatabaseName;");

			// check the new open database name
			string currentDB = cmdHandler->current_db_name;

			string expected = "exampleDatabaseName";
				
			Assert::AreEqual(expected, currentDB);
		}



		/// <summary>
		/// test the implementation to drop a database
		/// </summary>
		TEST_METHOD(dropDatabase)
		{
			// this test uses the commandHandler header
			CommandHandler* cmdHandler = new CommandHandler;

			// call the openDatabase() function
			cmdHandler->dropDatabase("drop database exampleDatabaseName;");

			// check the new open database name
			string currentDB = cmdHandler->current_db_name;

			string notExpected = "exampleDatabaseName";

			Assert::AreNotEqual(notExpected, currentDB);
		}
	};
}
