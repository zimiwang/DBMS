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
	/// The name of the table
	std::string table_name;

	/// The name of the Database
	std::string database_name;

	/// The collection of keys <type, value> of the table
	std::map<std::string, std::string> keys;

	/// The collection of column names and types to a table
	std::map<std::string, std::string> columns;

	/// The collection of arrays of rows for the table.
	std::vector<std::vector<std::string>> rows;
	std::vector<Row> newrows;

	BPTree primaryKeyTree;

	/// Counter for row ID
	int ID_count = 0;

	void Print_Rows(std::vector<std::string> column_names, vector<string> where_clause, string conditional);

	void Insert(std::vector<std::string> row);

	void AddKey(std::string key, std::string value);

	int GetLargestColumnSize();

	void DeleteRow(vector<string> row);

	int get_column_index(std::string column_name);

	void Rename_table(std::string new_table_name);

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
		this->table_name = name;
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
		for (std::string col : cols) {
			vector<string> tmp = Utils::split(col, " ");
			if (tmp[0] == "ID")
			{
				hasID = true;
			}
			//columns.insert(Utils::trim(tmp[0]), Utils::trim(tmp[1]));
			columns.insert(std::pair<std::string, std::string>(Utils::trim(tmp[0]), Utils::trim(tmp[1])));
			int letstes = 0;

		}
		if (hasID == false) //We have no ID column defined by the user, manually add one
		{
			columns.insert(std::pair<std::string, std::string>("ID", "int"));
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
	std::map<std::string, std::string>::iterator it;
	int col_index;

	it = columns.find(column_name);

	if (it != columns.end()) {
		col_index = std::distance(columns.begin(), it);
		ret = col_index;
	}

	return ret;
}

/// <summary>
/// Prints the rows using the specified where clause and column names
/// </summary>
/// <param name="column_names"></param>
/// <param name="where_clause"></param>
/// <param name="conditional"></param>
void Table::Print_Rows(std::vector<std::string> column_names, vector<string> where_clause, string conditional) {
	int row_count = 0;

	std::map<std::string, std::string> print_cols;
	std::vector<std::vector<std::string> > print_rows;

	std::vector<int> indices;
	std::map<std::string, std::string>::iterator it;
	int col_index;
	int where_idx = -1;

	if (where_clause.size() > 0) {
		where_idx = this->get_column_index(where_clause[0]);
	}


	if (column_names.size() > 0 && column_names[0] == "*") {
		column_names.clear();

		for (auto const& it : columns) {
			column_names.push_back(it.first);
		}
	}

	for (std::string name : column_names) {
		it = columns.find(name);

		if (it != columns.end()) {
			col_index = std::distance(columns.begin(), it);
			print_cols.insert({ it->first, it->second });
			indices.push_back(col_index);
		}
		else {
			std::cout << "Column Name does not exist: " << name << std::endl;
			return;
		}

	}

	// Where clause goes here
	for (std::vector<std::string> row : rows) {
		std::vector<std::string> new_row;

		for (int i = 0; i < indices.size(); i += 1) {
			new_row.push_back(row[indices[i]]);
		}

		if (where_idx == -1) {
			print_rows.push_back(new_row);
		}
		else if (new_row[where_idx] == where_clause[1]) {
			print_rows.push_back(new_row);
		}

	}

	columns = print_cols;
	rows = print_rows;

	int col_char_count = GetLargestColumnSize();
	int row_char_count = (columns.size() * (col_char_count + 3)) + 1;

	std::cout << "| ";

	for (std::string name : column_names) {
		std::cout << name;

		for (int i = 0; i < col_char_count - name.length(); i += 1) {
			std::cout << " ";
		}

		std::cout << " | ";
	}

	std::cout << std::endl;

	for (int i = 0; i < row_char_count; i += 1) {
		std::cout << "=";
	}

	std::cout << std::endl;

	for (std::vector<std::string> row : print_rows) {
		std::cout << "| ";

		for (std::string& value : row) {
			std::cout << value;

			for (int i = 0; i < col_char_count - value.length(); i += 1) {
				std::cout << " ";
			}

			std::cout << " | ";

		}

		std::cout << std::endl;

		for (int i = 0; i < row_char_count; i += 1) {
			std::cout << "-";
		}

		std::cout << std::endl;

		row_count += 1;
	}

	std::cout << row_count << " rows selected." << std::endl;

}

/// <summary>
/// Adds a key to the table
/// </summary>
/// <param name="key"></param>
/// <param name="value"></param>
void Table::AddKey(std::string key, std::string value) {
	std::cout << "Adding: " << key << " " << value << std::endl;

	keys.insert(std::pair<std::string, std::string>(key, value));

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