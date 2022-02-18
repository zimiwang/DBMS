/*
*	File: 	    database.h
*   Author:     Andrew Nunez, Janita Aamir, Saurav Gautam, Sanjeev Thakur
*   Date:       Sept. 23, 2021
*
*   This file holds a class that defines a database and it's data to read and write from a .bin file.
*/
#pragma once
#include <algorithm>
#include "filehelper.h"
#include "table.h"
#include "bplustree.h"
#include <stdbool.h>
#include"row.h"
#include <cstring>
#include<iostream>
#include<string>
class Database
{
private:
	void Read(std::string db_name);

public:
	
	/// <summary>
	/// The name of the database 
	/// </summary>
	std::string database_name;

	/// <summary>
	/// The tables associated to <database_name>
	/// </summary>
	std::vector<Table> tables;

	std::vector<BPTree> trees;

	const string PRIMARY_KEY = "ID";


	static void List();
	void List_Tables();
	void AddTable(Table& tbl);
	void Delete();
	void Save();
	void SaveTable(Table table);
	void DropTable(std::string name);
	void CreateTable(std::string tableName);
	bool find_table(std::string name);
	void insert_into(std::string statement, std::string table_name);
	void List_Info();
	Table get_table(std::string tbl_name);
	void UpdateTable(string table_name, vector<string> update_clause, vector<string> where_clause);
	void RenameTable(std::string old_table_name, std::string new_table_name);
	void RenameColumn(std::string old_column_name, std::string new_column_name, std::string table_name);
	void delete_column(std::string column_name, std::string table_name);
	void updateRows();
	void updatePrimaryTrees();

	
	/// <summary>
	/// constructor for empty database
	/// </summary>
	Database()
	{}

	
	/// <summary>
	/// Database contructor
	/// </summary>
	/// <param name="name">name for the new database</param>
	Database(std::string name)
	{
		Read(name);
	}
};


/// <summary>
/// delete the current database
/// </summary>
/// TODO: delete the associated .db file
void Database::Delete()
{
	// remove file
	delete this;
}


/// Author: Andrew Nunez
/// <summary>
/// Save the state of the current database to disk
/// </summary>
void Database::Save()
{
	std::string line;
	std::string contents;
	std::ofstream out("data/" + database_name + ".db");

	contents = "database:" + database_name + "\n";

	auto table = tables.begin();

	for (; table != tables.end(); table++)
	{
		contents += ("table_name:" + table->table_name);

		// Add keys
		if (table->keys.size() > 0)
		{
			contents += "\nkeys:";
			for (auto const& key : table->keys)
			{
				contents += (key.first + " " + key.second + ",");
			}

			contents.pop_back();
		}

		// Add Columns
		if (table->columns.size() > 0)
		{
			contents += "\ncolumns:";
			for (auto const& column : table->columns)
			{
				contents += (column.first + " " + column.second + ",");
			}

			contents.pop_back();
		}

		// Add Rows
		for (auto& row : table->rows)
		{
			contents += "\nrow:";

			for (auto& val : row)
			{
				contents += (val + ",");
			}

			contents.pop_back();
		}

		contents += "\n;\n";
	}

	out << contents;
	out.close();

	updateRows();
	updatePrimaryTrees();
}

// TODO: Accept a list of columns, tie into user input. This might change to accepting a table name and a list of columns and creating a Table constructor. That may be the cleanest way

/// <summary>
/// Add a table to the associated database
/// </summary>
/// <param name="tbl">reference to the table to add</param>
void Database::AddTable(Table& tbl)
{
	tables.push_back(tbl);

	this->Save();
}


/// <summary>
/// For all tables in the database, if the table name matches, erase it from the file and save its new state
/// </summary>
/// <param name="name">name of table to remove</param>
void Database::DropTable(std::string name)
{
	int count = 0;
	for (auto& it : tables)
	{
		if (it.table_name == name)
		{
			tables.erase(tables.begin() + count);

			this->Save();
		}
		count = count + 1;
	}
}


