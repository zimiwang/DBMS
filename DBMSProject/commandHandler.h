// commandHandler
/*
	contain handlers for user terminal input
*/

#pragma once
#include "database.h"
#include <string>
#include <sstream>
#include <iostream>

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
		db->delete_column(colname, tablename);
		db->updateRows();
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
		else if (new_cmd.find("key") != -1)
		{
			addKey(db, new_cmd);
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
	/// Indexes the table passed in to it, returns the B+ tree associated with the table.
	/// </summary>
	/// <param name="t">The table you want to index.</param>
	/// <returns>B+ Tree for the table.</returns>
	BPTree index_join(Table t)
	{
		// index the new table
		BPTree newPrimaryKeyIndex;
		newPrimaryKeyIndex.Name = t.table_name;

		for (Row r : t.newrows)
		{
			/*Row* rpoint = &r;*/
			r.InUse();
			for (Column<int> c : r.intColumn)
			{
				//check to see if the colname is the primary key
				if (c.GetName() == t.primaryKeyName)
				{
					//index based on the value here
					newPrimaryKeyIndex.insert(c.GetValue(), r);

					// set primary key
					if (!newPrimaryKeyIndex.HasPrimaryKey()) {
						newPrimaryKeyIndex.SetPrimaryKey(c.GetName());
					}

				}
			}

		}
		t.primaryKeyTree = newPrimaryKeyIndex;
		return newPrimaryKeyIndex;
	}

	/// <summary>
	/// command handler: handler for when "select" is in the command
	/// </summary>
	/// <returns>1 on completion</returns>
	int select(Database * new_db, string new_cmd)
	{
		// should convert to lowercase 
		cmd = new_cmd;
		db = new_db;
		
		// Parses the select command
		try {
			BPTree tree;
			
			// check for join
			bool skipmainprint = false;
			
			// check for sum() function
			if (Utils::contains(cmd, "sum("))
			{
				string columnName = Parser::getSumFunctionColumnName(cmd);
				string sourceTable = Parser::getSumFunctionSourceTableName(cmd);
				bool hasAS = false;

				// run handler function
				float sum = db->sumRows(sourceTable, columnName);

				cout << "(commandHandler.h) sum = " << sum << "\n";

			}
						
			// use if there is a join
			else if (Utils::contains(cmd, "join"))
			{
				std::vector<string> joinparser = Parser::get_join_info(cmd);

				Table t = db->join_table(joinparser[0], joinparser[1], joinparser[2], joinparser[3]);
				tree = index_join(t);

				cout << "Joined: " << joinparser[0] << " with " << joinparser[1] << " as " << t.table_name << endl;

			}
			else if (Utils::contains(cmd, "where"))
			{
				//check to see if this is a multitable operation
				if (Utils::contains(Utils::get_string_between_two_strings(cmd, "from ", " where"), ","))
				{
					std::vector<string> joinparser = Parser::get_join_where_info(cmd);

					Table t = db->join_table(joinparser[0], joinparser[1], joinparser[2], joinparser[3]);
					// index the new table
					tree = index_join(t);

					cout << "Joined: " << joinparser[0] << " with " << joinparser[1] << " as " << t.table_name << endl;
					std::vector<std::string> cols = Parser::get_select_columns(cmd);
					cols = Utils::trimColumns(cols);
					vector<Row> rows = tree.getFullTable();
					rows[0].PrintFullTable(rows, cols);
					skipmainprint = true;
				}
			}
			else if (Utils::contains(cmd, "between")) {
				string tbl_name = Parser::get_table_name(cmd, "from", "where");
				tree = db->get_tree(tbl_name);

			}
			else {
				// use if there is no join
				std::string tbl_name = Parser::get_table_name(cmd, "from", ";");
				cout << "Selecting from Table: " << tbl_name << endl;

				tbl_name = Utils::remove_char(tbl_name, ';');
				tree = db->get_tree(tbl_name);
			}
			if (tree.Name.length() > 0)
			{
				std::vector<std::string> cols = Parser::get_select_columns(cmd);
				cols = Utils::trimColumns(cols);

				std::string conditional = Parser::get_conditional(cmd);

				std::vector<std::string> where_clause = Parser::get_where_clause(cmd, conditional);
				cmd = Utils::remove_char(cmd, ';');
				Dictionary clauses = Parser::get_where_clause(cmd);

				if (skipmainprint == false)
				{
					// decide to print whole table or search table
					if (where_clause.empty()) {

						// print whole table
						vector<Row> rows = tree.getFullTable();
						rows[0].PrintFullTable(rows, cols);
					}
					else {
						// decide to use search based on pk, sk, or full search
						string column = clauses.GetValuesByKey("where")[0];

						// search based on pk
						if (tree.IsPrimaryKey(column)) {
							// search table
							if (Utils::contains(cmd, "between")) {
								SearchOnRange(tree, cols);
							}
							else {
								string pk = clauses.GetValuesByKey("where")[2];
								Row row = tree.search(stoi(pk));
								if (!row.isEmpty()) {
									row.PrintRow(cols, row.GetLargestColumnSize());
								}
								else {
									cout << "Could not find row" << endl;
								}
							}

						}
						// search based on sk
						else if (tree.IsSecondaryKey(column)) {
							if (Utils::contains(cmd, "between")) {
								SearchOnRange(tree, cols);
							}
						}
						// full searh
						else {
							FullSearch(tree, clauses, cols);
						}

					}

				}
				else
				{
					std::cout << "Table does not exist." << std::endl;
				}
			}
		}
		catch (const std::exception& e)
		{
			cout << "An error occured while trying to select a table" << e.what() << endl;
		}
		return 1;
	}

	/// <summary>
	/// Searches through all the tables instead of using a tree
	/// </summary>
	/// <param name="tree">The primary tree table to get all the rows</param>
	/// <param name="clauses"></param>
	/// <param name="cols"></param>
	void FullSearch(BPTree tree, Dictionary clauses, vector<string> cols) {
		vector<Row> rows = tree.getFullTable();
		// get the column type and name
		string colName = clauses.GetValuesByKey("where")[0];
		int type = rows[0].GetColumnType(colName);

		// the value to use to get the row
		string valueToCompare = clauses.GetValuesByKey("where")[2];
		vector<Row> rowsFound;
		bool shouldPrint = true;
		for (Row row : rows) {
			if (type == -1) {
				shouldPrint = false;
			}
			else if (type == 0) {
				Column<string> col = row.GetStringColumnByName(colName);
				// check if column value matches
				if (col.GetValue() == valueToCompare) {
					// print row
					rowsFound.push_back(row);
				}
			}
			else if (type == 1) {
				Column<int> col = row.GetIntColumnByName(colName);
				if (to_string(col.GetValue()) == valueToCompare) {
					rowsFound.push_back(row);
				}
			}
			else if (type == 2) {
				Column<char> col = row.GetCharColumnByName(colName);
				string value;
				value = col.GetValue();
				if (value == valueToCompare) {
					rowsFound.push_back(row);
				}
			}
		}
		// print all of the rows found with the given conditions
		if (shouldPrint) {
			rowsFound[0].PrintFullTable(rowsFound, cols);
		}
		else {
			cout << "Could not find the row" << endl;
		}
	}

	/// <summary>
	/// Searches through a range of values
	/// </summary>
	/// <param name="tree"></param>
	/// <param name="cols"></param>
	void SearchOnRange(BPTree tree, vector<string> cols) {
		// get range values
		string value1 = Utils::get_string_between_two_strings(cmd, "between", "and");
		string value2 = Utils::get_string_between_two_strings(cmd, "and", ";");
		// use range between PK
		vector<Row> rows = tree.searchMultiple(stoi(value1), stoi(value2));
		rows[0].PrintFullTable(rows, cols);
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
		std::cout << "Number of Keys: " << tbl.keys.size() << std::endl;
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
		string conditional = Parser::get_conditional(statement);
		vector<string> clause = Parser::get_where_clause(statement, conditional);

		db->DeleteFrom(tbl_name, conditional, clause);

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
	/// <summary>
	/// Adds a key to the specified table.
	/// </summary>
	/// <param name="new_db"></param>
	/// <param name="new_cmd"></param>
	/// <returns></returns>
	int addKey(Database* new_db, string new_cmd)
	{
		cmd = new_cmd;
		db = new_db;

		std::string tablename = Parser::get_table_name(cmd, "table", "add");
		std::string keytype = Utils::get_string_between_two_strings(cmd, "add", "key");
		std::string keyname = Utils::get_string_between_two_strings(cmd, "key", ";");

		//ensure the key type is valid, then check to see if the key is a column in the table
		if (keytype == "primary" || keytype == "secondary" || keytype == "foreign")
		{
			if (db->get_table(tablename).get_column_index(keyname) != -1)
			{
				db->keytotable(keytype, keyname, tablename);
			}
			else
			{
				//key isn't in the table, no point in adding it
				std::cout << "KEY NOT FOUND IN TABLE." << std::endl;
			}
		}
		else
		{
			//unsupported key type
			std::cout << "KEY NOT PRIMARY, SECONDARY, OR FOREIGN." << std::endl;
		}

		
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
		std::cout << "JOIN		- Used to select from a joined table." << std::endl;
		std::cout << "WHERE		- Used to select from a table joined on a specified column." << std::endl;
		std::cout << "UPDATE TABLE 		- Updates the single column and meta or multiple columns and meta for the given table." << std::endl;
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
