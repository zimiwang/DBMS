// commandHandler
/*
	contain handlers for user terminal input
*/

#pragma once
#include "database.h"

using namespace std;

class CommandHandler
{
public:

	std::string current_db_name;
	Database* db;
	std::string table_name;
	std::string cmd;
	std::string statement;

	

	/// <summary>
	/// command handker: exit the dbms--this is fairly self-explanatary
	/// </summary>
	/// <returns>1 on execution</returns>
	int exitDBMS()
	{
		// save only if a db is open
		if( !current_db_name.empty())
			db->Save();
	
		std::cout << "Good Bye user\n";
		return 1;
	}



	/// <summary>
	/// command handler: display the help menu
	/// </summary>
	/// <returns>1 on execution</returns>
	int helpMenu()
	{
		show_help();
		return 1;
	}



	/// <summary>
	/// command handler: when there is no ';' terminating character
	/// </summary>
	/// <returns>1 on text prompt execution</returns>
	int noSemiColon()
	{
		std::cout << "SQL command not properly terminated." << std::endl;
		return 1;
	}



	/// <summary>
	/// create a new database
	/// </summary>
	/// <param name="current_db_name">Database, reference to the current database</param>
	/// <param name="db">string name of the current database</param>
	/// <param name="cmd">command string</param>
	/// <returns>1 on completion</returns>
	int createDatabase(string new_current_db_name, Database *new_db, string new_cmd )
	{
		current_db_name = new_current_db_name;
		db = new_db;
		cmd = new_cmd;

		current_db_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
		db = new Database();
		db->database_name = current_db_name;
		db->Save();
		return 1;
	}



	/// <summary>
	/// open an existing database
	/// </summary>
	/// <param name="current_db_name">name of the current database</param>
	/// <param name="db">reference to the current database</param>
	/// <param name="cmd">the command string</param>
	/// <returns>um....</returns>
	int openDatabase(string new_current_db_name, Database *new_db, string new_cmd)
	{
		current_db_name = new_current_db_name;
		db = new_db;
		cmd = new_cmd;
		//current_db_name = statement.substr(statement.find_last_of(' ') + 1, statement.find_last_of(';') - statement.find_last_of(' ') - 1);

		current_db_name = Utils::trim(Utils::get_string_between_two_strings(cmd, "database ", ";"));
		db = new Database(current_db_name);

		if (db->database_name != current_db_name) {
			current_db_name = "";
		}

		return 1;
	}




	/// <summary>
	/// command handler: load a named sql file
	/// </summary>
	/// <returns>1 on completion</returns>
	int loadSQLfile(Database *new_db, string new_current_db_name)
	{
		db = new_db;
		current_db_name = new_current_db_name;

		string target_file_path = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
		db = read_sql_file(target_file_path);

		current_db_name = db->database_name;

		cout << "Database Created: " << current_db_name << endl;
		return 1;
	}



	/// Author: Saurav Gautam, Andrew Nunez
/// Loads the sql file
/// Parses the data
/// Runs the queries
/// Saves the database
	Database* read_sql_file(string path)
	{
		ifstream infile("data/" + path, std::ios::in | std::ios::binary);
		string file_contents{ istreambuf_iterator<char>(infile), istreambuf_iterator<char>() };
		file_contents.erase(std::remove(file_contents.begin(), file_contents.end(), '\n'), file_contents.end());

		vector<string> commands = split_text(file_contents, ";");
		string table_name;
		string current_db_name;
		Database* db = new Database();

		for (string statement : commands)
		{
			string statement_lowercase = Parser::to_lower(Utils::trim(statement));

			if (statement_lowercase.find("create database") == 0)
			{
				current_db_name = statement.substr(statement.find_last_of(' ') + 1, statement.find_last_of(';') - statement.find_last_of(' ') - 1);
				db->database_name = current_db_name;
			}
			else if (statement_lowercase.find("create table ") == 0)
			{
				table_name = Parser::get_table_name(statement, "table", "(");
				cout << "Table Created: " << table_name << endl;
				vector<string> cols = Parser::get_create_columns(statement);
				Table* tbl = new Table(table_name, cols);
				db->AddTable(*tbl);
			}
			else if (statement_lowercase.find("insert into") == 0)
			{
				table_name = Parser::get_table_name(statement, "into", "(");
				db->insert_into(statement, table_name);
				db->Save();
			}
		}
		db->Save();
		return db;
	}





	/// <summary>
	/// command handler: drop a named database
	/// </summary>
	/// <returns>1 on completion</returns>
	int dropDatabase(string new_cmd)
	{
		cmd = new_cmd;
		string db_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
		//db = new Database(current_db_name);
		drop_database(db_name);
		return 1;
	}

