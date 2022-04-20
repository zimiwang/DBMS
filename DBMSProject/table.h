/*
*	File: 	    table.h
*   Author:     Andrew Nunez
*   Date:       Sept. 23, 2021
*
*   This file holds a class that defines a table and it's data to read and write from a .bin file.
*/
#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "parser.h"
#include "row.h"
#include "bplustree.h"

class Table {
public:

	/// ***Unreachable Method
	void Print_Rows(std::vector<std::string> column_names, vector<string> where_clause, string conditional);
	void Rename_table(std::string new_table_name);


	/// The name of the table
	std::string table_name;

	/// The name of the Database
	std::string database_name;

	/// The collection of keys <type, name> of the table -- depreciate this later.
	std::map<std::string, std::string> keys;

	std::string primaryKeyName = "ID";

	std::vector<std::string> secondaryKeys;
	std::vector<std::string> foreignKeys;

	/// The collection of column names and types to a table
	std::map<std::string, std::string> columns;

	/// The collection of arrays of rows for the table.
	std::vector<std::vector<std::string>> rows;
	std::vector<Row> newrows;

	BPTree primaryKeyTree;
	bool wasCreated = true;
	/// Counter for row ID
	int ID_count = 0;

	void Insert(std::vector<std::string> row);

	void AddKey(std::string key, std::string value);

	int GetLargestColumnSize();

	void DeleteRow(vector<string> row);

	int get_column_index(std::string column_name);

	map<std::string, std::string> Rename_column(std::string new_column_name, std::string old_column_name);


	// TODO: Add column names
	std::vector<std::vector<std::string> > Select(std::vector<std::string> col_names);

	std::vector<std::string> get_column_names();

	void Delete();

	Table() {
	}

	// Use this as as create in DB CreateTable method
	// TODO: Tie into user input
	Table(std::string name) {
		table_name = name;
		primaryKeyName = "ID_" + name;
		keys.insert(std::pair<std::string, std::string>("primary", primaryKeyName));

	}


	/// <summary>
	/// Creates a new instances of a table with columns assigned
	/// </summary>
	/// <param name="name"></param>
	/// <param name="cols"></param>
	Table(std::string name, vector<string> cols)
		: Table(name) {
		this->table_name = name;
		bool hasID = false;
		bool isCreate = true;

		for (std::string col : cols) {

			vector<string> tmp = Utils::split(col, " ");

			if (tmp.size() == 2)
			{
				if (tmp[0] == ("ID_" + name))
				{
					hasID = true;
				}
				//columns.insert(Utils::trim(tmp[0]), Utils::trim(tmp[1]));
				columns.insert(std::pair<std::string, std::string>(Utils::trim(tmp[0]), Utils::trim(tmp[1])));
				int letstes = 0;
			}
			else
			{
				std::cout << "Invalid format " << std::endl;
				isCreate = false;
				wasCreated = false;
				break;
			}
		}
		if (isCreate = true)
		{
			if (hasID == false) //We have no ID column defined by the user, manually add one
			{
				columns.insert(std::pair<std::string, std::string>(("ID_" + name), "int"));
			}		
		}

	}

};


/// <summary>
/// Gets the size of the largest column
/// </summary>
/// <returns>The size of the largest column</returns>
int Table::GetLargestColumnSize() {
	int ret = 0;

	for (std::vector<std::string> row : rows) {
		for (std::string& value : row) {
			if (value.length() > ret) {
				ret = value.length();
			}
		}

	}

	for (auto const& it : columns) {
		if (it.first.length() > ret) {
			ret = it.first.length();
		}

	}

	return ret;

}

/// <summary>
/// Gets the index of the provided column
/// </summary>
/// <param name="column_name"></param>
/// <returns>index of the provided column</returns>
int Table::get_column_index(std::string column_name) {
	int ret = -1;
	//std::map<std::string, std::string>::iterator it;
	int col_index;

	//it = columns.find(column_name);
	auto it = columns.find(column_name);

	if (it != columns.end()) {
		col_index = std::distance(columns.begin(), it);
		ret = col_index;
	}

	return ret;
}

/// <summary>
/// Adds a key to the table
/// </summary>
/// <param name="key"></param>
/// <param name="value"></param>
void Table::AddKey(std::string type, std::string name) {
	std::cout << "Adding: " << type << " " << name << std::endl;

	keys.insert(std::pair<std::string, std::string>(type, name));

	if (type == "secondary")
	{
		secondaryKeys.push_back(name);
	}
	else if (type == "foreign")
	{
		foreignKeys.push_back(name);
	}

}


/// <summary>
/// Inserts a row vector to the table
/// </summary>
/// <param name="row"></param>
void Table::Insert(std::vector<std::string> row) {
	rows.push_back(row);
}


/// <summary>
/// Selects the columns from the table
/// </summary>
/// <param name="col_names"></param>
/// <returns></returns>
std::vector<std::vector<std::string> > Table::Select(std::vector<std::string> col_names) {
	return rows;
}



/// <summary>
/// Deletes the table
/// </summary>
void Table::Delete() {
	delete this;
}

std::vector<std::string> Table::get_column_names() {
	std::vector<std::string> temp;
	auto iter = columns.begin();

	while (iter != columns.end()) {
		temp.push_back(Utils::trim(iter->first));
		iter++;
	}

	return temp;
}

/// <summary>
/// Delete the provided row
/// </summary>
/// <param name="row"></param>
void Table::DeleteRow(vector<string> row) {
	rows.erase(std::remove(rows.begin(), rows.end(), row), rows.end());
}
#pragma once


/// ***Unreachable Method
/// <summary>
/// rename an existing table
/// </summary>
/// <param name="new_table_name"></param>
void Table::Rename_table(std::string new_table_name) 
{
	this->table_name = new_table_name;
}

/// <summary>
/// Rename a column in an existing table
/// </summary>
/// <param name="new_column_name"></param>
/// <param name="old_column_name"></param>
/// <returns></returns>
map<std::string, std::string> Table::Rename_column(std::string new_column_name, std::string old_column_name)
{
	std::map<std::string, std::string> new_columns;
	std::map<std::string, std::string>::iterator it;

	for (it = columns.begin(); it != columns.end(); it++)
	{
		cout << it->first << ' ' << it->second << endl;
	}

	for (it = columns.begin(); it != columns.end(); it++) 
	{
		if (it->first != old_column_name)
		{
			new_columns.insert({ it->first, it->second });
		}
		else
		{
			new_columns.insert(std::pair<string, std::string>(new_column_name, it->second));
		}
	}

	return new_columns;
}