/// <summary>
/// with an inputted name and update clause, the table is updated by creating a new table and deleting the old one, then saving it.
/// </summary>
/// <param name="table_name">Name of the table to update</param>
/// <param name="update_clause">command to execute</param>
/// <param name="where_clause">table to update based on the command</param>
void Database::UpdateTable(string table_name, vector<string> update_clause, vector<string> where_clause) {
	Table tbl = this->get_table(table_name);
	int update_idx = tbl.get_column_index(update_clause[0]);
	int where_idx = tbl.get_column_index(where_clause[0]);
	vector<vector<string>> new_rows;

	cout << "Where idx: " << where_idx << endl;
	cout << "Update idx: " << update_idx << endl;

	for (vector<string> row : tbl.rows) {
		if (row[where_idx] == where_clause[1]) {
			row[update_idx] = update_clause[1];
		}

		new_rows.push_back(row);
	}

	tbl.rows = new_rows;

	this->DropTable(table_name);
	this->AddTable(tbl);

	this->Save();

}

/// Author: Andrew Nunez
/// <summary>
/// Read the given file name 
/// </summary>
/// <param name="db_name">name of the database to read</param>
void Database::Read(std::string db_name)
{
	int tmp_size, i = 0;
	std::string line;
	std::string* tmp_parent_array;
	std::string* tmp_child_array;
	std::ifstream file("data/" + db_name + ".db");

	std::string tbl_name;
	std::map<std::string, std::string> keys;
	std::vector<std::vector<std::string>> rows;
	std::map<std::string, std::string> columns;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			tmp_size = std::count(line.begin(), line.end(), ',') + 1;

			// Begin parsing each line
			if (line.find("database:") == 0)
			{
				database_name = line.substr(line.find(":") + 1);
			}
			else if (line.find("table_name:") == 0)
			{
				tbl_name = line.substr(line.find(":") + 1);
			}
			else if (line.find("row:") == 0)
			{
				std::vector<std::string> tmp_v;
				tmp_parent_array = Parser::split_str(line, ',');

				for (i = 0; i < tmp_size; i++)
				{
					tmp_v.push_back(tmp_parent_array[i]);
				}

				rows.push_back(tmp_v);
			}
			else if (line.find("keys:") == 0)
			{
				//keys.insert({ tmp_child_array[0], tmp_child_array[1] }); fix later
			}
			else if (line.find("columns:") == 0)
			{
				tmp_parent_array = Parser::split_str(line, ',');

				for (i = 0; i < tmp_size; i++)
				{
					tmp_child_array = Parser::split_str(tmp_parent_array[i], ' ');

					if (line.find("keys:") == 0)
					{
						keys.insert({ tmp_child_array[0], tmp_child_array[1] });
					}
					else if (line.find("columns:") == 0)
					{
						columns.insert({ tmp_child_array[0], tmp_child_array[1] });
					}
				}
			}
			/*else if (line.find(",") != std::string::npos)
			{
				tmp_parent_array = Parser::split_str(line, ',');

				for (i = 0; i < tmp_size; i++)
				{
					tmp_child_array = Parser::split_str(tmp_parent_array[i], ' ');

					if (line.find("keys:") == 0)
					{
						keys.insert({ tmp_child_array[0], tmp_child_array[1] });
					}
					else if (line.find("columns:") == 0)
					{
						columns.insert({ tmp_child_array[0], tmp_child_array[1] });
					}
				}
			}*/
			else if (line.find(";") == 0)
			{
				Table* tbl = new Table(tbl_name);
				tbl->keys = keys;
				tbl->rows = rows;
				tbl->columns = columns;

				this->AddTable(*tbl);

				rows.clear();
				columns.clear();
				keys.clear();

			}
			else
			{
				std::cout << "Database is Corrupt!" << std::endl;
			}
		}
		file.close();
	}
	else
	{
		std::cout << "Database does not exist!" << std::endl;
	}
}

/// list the "data" files in the db
void Database::List()
{
	FileHelper::listfiles("data", "db");
}

/// list the tables
void Database::List_Tables()
{
	for (Table tbl : tables)
	{
		std::cout << tbl.table_name << std::endl;
	}
}

