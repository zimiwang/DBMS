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
//#include "../DBMSProject/mainReferenceHeader.h"

#include "../DBMSProject/main.cpp"

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

			char* cmd[] = { (char*)"help" };


			int provided  = main(2, cmd);
			
			int expected = 1;

			Assert::AreEqual(expected, provided);
		}


		/// <summary>
		/// test the create database script
		/// </summary>
		
		TEST_METHOD(createDatabase)
		{

			// command	
			char* cmd[] = { (char*)"create ",  (char*)"database ",  (char*)"exampleDatabaseName;" };
			
			main(4, cmd);

			// this test uses the commandHandler header
			CommandHandler* cmdHandler = new CommandHandler;
			
			// check the new open database name
			string currentDB = current_db_name;

			string expected = "exampleDatabaseName";

			Assert::AreEqual(expected, currentDB);

		}
		



		/// <summary>
		/// test open database script
		/// </summary>		 
		TEST_METHOD(openDatabase)
		{
			
			char* cmd[] = { (char*)"open ",  (char*)"database ",  (char*)"exampleDatabaseName;" };

			main(4, cmd);

			// check the new open database name
			string currentDB = current_db_name;

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
