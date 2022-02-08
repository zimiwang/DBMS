// unitTests header file
/*
*	contins Unit tests for various functions
*/

#pragma once

#include <windows.h>
#include <iostream>
#include <string>



// handle for text color changing
HANDLE o = GetStdHandle(STD_OUTPUT_HANDLE);

// global variables
int errors = 0;

// function declarations
int testsStartBanner();
int testsEndBanner();
int testHelpMenu();


class UnitTests
{

public:

	/// <summary>
	/// function to actually run all the tests
	/// </summary>
	/// <returns>1 on completion</returns>
	int test() {
		testsStartBanner();

		testHelpMenu();
	
		
		testsEndBanner();
		return 1;
	}

	

};


/// <summary>
/// test the implementation to call the help menu. If it fails, increment the error count and output the error
/// </summary>
/// <returns>1 on success, 0 on failure</returns>
int testHelpMenu()
{
	// call help menu from command
	std::string testCommand = "HELP";
	std::string statement = Parser::to_lower(testCommand);
	
	if (statement != "help") { std::cout << "***error*** \t Parser::to_lower(testCommand) failed\n"; errors++; }
	if (statement == "help")
		return 1;
		//(*sqlCommands.find("help")).second();

	


	return 1;
}



/// <summary>
/// simple banner to indicate that tests have started
/// </summary>
/// <returns>1 on completion</returns>
int testsStartBanner()
{
	std::cout << "Beginning Unit Tests . . .\nPlease do not cancel this process until it is completed\n";
	
	return 1;
}



/// <summary>
/// simple banner to indicate that tests have ended
/// </summary>
/// <returns>1 on completion</returns>
int testsEndBanner()
{
	std::cout << "Unit tests ended with ";
	
	if ( errors > 0 )
	{
		// print the number in red
		SetConsoleTextAttribute(o, 4);
	
		std::cout << errors;
		SetConsoleTextAttribute(o, 7);

	}
	else
	{
		// print the number in green
		SetConsoleTextAttribute(o, 10);
		std::cout << errors;
		

		SetConsoleTextAttribute(o, 7);
	}

	std::cout << " errors \n\n";
	
	return 1;
}