/// return a boolean; true if the a table is found, false otherwise
bool Database::find_table(std::string name)
{
	for (Table tbl : tables)
	{

		if (name == tbl.table_name)
		{
			return true;
		}
		else
			return false;
	}
}


/// return the table reference with a particular name
Table Database::get_table(std::string name)
{
	Table ret;

	for (Table tbl : tables)
	{
		if (tbl.table_name == name)
		{
			ret = tbl;

			break;
		}
	}

	return ret;
}



/// Author: Saurav Gautam
/// Save the table after being updated in the console
/// Replaces the table by creating a new one
void Database::SaveTable(Table table)
{

	std::string tableName = table.table_name;
	int count = 0;

	for (Table tbl : tables)
	{
		if (tbl.table_name == tableName)
		{
			break;
		}
		count = count + 1;
	}

	tables[count] = table;
}

/// <summary>
/// Rename an existing table
/// </summary>
/// <param name="old_table_name"></param>
/// <param name="new_table_name"></param>
void Database::RenameTable(std::string old_table_name, std::string new_table_name)
{

	Table tbl = this->get_table(old_table_name);

	tbl.table_name = new_table_name;

	this->DropTable(old_table_name);
	this->AddTable(tbl);

	this->Save();
}

/// <summary>
/// Rename a column in an existing table
/// </summary>
/// <param name="old_column_name"></param>
/// <param name="new_column_name"></param>
/// <param name="table_name"></param>
void Database::RenameColumn(std::string old_column_name, std::string new_column_name, std::string table_name)
{
	Table tbl = this->get_table(table_name);
	std::map<std::string, std::string> new_columns;

	new_columns = tbl.Rename_column(new_column_name, old_column_name);
	tbl.columns = new_columns;

	this->DropTable(table_name);
	this->AddTable(tbl);

	this->Save();
}


/// list the tables and print their information
void Database::List_Info() {
	int size = 0;
	std::cout << "Database Name:    	" << database_name << endl;
	std::cout << "Number of Tables: 	" << tables.size() << endl;
	std::cout << "==========================" << endl;
	for (Table tbl : tables) {
		cout << ">    " << tbl.table_name << endl;

		for (vector<string> row : tbl.rows) {
			for (string str : row) {
				size += str.length();
			}
		}

	}
	std::cout << "==========================" << endl;

	std::cout << "Total Size: " << size << " bytes" << endl;
}

/// Author: Saurav Gautam, Andrew Nunez
/// Read the insert statement and insert values in the table
void Database::insert_into(std::string statement, std::string table_name)
{
	Table current_table = get_table(table_name);;
	vector<string> columns = Parser::get_insert_columns(statement, table_name);
	vector<vector<string> > values = Parser::get_insert_rows(statement, table_name);

	//check to see if ID is present in the columns
	if (std::find(columns.begin(), columns.end(), "ID") != columns.end())
	{
		//no worries, the user SHOULD be setting their own ID
	}
	else
	{
		//manually add ID to columns, then a basic iterator representing row number to the values
		columns.push_back("ID");
		string newid = std::to_string(current_table.rows.size() + 1);
		
		values[0].push_back(newid);
	}

	vector<string> col_names = current_table.get_column_names();

	vector<int> order;

	for (string str : col_names) {
		auto it = std::find(columns.begin(), columns.end(), str);

		if (it != columns.end()) {
			order.push_back(std::distance(columns.begin(), it));
		}
		else {
			order.push_back(-1);
		}
	}

	int cnt = 0;

	for (vector<string> row : values)
	{
		vector<string> temp;

		for (int j = 0; j < order.size(); j++)
		{
			if (order[j] == -1)
			{
				temp.push_back("NULL");
			}
			else
			{
				temp.push_back(Utils::trim(row[order[j]]));
			}
		}

		cnt += 1;

		current_table.Insert(temp);
	}

	SaveTable(current_table);

}

void Database::delete_column(std::string column_name, std::string table_name)
{
	//get the index of the column name
	Table current_table = get_table(table_name);;

	int colindex = current_table.get_column_index(column_name);

	if (colindex != -1)
	{
		//loop through rows to delete the value at the column index

		for (int i = 0; i < current_table.rows.size(); i++)
		{
			current_table.rows[i].erase(current_table.rows[i].begin() + colindex);
		}

		//delete the reference in column storage
		current_table.columns.erase(current_table.columns.find(column_name));
	}

	SaveTable(current_table);
}