	/// <summary>
	/// command handler: drop a named column from a table
	/// </summary>
	/// <returns>1 on completion</returns>
	int dropColumn(string new_cmd)
	{
		cmd = new_cmd;
		string colname = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
		string tablename = cmd.substr(cmd.find("table") + 6, ((cmd.find("drop")) - (cmd.find("table") + 6)) - 1);
		this->db->delete_column(colname, tablename);
		return 1;
	}

	/// <summary>
	/// alter keyword command handler
	/// </summary>
	/// <returns>1 on completion</returns>
	int alterHandler(string new_cmd)
	{
		cmd = new_cmd;
		if (new_cmd.find("rename") != -1)
		{
			renameColumn(db, new_cmd);
		}
		else if (new_cmd.find("drop column") != -1)
		{
			dropColumn(new_cmd);
		}

		return -1;
	}



	/// <summary>
	/// command handler: give some feedback when there is no db open, but the user tries to do some db command
	/// </summary>
	/// <returns>1 on completion</returns>
	int noDBopen()
	{
		std::cout << "Open a database first." << std::endl;
		return 1;
	}



	/// <summary>
	/// command handler: list the tables
	/// </summary>
	/// <returns>1 on completion</returns>
	int listTables(Database *new_db)
	{
		db = new_db;
		db->List_Tables();
		return 1;
	}




	/// <summary>
	/// command handler: give information on the active db
	/// </summary>
	/// <returns>1 on completion</returns>
	int dbInfo(Database *new_db)
	{
		db = new_db;
		db->List_Info();
		return 1;
	}



