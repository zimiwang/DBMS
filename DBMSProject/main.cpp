// main function
/* run this program using the console pauser or add your own getch, system("pause") or input loop */


#define _CRT_SECURE_NO_WARNINGS		// error occurred because of security reasons. Needed to add this.
#include "database.h"
#include <vector>
#include <windows.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include <regex>
#include <iostream>
#include <cerrno>


#include "commandHandler.h"
//#include "mainReferenceHeader.h"

// global variables
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
Table* create_table(std::string table_name, std::vector<pair<std::string, std::string>> columns_info);

std::string current_db_name;
std::string to_lower(std::string str);
std::string table_name;
std::string db_name;
std::string cmd = "";
std::string statement;

Database* read_sql_file(string path);
Database* db = NULL;

int traversed = 0;	// traversed commands in the inputted string (implemented to make testing easier)
int numberSemiColons = 0;

// function initializers
void color(int s);

void setup_intro();

int mainFunct(string cmd);


CommandHandler* cmdHandler = new CommandHandler;


/*	
	these functions actually call and return the values for their sister-functions in commandHander.h 
	the variables for cmd, db, etc. are updated after these functions run, just in case they are important
*/
int exitDBMS() { return cmdHandler->exitDBMS(); }
int helpMenu() { return cmdHandler->helpMenu(); } /* test incorperated in test.cpp */
int noSemiColon() { return cmdHandler->noSemiColon(); }
int openDatabase() { int retVal = cmdHandler->openDatabase(current_db_name, db, cmd); current_db_name = cmdHandler->current_db_name; db = cmdHandler->db; return retVal;}/* test incorperated in tests.cpp */
int createDatabase() { int retVal = cmdHandler->createDatabase(current_db_name, db, cmd); current_db_name = cmdHandler->current_db_name; db = cmdHandler->db; return retVal; }/* test incorperated in tests.cpp */
int listDatabases() { int retVal = cmdHandler->listDatabases(); return retVal; }
int loadSQLfile() { int retVal = cmdHandler->loadSQLfile(db, current_db_name); current_db_name = cmdHandler->current_db_name; db = cmdHandler->db; return retVal; }
int dropDatabase() { int retVal = cmdHandler->dropDatabase(cmd); current_db_name = cmdHandler->current_db_name; return retVal; } /* test incorperated in tests.cpp*/
int noDBopen() { int retVal = cmdHandler->noDBopen(); return retVal; }
int listTables() { int retVal = cmdHandler->listTables(db);  db = cmdHandler->db; return retVal; }
int dbInfo() { int retVal = cmdHandler->dbInfo(db); return retVal; }
int select() { int retVal = cmdHandler->select(db, cmd); current_db_name = cmdHandler->current_db_name; db = cmdHandler->db; return retVal;} //TODO ***************** untested ************************
int createTable() { int retVal = cmdHandler->createTable(db, cmd, table_name); table_name = cmdHandler->table_name; db = cmdHandler->db; return retVal; }
int insertInto() { int retVal = cmdHandler->insertInto(table_name, db, statement, cmd); table_name = cmdHandler->table_name; db = cmdHandler->db; return retVal; }  //tested, should work
int tableInfo() { int retVal = cmdHandler->tableInfo(db, cmd, table_name); return retVal; }
int dropTable() { int retVal = cmdHandler->dropTable(db, cmd); db = cmdHandler->db; return retVal; }
int update() { int retVal = cmdHandler->update(db, cmd); db = cmdHandler->db; return retVal; } //TODO ***************** untested ************************
int deleteFrom() { int retVal = cmdHandler->deleteFrom(db, statement, cmd); db = cmdHandler->db; return retVal; } //TODO ***************** untested ************************
int renameTable() { int retVal = cmdHandler->renameTable(db, cmd); db = cmdHandler->db; return retVal; }
int renameColumn() { int retVal = cmdHandler->renameColumn(db, cmd); db = cmdHandler->db; return retVal; }
int alterHandler() { int retVal = cmdHandler->alterHandler(cmd); db = cmdHandler->db; return retVal; }
int dropColumn() { int retVal = cmdHandler->dropColumn(cmd); db = cmdHandler->db; return retVal; }





/*
// short script for testing the unit tests' commands
// basically, you just rename main() to mainT() and follow the example below to parse in a command array and array size
int mainT(int argc, char** argv);
int main() {
			char* cmdCreateDb[] = { (char*)"create ",  (char*)"database ",  (char*)"exampleDatabaseName;", (char*)"create ",  (char*)"table ", (char*)"tableExample(char a, char b, char c);" };
			mainT(8, cmdCreateDb);

               return 1;
}
*/


