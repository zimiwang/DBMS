

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

// function initializers
void color(int s);

void setup_intro();

/*
void show_help();
void print_rows(Table tbl);
void table_info(Table tbl);
void insert_into(Database* db, vector<string> split_commands);
void drop_table(Database* db, Table* tbl);
void drop_database(string db_name);
bool has_special_char(std::string const& str);
void update_table(Database* db, std::string table_name, std::string col1, std::string toUpdate, std::string col2, std::string forVariable);
*/

CommandHandler* cmdHandler = new CommandHandler;


/*	
	these functions actually call and return the values for their sister-functions in commandHander.h 
	the variables for cmd, db, etc. are updated after these functions run, just in case they are important
*/
int exitDBMS() { return cmdHandler->exitDBMS(); }
int helpMenu() { return cmdHandler->helpMenu(); }
int noSemiColon() { return cmdHandler->noSemiColon(); }
int openDatabase() { int retVal = cmdHandler->openDatabase(current_db_name, db, cmd); current_db_name = cmdHandler->current_db_name; db = cmdHandler->db; return retVal;}
int createDatabase() { int retVal = cmdHandler->createDatabase(current_db_name, db, cmd); current_db_name = cmdHandler->current_db_name; db = cmdHandler->db; return retVal; }
int listDatabases() { int retVal = cmdHandler->listDatabases(); return retVal; }
int loadSQLfile() { int retVal = cmdHandler->loadSQLfile(db, current_db_name); current_db_name = cmdHandler->current_db_name; db = cmdHandler->db; return retVal; }
int dropDatabase() { int retVal = cmdHandler->dropDatabase(cmd); current_db_name = cmdHandler->current_db_name; return retVal; }
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