	/// <summary>
	/// command handler: handler for when "select" is in the command
	/// </summary>
	/// <returns>1 on completion</returns>
	int select(Database * new_db, string new_cmd)
	{
		cmd = new_cmd;
		db = new_db;

		// Parses the select command
		try {

			std::string tbl_name = Parser::get_table_name(cmd, "from", ";");
			cout << "Selecting from Table: " << tbl_name << endl;

			tbl_name = Utils::remove_char(tbl_name, ';');

			Table tbl = db->get_table(tbl_name);

			if (tbl.table_name.length() > 0)				// Why look for table name length? Could use if table is null instead?
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
		return 1;
	}



	/// <summary>
	/// command handler: create a new table
	/// </summary>
	/// <returns>1 on completion</returns>
	int createTable(Database *new_db, string new_cmd, string new_table_name)
	{
		cmd = new_cmd;
		table_name = new_table_name;
		db = new_db;

		
		// create a new table by parsing the inputted command
		table_name = Parser::get_table_name(cmd, "table", "(");
		vector<string> cols = Parser::get_create_columns(cmd);
		Table* tbl = new Table(table_name, cols);

		// add the table
		db->AddTable(*tbl);
		return 1;
	}




	/// <summary>
	/// command handler: insert table information into the db
	/// </summary>
	/// <returns>1 on completion</returns>
	int insertInto(string new_table_name, Database* new_db, string new_statement, string new_cmd)
	{
		table_name = new_table_name;
		db = new_db;
		statement = new_statement;

		// create a new table by parsing data from the statement
		table_name = Parser::get_table_name(statement, "into", "(");

		db->insert_into(new_cmd, table_name);
		db->Save();
		return 1;

		table_name = Parser::get_table_name(statement, "into", "(");
		db->insert_into(new_cmd, table_name);
		db->Save();
	}



	/// <summary>
	/// command handler: print the table info
	/// </summary>
	/// <returns>1 on completion</returns>
	int tableInfo(Database *new_db, string new_cmd, string new_table_name)
	{
		cmd = new_cmd;
		table_name = new_table_name;
		db = new_db;

		table_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
		Table tbl = db->get_table(table_name);
		table_info(tbl);

		return 1;
	}



	/// Author: Janita Aamir, Saurav Gautam
	/// Shows the given table information
	/// Table Name, Column Names, Number of Rows
	void table_info(Table tbl)
	{
		std::cout << "Table name: " << tbl.table_name << std::endl;
		std::cout << "----------------------------- " << std::endl;
		std::cout << "Column names: " << std::endl;
		std::vector<std::string> it = tbl.get_column_names();

		for (int i = 0; i < it.size(); i++)
		{
			cout << "\t"
				<< "-" << it.at(i) << '\n';
		}

		std::cout << "----------------------------- " << std::endl;
		std::cout << "Number of Rows: " << tbl.rows.size() << std::endl;
	}


	/// <summary>
	/// command handler: drop some identified table
	/// </summary>
	/// <returns>1 on completion</returns>
	int dropTable(Database *new_db, string new_cmd)
	{
		cmd = new_cmd;
		db = new_db;

		string table_name = cmd.substr(cmd.find_last_of(' ') + 1, cmd.find_last_of(';') - cmd.find_last_of(' ') - 1);
		db->DropTable(table_name);
		db->Save();
		return 1;
	}



	/// <summary>
	/// command handler: update the db 
	/// </summary>
	/// <returns>` on completion</returns>
	int update(Database *new_db, string new_cmd)
	{
		cmd = new_cmd;
		db = new_db;

		// get table name by sending command through parser
		//std::string table_name = Parser::get_table_name(cmd, "update", "set");
		//vector<string> upd_clause = Parser::get_update_clause(cmd);
		//vector<string> where_clause = Parser::get_where_clause(cmd, "=");


		std::string table_name = Parser::get_table_name(cmd, "update", "set");
		vector<vector<string>> update_clause = Parser::get_update_clauses(cmd);
		vector<string> where_clause = Parser::get_where_clause(cmd, "=");

		db->UpdateTable(table_name, update_clause, where_clause);
		return 1;
	}



	/// <summary>
/// command handler: delete rows in a table
/// </summary>
/// <returns>1 on completion</returns>
	int deleteFrom(Database *new_db, string new_statement, string new_cmd)
	{
		cmd = new_cmd;
		statement = new_statement;
		db = new_db;

	
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

		return 1;
	}






	///Author: Janita Aamir
///This function drops the given database from the dbms.
	void drop_database(string db_name)
	{
		// identify the string to wipe
		std::string s = "data/" + db_name + ".db";

		// remove the file
		if (std::remove(s.c_str()) != 0)
		{

			perror("Error deleting file");
		}
		else
		{
			puts("File successfully deleted");
			current_db_name = "";
		}


	}



	/// <summary>
	/// command handler: list the databases
	/// </summary>
	/// <returns>1 on completion</returns>
	int listDatabases()
	{
		Database::List();
		return 1;
	}

	/// <summary>
	/// command handler: modified an existing table name
	/// </summary>
	/// <returns>1 on completion</returns>
	//rename table [old table name] to [new table name];
	int renameTable(Database* new_db, string new_cmd) 
	{
		cmd = new_cmd;
		db = new_db;

		// get old table name by sending command through parser
		std::string table_name = Parser::get_table_name(cmd, "table", "to");
		std::string new_table_name = Utils::get_string_between_two_strings(cmd, "to", ";");
		
		db->RenameTable(table_name, new_table_name);
		db->Save();

		return 1;
	}

	/// <summary>
	/// command handler: modified an existing column in a table
	/// </summary>
	/// <param name="new_db"></param>
	/// <param name="new_cmd"></param>
	/// <returns></returns>
	int renameColumn(Database* new_db, string new_cmd)
	{
		cmd = new_cmd;
		db = new_db;

		// get old table name by sending command through parser
		std::string table_name = Parser::get_table_name(cmd, "alter", "rename");

		// get old column name by sending command through parser
		std::string old_column_name = Utils::get_string_between_two_strings(cmd, "column", "to");
		std::string new_column_name = Utils::get_string_between_two_strings(cmd, "to", ";");

		db->RenameColumn(old_column_name, new_column_name, table_name);

		return 1;
	}

	/// Author: Andrew Nunez

	/// <summary>
	/// output the help menu
	/// </summary>
	void show_help()
	{
		std::cout << "Available Commands:" << std::endl;
		std::cout << "OPEN DATABASE [NAME] 	- Check if the database exists and open it." << std::endl;
		std::cout << "CREATE DATABASE [NAME]	- Creates a new database under the provided name and opens a connection to it." << std::endl;
		std::cout << "DB INFO 		- Lists the database name and prints table information." << std::endl;	// calls List_Info()
		// * removed due to redundancy * // std::cout << "DROP DATABASE 		- Deletes the given database." << std::endl;
		std::cout << "CREATE TABLE 		- Creates a table in the current database." << std::endl;
		std::cout << "DROP TABLE [NAME] 	- Creates a table in the current database." << std::endl;
		std::cout << "DROP DATABASE [NAME]		- Check if the database exists and drop it." << std::endl;
		std::cout << "SELECT [] FROM [] 	- Selects the specified columns from the table." << std::endl;
		std::cout << "UPDATE TABLE 		- Updates the columns and meta for the given table." << std::endl;
		std::cout << "DELETE FROM 		- Deletes the sepcified data from the table." << std::endl;
		std::cout << "INSERT INTO 		- Inserts the data into the table. (In Testing))" << std::endl;
		std::cout << "LIST DATABASES 		- Lists the current database names." << std::endl;
		std::cout << "LIST TABLES 		- Lists the current database names." << std::endl;
		std::cout << "TABLE INFO [name] 	- Lists the current database names." << std::endl;
		std::cout << "RENAME TABLE		- Modifies an existing table name" << std::endl;
		std::cout << "RENAME COLUMN		- Modifies an existing column name in a table" << std::endl;
		std::cout << "ALTER TABLE		- Used to modify table, I.E. DROP COLUMN {name}." << std::endl;
	}



};