/// <summary>
/// main function
/// </summary>
/// <param name="argc">number of inputs to function call</param>
/// <param name="argv">inputs to functions stored as string array (char**)</param>
/// <returns></returns>
int main(int argc, char** argv)
{

	// function for "setup", this is simply the opening animation sequence
	setup_intro();


	while (Parser::to_lower(cmd) != "exit")
	{

		// Setup the command to wait for input
		color(10);
		if (current_db_name.length() > 0)
		{
			std::cout << current_db_name << "@";
		}
		std::cout << "SQL>";
		color(7);


		cmd = "";

		// if there are no inputs, then use the stdin for user control
		if ( argc == 1 )	std::getline(std::cin, cmd);

		// if there are inputs, use the argc[] string array for inputs
		else
		{
			cmd = string(argv[traversed]);
			traversed++;
			for (int i = traversed; i < argc - 1; i++) {
				cout << "Reading inputted command from creation terminal...\n";
				
				cmd += string(argv[i]);

				// if a semicolon is found, 
				if (cmd[(cmd.length()) - 1] == ';') {
					numberSemiColons++;
					// if there are no inputted commands past this command, exit the dbms
					if (i >= (argc - numberSemiColons) - 1) { traversed = 1;		argc = 2;	argv[1] = (char*)"exit"; }
					else								 {	traversed = i + 1;	break;	}
					i = traversed;
				}
			}
		}


		statement = Parser::to_lower(cmd);



		/// <summary>
		/// list of sql commands and references to the functions they call
		/// </summary>
		/// <param name="argc">command name</param>
		/// <param name="argv">function reference</param>
		/// <returns></returns>
		//std::template <typename T>
		map<string, int (*)() > sqlCommands;
		sqlCommands = {
			{ "exit", &exitDBMS, },
			{ "help", &helpMenu, },
			{ "noSemiColon", &noSemiColon, },
			{ "openDatabase", &openDatabase, },
			{ "createDatabase", &createDatabase, },
			{ "listDatabases", &listDatabases, },
			{ "loadSQLfile", &loadSQLfile, },
			{ "dropDatabase", &dropDatabase, },
			{ "noDBopen", &noDBopen, },
			{ "listTables", &listTables,},
			{ "dbInfo",&dbInfo, },
			{ "select", &select, },
			{ "createTable", &createTable, },
			{ "insertInto", &insertInto, },
			{ "tableInfo", &tableInfo, },
			{ "dropTable", &dropTable, },
			{ "update", &update, },
			{ "deleteFrom", &deleteFrom, },
			{ "renameTable", &renameTable, },
			{ "renameColumn", & renameColumn, },
			{ "alterHandl", & alterHandler, },
			{ "dropCol", & dropColumn}
			
		};


		// Display all of the mapped functions
		std::map<string, int(*)()>::const_iterator it = sqlCommands.begin();
		std::map<string, int(*)()>::const_iterator end = sqlCommands.end();

		// condition checking. If true, execute the function pointer (located in sqlCommands)
		if (statement == "")								cout << "";
		else if (statement == "exit")							(*sqlCommands.find("exit")).second();
		else if (statement == "help")						(*sqlCommands.find("help")).second();
		else if (statement.back() != ';')					(*sqlCommands.find("noSemiColon")).second();
		else if (statement.find("open database ") == 0)		(*sqlCommands.find("openDatabase")).second();
		else if (statement.find("create database") == 0)	(*sqlCommands.find("createDatabase")).second();
		else if (statement == "list databases;")			(*sqlCommands.find("listDatabases")).second();
		else if (statement.find("load sqlfile ") == 0)		(*sqlCommands.find("loadSQLfile")).second();
		else if (statement.find("drop database ") == 0)		(*sqlCommands.find("dropDatabase")).second();
		else if (current_db_name.length() == 0)				(*sqlCommands.find("noDBopen")).second();
		else if (statement == "list tables;")				(*sqlCommands.find("listTables")).second();
		else if (statement == "db info;")					(*sqlCommands.find("dbInfo")).second();
		else if (statement.find("select ") == 0)			(*sqlCommands.find("select")).second();
		else if (statement.find("create table ") == 0)		(*sqlCommands.find("createTable")).second();
		else if (statement.find("insert into") == 0)		(*sqlCommands.find("insertInto")).second();
		else if (statement.find("table info ") == 0)		(*sqlCommands.find("tableInfo")).second();
		else if (statement.find("drop table ") == 0)		(*sqlCommands.find("dropTable")).second();
		else if (statement.find("update ") == 0)			(*sqlCommands.find("update")).second();
		else if (statement.find("delete from ") == 0)		(*sqlCommands.find("deleteFrom")).second();
		else if (statement.find("rename table ") == 0)		(*sqlCommands.find("renameTable")).second();
		else if (statement.find("rename column ") == 0)		(*sqlCommands.find("renameColumn")).second();
		else if (statement.find("alter ") == 0)             (*sqlCommands.find("alterHandl")).second();
		else if (statement.find("drop column ") == 0)       (*sqlCommands.find("dropCol")).second();

		
		else												std::cout << "Invalid Command." << std::endl;

	}

	return 1;
}
// end of main function




///Author: Andrew Nunez
///Sets the console output color
void color(int s)
{
	SetConsoleTextAttribute(h, s);
}



///Janita Aamir
///This function is used within create table. It checks to see if the
///database selected has any special characters that aren't allowed.
bool has_special_char(std::string const& s)
{
	for (int i = 0; i < s.length(); i++)
	{
		if (!std::isalpha(s[i]) && !std::isdigit(s[i]) && s[i] != '_')
			return false;
	}
}



/// <summary>
/// Setups the intro, emulating a startup sequence... we can probably have it set to actually do something interesting
/// </summary>
void setup_intro()
{
	std::cout << "ISU RDBMS Project" << std::endl;
	std::cout << "Opening RDBMS Shell.";
	Sleep(400);
	std::cout << ".";
	Sleep(400);
	std::cout << ".";
	Sleep(400);
	std::cout << ".";
	Sleep(400);
	std::cout << ".";
	Sleep(400);
	std::cout << ".";

	std::cout << std::endl
		<< std::endl
		<< "Success! Here is your shell." << std::endl
		<< "Type [help] for a list of commands. Type [exit] to quit." << std::endl
		<< std::endl;
}