void Database::updateRows()
{
	for (Table tbl : tables)
	{
		tbl.newrows.clear();
		for (std::vector<std::string> rw : tbl.rows)
		{
			Row nrow = Row();
			int rowfind = 0;
			int introws = 0;
			int strrows = 0;
			int charrows = 0;

			for (std::pair<std::string, std::string> col : tbl.columns)
			{
				if (col.second == "string")
				{
					Column<string> newcol = Column<string>();
					newcol.AddValue(rw[rowfind]);
					newcol.SetName(col.first);
					nrow.strColumn.push_back(newcol);
					
				}
				else if (col.second == "int")
				{
					Column<int> newcol = Column<int>();
					newcol.AddValue(stoi(rw[rowfind]));
					newcol.SetName(col.first);
					nrow.intColumn.push_back(newcol);

				}
				else if (col.second == "char")
				{
					char* char_arr;
					string str_obj(rw[rowfind]);
					char_arr = &str_obj[0];
					cout << char_arr;
					Column<char*> newcol = Column<char*>();
					newcol.AddValue(char_arr);
					newcol.SetName(col.first);
					nrow.charColumn.push_back(newcol);
				}
				else
				{
					//unsupported column type - assume string? - come back to this
					Column<string> newcol = Column<string>();
					newcol.AddValue(rw[rowfind]);
					newcol.SetName(col.first);
					nrow.strColumn.push_back(newcol);
				}
				rowfind = rowfind + 1;

				tbl.newrows.push_back(nrow);
			}
			SaveTable(tbl);
		}
}
	//	int intindex = 0;
	//	int stringindex = 0;
	//	int charindex = 0;
	//	int typesearch = 0;
	//	int whichrow = 0;
	//	for (std::vector<std::string> rw : tbl.rows)
	//	{
	//		int whichcol = 0;
	//		for (std::pair<std::string, std::string> col : tbl.columns)
	//		{								
	//			string test = tbl.rows[whichrow][whichcol];
	//			if (true == true)
	//			{
	//				if (col.second == "string")
	//				{
	//					tbl.newrows[whichrow].strColumn[stringindex].AddValue(test);
	//					stringindex = stringindex + 1;
	//				}
	//				else if (col.second == "int")
	//				{
	//					tbl.newrows[whichrow].intColumn[intindex].AddValue(stoi(tbl.rows[whichrow][whichcol]));
	//					intindex = intindex + 1;
	//				}
	//				else if (col.second == "char")
	//				{
	//					char* char_arr;
	//					string str_obj(tbl.rows[whichrow][whichcol]);
	//					char_arr = &str_obj[0];
	//					cout << char_arr;
	//					tbl.newrows[0].charColumn[charindex].AddValue(char_arr);
	//					charindex = charindex + 1;
	//				}
	//				else
	//				{
	//					tbl.newrows[0].strColumn[stringindex].AddValue(tbl.rows[whichrow][whichcol]);
	//					stringindex = stringindex + 1;
	//				}
	//			}
	//			
	//			whichcol = whichcol + 1;
	//		}
	//		whichrow = whichrow + 1;
	//	}
	//	int x = 4;
	//}
}

inline void Database::updatePrimaryTrees()
{
	trees.clear();
	for (Table tbl : tables)
	{
		BPTree newPrimaryKeyIndex;
		newPrimaryKeyIndex.Name = tbl.table_name;

		for (Row r : tbl.newrows)
		{
			Row* rpoint = &r;
			
			for (Column<int> c : r.intColumn)
			{
				//check to see if the colname is the primary key
				if (c.GetName() == PRIMARY_KEY)
				{
					//index based on the value here
					newPrimaryKeyIndex.insert(c.GetValue(), rpoint);
				}
			}

		}
		tbl.primaryKeyTree = newPrimaryKeyIndex;
		trees.push_back(newPrimaryKeyIndex);

		SaveTable(tbl);
	}
}
