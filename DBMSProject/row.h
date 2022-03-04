#include <string>
#include <vector>
#include "utils.h"
#pragma once


using namespace std;


/// <summary>
/// A column class that holds the value of a column
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T> class Column {
	string name;	// The name of the column || Have not decided if we will use this
	T value;
public:	
	/// <summary>
	/// Adds a value
	/// </summary>
	/// <typeparam name="T">The type of value</typeparam>
	/// <param name="val">The value to save</param>
	template <typename T> 
	void AddValue(T val) {
		value = val;
	}
	/// <summary>
	/// Sets the name of the column
	/// </summary>
	/// <typeparam name="T">The type of value</typeparam>
	/// <param name="na">The name of the column</param>	
	void SetName(string na) {
		name = na;
	}
	
	
	/// <summary>
	/// Returns the value of the column
	/// </summary>
	/// <typeparam name="T">The type of value to return</typeparam>
	/// <returns>Returns the value of the column</returns>	
	T GetValue() {
		return value;
	}
	
	/// <summary>
	/// Gets the name of the column
	/// </summary>
	/// <typeparam name="T">The type of the column</typeparam>
	/// <returns>The name of the column</returns>	
	string GetName() {
		return name;
	}
};






/// <summary>
/// A Row class that contains all the values of a row
/// </summary>
class Row {	
private:
	bool empty;

	/// <summary>
	/// Checks to see if all the columns in the columns parameter are in the columns of the row
	/// </summary>
	/// <param name="columns">The columns to check</param>
	/// <returns>Returns True if all of the columns are in the row else False</returns>
	bool CheckAllColumnsFound(vector<string> columns) {
		bool found = true;
		vector<string> rowColumns = GetColumnNames();
		for (string column : columns) {					
			if (column != "*" && !(find(rowColumns.begin(), rowColumns.end(), column) != rowColumns.end())) {
				cout << "Column " <<  "'" << column << "'" << " not found." << endl;				
				found = false;				
				break;
			}
		}

		return found;
	}

	/// <summary>
	/// Gets all the column names in the row
	/// </summary>
	/// <returns></returns>
	vector<string> GetColumnNames() {
		vector<string> columns;

		// Go through the rows columns 
		for (Column<int> i : intColumn) {
			// Got through the incoming columns
			columns.push_back(i.GetName());
		}

		// The same as above
		for (Column<string> i : strColumn) {
			columns.push_back(i.GetName());
		}

		// The same as above
		for (Column<char*> i : charColumn) {
			columns.push_back(i.GetName());
		}

		return columns;
	}


	/// <summary>
	/// Prints a formatted value of a column 
	/// </summary>
	/// <param name="whichColumnValue"></param>
	/// <param name="columns"></param>
	void PrintColumnValue(string value, int maxSize) {
		cout << value;

		PrintSpaces(value.length(), maxSize);
		cout << " | ";
	}

	/// <summary>
	/// Prints all column names or values
	/// </summary>
	/// <param name="whichColumnValue">Print column names or column values (0 - names, 1 - values)</param>
	/// <param name="columns">A vector of columns to print</param>
	void PrintColumns(int whichColumnValue, vector<string> columns, int maxSize) {		
		int totalColumnsFound = 0;

		// Go through the rows columns 
		for (Column<int> i : intColumn) {
			// Got through the incoming columns
			for (string col : columns) {
				// check if the incoming column matches row column
				if (col == "*" || col == i.GetName()) {
					totalColumnsFound++;
					// print column names or column values
					if (whichColumnValue == 0) {
						PrintColumnName(i.GetName(), maxSize);
					}
					else {
						PrintColumnValue(to_string(i.GetValue()), maxSize);
					}
				}
			}
		}

		// The same as above
		for (Column<string> i : strColumn) {
			for (string col : columns) {
				if (col == "*" || col == i.GetName()) {
					totalColumnsFound++;
					if (whichColumnValue == 0) {
						PrintColumnName(i.GetName(), maxSize);
					}
					else {
						PrintColumnValue(i.GetValue(), maxSize);
					}
				}
			}
		}

		// The same as above
		for (Column<char*> i : charColumn) {
			for (string col : columns) {
				if (col == "*" || col == i.GetName()) {
					totalColumnsFound++;
					if (whichColumnValue == 0) {
						PrintColumnName(i.GetName(), maxSize);
					}
					else {
						PrintColumnValue(to_string(*i.GetValue()), maxSize);
					}
				}
			}
		}
		
	}

	/// <summary>
	///	Prints spaces between values
	/// </summary>
	/// <param name="nameSize">The size of the value that was printed before</param>
	/// <param name="maxSize">The size of the largest value in any column</param>
	void PrintSpaces(int nameSize, int maxSize) {		
		for (int i = 0; i < maxSize - nameSize; i++) {
			cout << " ";
		}
	}