int main(int argc, char** argv)
{



	setup_intro();


	while (Parser::to_lower(cmd) != "exit")
	{
		cmd = "";


		// Setup the command to wait for input
		color(10);
		if (current_db_name.length() > 0)
		{
			std::cout << current_db_name << "@";
		}

		std::cout << "SQL>";
		color(7);
		std::getline(std::cin, cmd);
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
			{ "deleteFrom", &deleteFrom, }
			
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
		
		else												std::cout << "Invalid Command." << std::endl;


		/*
		if (statement == "exit")
		{
			std::cout << "Good Bye" << std::endl;
		}

		else if (statement == "help")
		{
			show_help();

		}

		else if (statement.back() != ';') {
			std::cout << "SQL command not properly terminated." << std::endl;
		}

		else if (statement.find("open database ") == 0) {
			//current_db_name = statement.substr(statement.find_last_of(' ') + 1, statement.find_last_of(';') - statement.find_last_of(' ') - 1);
			current_db_name = Utils::trim(Utils::get_string_between_two_strings(cmd, "database ", ";"));
			db = new Database(current_db_name);

			if (db->database_name != current_db_name) {
				current_db_name = "";
			}

		}
		else if (statement.find("create database") == 0) {
			current_db_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
			db = new Database();
			db->database_name = current_db_name;
			db->Save();

		}
		else if (statement == "list databases;") {
			Database::List();
		}
		else if (statement.find("load sqlfile ") == 0) {
			string target_file_path = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
			db = read_sql_file(target_file_path);

			current_db_name = db->database_name;

			cout << "Database Created: " << current_db_name << endl;
		}
		else if (statement.find("drop database ") == 0) {
			string db_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
			//db = new Database(current_db_name);
			drop_database(db_name);
		}
		else if (current_db_name.length() == 0) {
			std::cout << "Open a database first." << std::endl;
		}
		else if (statement == "list tables;") {
			db->List_Tables();

		}
		else if (statement == "db info;") {
			db->List_Info();
		}
		else if (statement.find("select ") == 0) {
			// Parses the select command
			try {

				//std::string tbl_name = cmd.substr(statement.find(" from") + 6);
				std::string tbl_name = Parser::get_table_name(cmd, "from", "where");

				cout << "Selecting from Table: " << tbl_name << endl;

				if (tbl_name.length() == 0) {
					tbl_name = Parser::get_table_name(cmd, "from", ";");
				}

				tbl_name = Utils::remove_char(tbl_name, ';');

				Table tbl = db->get_table(tbl_name);

				if (tbl.table_name.length() > 0)
				{
					std::vector<std::string> cols = Parser::get_select_columns(cmd);
					std::string conditional = Parser::get_conditional(cmd);
					std::vector<std::string> where_clause = Parser::get_where_clause(cmd, conditional);
					tbl.Print_Rows(cols, where_clause, conditional);
				}
				else
				{
					std::cout << "Table does not exist." << std::endl;
				}
			}
			catch (const std::exception&)
			{
			}
		}
		else if (statement.find("create table ") == 0) {
			table_name = Parser::get_table_name(cmd, "table", "(");
			vector<string> cols = Parser::get_create_columns(cmd);
			Table* tbl = new Table(table_name, cols);
			db->AddTable(*tbl);
		}
		else if (statement.find("insert into") == 0) {
			table_name = Parser::get_table_name(statement, "into", "(");
			db->insert_into(cmd, table_name);
			db->Save();
		}
		else if (statement.find("table info ") == 0) {

			table_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
			Table tbl = db->get_table(table_name);
			table_info(tbl);
		}
		else if (statement.find("drop table ") == 0) {
			string table_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
			db->DropTable(table_name);
			db->Save();
		}
		else if (statement.find("update ") == 0) {
			std::string table_name = Parser::get_table_name(cmd, "update", "set");
			vector<string> upd_clause = Parser::get_update_clause(cmd);
			vector<string> where_clause = Parser::get_where_clause(cmd, "=");

			db->UpdateTable(table_name, upd_clause, where_clause);

		}
		else if (statement.find("delete from ") == 0) {
			string tbl_name = Utils::get_string_between_two_strings(cmd, "from ", " where");

			int count = 0;
			string conditional = Parser::get_conditional(statement);
			vector<string> clause = Parser::get_where_clause(statement, conditional);
			string value = clause[1];
			Table currentTable = db->get_table(tbl_name);
			int col_ndx = currentTable.get_column_index(clause[0]);
			vector<vector<string> > rows = currentTable.rows;
			for (vector<string> row : rows) {
				cout << row[col_ndx] << conditional << value << endl;

				if (conditional == "=") {
					if (row[col_ndx] == value)
					{
						currentTable.DeleteRow(row);
						count += 1;
					}
				}
				else if (conditional == ">=") {
					if (row[col_ndx] >= value)
					{
						currentTable.DeleteRow(row);
						count += 1;
					}
				}
				else if (conditional == "<=") {
					if (row[col_ndx] <= value)
					{
						currentTable.DeleteRow(row);
						count += 1;
					}
				}
				else if (conditional == ">") {
					if (row[col_ndx] > value)
					{
						currentTable.DeleteRow(row);
						count += 1;
					}
				}
				else if (conditional == "<") {
					if (row[col_ndx] < value)
					{
						currentTable.DeleteRow(row);
						count += 1;
					}
				}
				else if (conditional == "!=") {
					if (row[col_ndx] != value)
					{
						currentTable.DeleteRow(row);
						count += 1;
					}
				}
				else {
					std::cout << "Given conditional statement is not supported!" << std::endl;
				}
			}

			db->SaveTable(currentTable);

			db->Save();

			std::cout << count << " rows deleted." << endl;
		}
		else
		{
			std::cout << "Invalid Command." << std::endl;
		}
		*/
	}

	return 0;
}
// end of main function




///Author: Andrew Nunez
///Sets the console output color
void color(int s)
{
	SetConsoleTextAttribute(h, s);
}





/*
///Author: Janita Aamir
	///This function updates an existing value with a new one given the column names and specific row.
void update_table(Database* db, std::string table_name, std::string col1, std::string toUpdate, std::string col2, std::string forVariable) {
	Table tbl = db->get_table(table_name);

	int col1Index = tbl.get_column_index(col1);
	int col2Index = tbl.get_column_index(col2);

	std::vector<std::string>::const_iterator col;
	std::vector<std::vector<std::string> > rows = tbl.rows;


	for (std::vector<std::string> row : rows) {
		if (row[col2Index] == forVariable) {
			row[col1Index] = toUpdate;
		}

	}


}
*/


/*
/// Author: Janita Aamir
/// Creates a table with given column info
Table* create_table(std::string table_name, std::vector<pair<std::string, std::string>> columns_info)
{
	Table* tbl = new Table(table_name);
	for (int i = 0; i < columns_info.size(); i++)
	{
		tbl->columns.insert({ columns_info[i].first, columns_info[i].second });
	}
	return tbl;
}
*/


/*
///Author: Janita Aamir
///This function drops the given table from the current database.
void drop_table(Database* db, Table* tbl) {
	tbl->Delete();

	for (std::vector<Table>::iterator it = db->tables.begin(); it != db->tables.end(); ++it)
	{
		if (it->table_name == tbl->table_name)
		{
			db->tables.erase(it);
			break;
		}
	}
	db->Save();
}
*/




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







/// Author: Andrew Nunez

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

