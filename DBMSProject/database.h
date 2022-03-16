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

using namespace std;

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
	std::vector<Table> join_tables;

	std::vector<BPTree> primary_key_trees;

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
	Table join_table(std::string src_table, std::string dest_table, std::string foreign_key, std::string exkey);
	BPTree get_tree(string name);
	void UpdateTable(string table_name, vector<vector<string>> update_clause, vector<string> where_clause);
	void RenameTable(std::string old_table_name, std::string new_table_name);
	void RenameColumn(std::string old_column_name, std::string new_column_name, std::string table_name);
	void DeleteFrom(std::string tbl_name, std::string conditional, vector<string> clause);
	void delete_column(std::string column_name, std::string table_name);
	void keytotable(std::string keytype, std::string keyname, std::string table_name);
	void sortKeys();
	void updateRows();
	void updatePrimaryTrees();
	float sumRows(std::string table, std::string column);

	
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
	sortKeys();
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
/// with an inputted name and update clause or multiple update clauses, 
/// the table is updated by creating a new table and deleting the old one, then saving it.
/// </summary>
/// <param name="table_name"></param>
/// <param name="update_clause"></param>
/// <param name="where_clause"></param>
void Database::UpdateTable(string table_name, vector<vector<string>> update_clause, vector<string> where_clause) {
	Table tbl = this->get_table(table_name);

	for (size_t i = 0; i < update_clause.size(); i++)
	{
		vector<vector<string>> new_rows;
		//cout << "get_where_clause: " << where_clause[0] << endl;
		//cout << "get_update_clause: " << update_clause[0][i] << endl;

		int where_idx = tbl.get_column_index(where_clause[0]);
		int update_idx = tbl.get_column_index(update_clause[i][0]);

		cout << "Where idx: " << where_idx << endl;
		cout << "Update idx: " << update_idx << endl;

		if (where_idx == -1 || update_idx == -1) {
			std::cout << "The column name is incorrect" << std::endl;
			break;
		}

		else
		{
			for (vector<string> row : tbl.rows) {
				if (row[where_idx] == where_clause[1]) {

					//cout << "get_update_clause: " << update_clause[i][1] << endl;

					row[update_idx] = update_clause[i][1];
				}
				new_rows.push_back(row);
			}

			tbl.rows = new_rows;

			this->DropTable(table_name);
			this->AddTable(tbl);

		}

	}
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
				tmp_parent_array = Parser::split_str(line, ',');
				for (i = 0; i < tmp_size; i++)
				{
					tmp_child_array = Parser::split_str(tmp_parent_array[i], ' ');
					keys.insert({ tmp_child_array[0], tmp_child_array[1] });
				}

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
		if (tbl.table_name == "exT") cout << "exT table found\n";
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



/// <summary>
/// return the sum of the column
/// </summary>
/// <param name="table">table name to use</param>
/// <param name="column">name of which column to add</param>
/// <returns>float of the sum</returns>
float Database::sumRows(std::string fromTable, std::string column)
{
	float sum = 0;

	//get the table information (table, column, rows)
	Table table = this->get_table(fromTable);
	int columnIndex = table.get_column_index(column);
	std::vector<std::vector<std::string>> rows = table.rows;

	// iterate through the rows
	for (int i = 0; i < rows.size(); i++)
	{
		std::string value = rows[i][columnIndex];
		//cout << "row:" << i << "\t" << value << "\n";
		sum += stoi(value);
	}

	return sum;
}



/// <summary>
/// Joins two tables based on a foreign key
/// </summary>
/// <param name="src_table">table that has the foreign key</param>
/// <param name="dest_table">table who's primary key is the foreign key</param>
/// <param name="foreign_key">which column of table 1 is the foreign key</param>
/// <returns></returns>
Table Database::join_table(std::string src_table, std::string dest_table, std::string foreign_key, std::string exkey)
{
	Table join;
	Table src = this->get_table(src_table);
	Table dest = this->get_table(dest_table);
	join.table_name = src_table + "_" + dest_table;
	int colindex = src.get_column_index(foreign_key);
	int col2index = dest.get_column_index(exkey);
	int rowcount = 1;
	if (colindex != -1 && col2index != -1)
	{
		//colindex shouldn't ever be -1 but from here we loop through the rows and match local foreign key to foreign 
		//primary key --- TO NOTE | CURRENTLY CAN ONLY JOIN ON INT COLUMNS
		for (Row r : src.newrows)
		{
			
			for (Column<int> col : r.intColumn)
			{	
				if (col.GetName() == foreign_key) //this should pass exactly once per row
				{
					//we have the local row value, search through the primary index of the destination tree for the matching row
					Row foreignRow = dest.primaryKeyTree.search(col.GetValue());
					Row destrow = Row();  //empty by default
					bool keeprow = false;
					for (Row r2 : dest.newrows)
					{
						for (Column<int> col2 : r2.intColumn)
						{
							if (col2.GetName() == exkey)
							{
								if (col.GetValue() == col2.GetValue())
								{
									destrow = r2;
									//combine the two table's row structure
									Row combinedRow = Row(r); //i believe this is how you do a deep copy in C++
									for (Column<int> loc : destrow.intColumn)
									{
										combinedRow.intColumn.push_back(loc);
									}
									for (Column<string> loc : destrow.strColumn)
									{
										combinedRow.strColumn.push_back(loc);
									}
									for (Column<char> loc : destrow.charColumn)
									{
										combinedRow.charColumn.push_back(loc);
									}

									//push our merged row into the new table
									Column<int> newindex;
									newindex.SetName("ID_" + join.table_name);
									newindex.AddValue(rowcount);
									rowcount++;
									combinedRow.intColumn.push_back(newindex);
									join.newrows.push_back(combinedRow);
								}
							}
						}
					}
					
				}
			}
		}
	}
	join.primaryKeyName = "ID_" + join.table_name;
	this->join_tables.push_back(join);
	return join;
}

BPTree Database::get_tree(string name) {
	BPTree ret;

	for (BPTree tree : primary_key_trees) {
		if (tree.Name == name) {
			ret = tree;
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

void Database::DeleteFrom(std::string tbl_name, std::string conditional, vector<string> clause) {

	int count = 0;
	string value = clause[1];

	Table currentTable = this->get_table(tbl_name);
	int col_ndx = currentTable.get_column_index(clause[0]);
	vector<vector<string> > rows = currentTable.rows;
	for (vector<string> row : rows) {

		if (col_ndx != -1)
		{
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
		else {
			std::cout << count << "Incorrect column name" << endl;
			break;
		}
	}
	SaveTable(currentTable);
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



/// <summary>
/// Read the insert statement and insert values in the table
/// </summary>
/// <param name="statement">user-inputted command</param>
/// <param name="table_name">name of the table</param>
void Database::insert_into(std::string statement, std::string table_name)
{
	Table current_table = get_table(table_name);;
	vector<string> columns = Parser::get_insert_columns(statement, table_name);
	vector<vector<string> > values = Parser::get_insert_rows(statement, table_name);

	//for (int i = 0; i < columns.size(); i++) cout << "column:" << columns[i] << "\n";

	//check to see if ID is present in the columns
	if (std::find(columns.begin(), columns.end(), ("ID_" + table_name)) != columns.end())
	{
		//no worries, the user SHOULD be setting their own ID
	}
	else
	{
		//manually add ID to columns, then a basic iterator representing row number to the values
		columns.push_back(("ID_" + table_name));
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
/// <summary>
/// Deletes a column from the database
/// </summary>
/// <param name="column_name"></param>
/// <param name="table_name"></param>
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

		


		//loop through the new rows to delete that value
		//delete the reference in column storage
		current_table.columns.erase(current_table.columns.find(column_name));

	}
	
	SaveTable(current_table);
	updateRows();
	updatePrimaryTrees();
	
	
}
/// <summary>
/// adds a key to the old key storage from the table
/// </summary>
/// <param name="keytype"></param>
/// <param name="keyname"></param>
/// <param name="table_name"></param>
void Database::keytotable(std::string keytype, std::string keyname, std::string table_name)
{
	//get the index of the column name
	Table current_table = get_table(table_name);;
	
	current_table.AddKey(keytype, keyname);

	SaveTable(current_table);
}

/// <summary>
/// sorts keys into the lists they belong in. Something to note: Since we only have 1 primary key, it will be set to the most recently specified primary
/// </summary>
void Database::sortKeys()
{
	for (Table tbl : tables)
	{
		tbl.secondaryKeys.clear();
		tbl.foreignKeys.clear();
		for (std::pair<std::string, std::string> current_key : tbl.keys)
		{
			if (current_key.first == "primary")
			{
				//found primary key
				tbl.primaryKeyName = current_key.second;
			}
			else if (current_key.first == "secondary")
			{
				//found secondary key
				tbl.secondaryKeys.push_back(current_key.second);
			}
			else if (current_key.first == "foreign")
			{
				//foreign key found
				tbl.foreignKeys.push_back(current_key.second);

			}

		}
		SaveTable(tbl);
	}
}


/// <summary>
/// Updates the new row data structure from each tree based on the old row storage methodology.
/// </summary>
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
					char char_arr;
					string str_obj(rw[rowfind]);
					char_arr = str_obj[0];					
					Column<char> newcol = Column<char>();
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
			}
			tbl.newrows.push_back(nrow);
		}		
	SaveTable(tbl);
	}
}
/// <summary>
/// Updates the primary key trees for each table. TODO - Add a check to see if the table needs to be updated - maybe a bool flag in
/// table.h that says whether or not it has been altered since the last table was generated.
/// </summary>
inline void Database::updatePrimaryTrees()
{
	primary_key_trees.clear();
	for (Table tbl : tables)
	{
		BPTree newPrimaryKeyIndex;
		newPrimaryKeyIndex.Name = tbl.table_name;

		for (Row r : tbl.newrows)
		{
			/*Row* rpoint = &r;*/
			r.InUse();
			for (Column<int> c : r.intColumn)
			{
				//check to see if the colname is the primary key
				if (c.GetName() == tbl.primaryKeyName)
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
		tbl.primaryKeyTree = newPrimaryKeyIndex;
		primary_key_trees.push_back(newPrimaryKeyIndex);

		SaveTable(tbl);
	}
}