	/// <summary>
	/// Prints a column name
	/// </summary>
	/// <param name="name">The name to print</param>
	/// <param name="maxSize">The size of the largest value in any column</param>
	void PrintColumnName(string name, int maxSize) {
		cout << name;

		PrintSpaces(name.length(), maxSize);

		cout << " | ";
	}


	/// <summary>
	/// Prints the names of columns as headers
	/// </summary>
	/// <param name="columns">A vector of column names to print</param>	
	bool PrintHeaders(vector<string> columns, int maxSize) {		
		int totalSpace = (TotalColumns(columns) * (maxSize + 3)) + 1;
		

		// checks that all the columns are contained in the row
		// if not then don't start printing the table
		if (CheckAllColumnsFound(columns)) {
			// top indicator
			for (int i = 0; i < totalSpace; i++) {
				cout << "-";
			}
			cout << endl;

			// start printing
			cout << "| ";		

			// print column names
			PrintColumns(0, columns, maxSize);
		
			// end headers
			cout << endl;


			// seperator indecator
			for (int i = 0; i < totalSpace; i++) {
				cout << "=";
			}
			cout << endl;

			return true;

		}
		return false;

	}

public:		
	vector<Column<string>> strColumn;
	vector<Column<int>> intColumn;
	vector<Column<char*>> charColumn;		
	
	Row() {	
		empty = true;	
	}

	void InUse() {
		empty = false;
	}

	bool isEmpty() {
		return empty;
	}

	/// <summary>
	/// Gets the largest value out of all the columns
	/// </summary>
	/// <returns>The size of the largest value</returns>
	int GetLargestColumnSize() {
		int size = 0;
		// for column names
		// integers
		for (Column<int> i : intColumn) {
			// convert to string and then check size
			int temp = i.GetName().length();
			if (temp > size) {
				size = temp;
			}
		}
		// strings
		for (Column<string> i : strColumn) {
			int temp = i.GetName().length();
			if (temp > size) {
				size = temp;
			}
		}
		// char
		for (Column<char*> i : charColumn) {
			// convert to string and then check size
			int temp = i.GetName().length();
			if (temp > size) {
				size = temp;
			}
		}

		// for column values
		// integers
		for (Column<int> i : intColumn) {
			// convert to string and then check size
			int temp = to_string(i.GetValue()).length();
			if (temp > size) {
				size = temp;
			}
		}
		// strings
		for (Column<string> i : strColumn) {
			int temp = i.GetValue().length();
			if (temp > size) {
				size = temp;
			}
		}

		// chars will always have one character so no need for checking size
		return size;
	}


	/// <summary>
	///	Finds the total columns being used
	/// </summary>
	/// <param name="columns">A vector of column names</param>
	/// <returns>Returns teh total columbs being used</returns>	 
	int TotalColumns(vector<string> columns) {
		int total = 0;

		// if all columns are being used count all of the rows columns
		if (columns[0] == "*") {
			for (Column<int> i : intColumn) {
				total++;
			}

			for (Column<string> i : strColumn) {
				total++;
			}

			for (Column<char*> i : charColumn) {
				total++;
			}
		}
		else {
			// the total columns is the incoming column names
			total = columns.size();
		}

		return total;
	}

	const static void PrintFullTable(vector<Row> rows, vector<string> cols) {
		// get largest column value
		int maxSize = 0;
		for (Row row : rows) {
			int size = row.GetLargestColumnSize();
			if (size > maxSize) {
				maxSize = size;
			}
		}

		// print rows
		int i = 0;
		for (Row row : rows) {
			if (!row.isEmpty()) {
				if (i == 0) {
					row.PrintRow(cols, maxSize);
					i++;
				}
				else {
					row.PrintSingleRow(cols, maxSize);
				}
			}
			else {
				cout << "Could not find rows" << endl;
			}
		}
	}

	/// <summary>
	/// Prints a row with headers
	/// </summary>
	/// <param name="columns"></param>
	void PrintRow(vector<string> columns, int maxSize = 0) {
		maxSize = maxSize == 0 ? GetLargestColumnSize() : maxSize;
		bool shouldPrint = PrintHeaders(columns, maxSize);
		PrintSingleRow(columns, maxSize, shouldPrint);
	}

	/// <summary>
	///	Prints a row without headers
	/// </summary>
	/// <param name="columns"></param>
	void PrintSingleRow(vector<string> columns, int maxSize, bool checkColumns = true) {
		// checks that all the columns are contained in the row
		// if not then don't start printing		
		if (checkColumns) {
			// start printing
			cout << "| ";
			
			int totalSpace = (TotalColumns(columns) * (maxSize + 3)) + 1;

			// print column values
			PrintColumns(1, columns, maxSize);

			// end of column values
			cout << endl;

			// seperator indecator
			for (int i = 0; i < totalSpace; i++) {
				cout << "-";
			}
			cout << endl;
		}		
	}
};

