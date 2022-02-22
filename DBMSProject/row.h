#include <string>
#include <vector>
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
	/// <typeparam name="T"></typeparam>
	/// <param name="val"></param>
	template <typename T> 
	void AddValue(T val) {
		value = val;
	}
	/// <summary>
	/// Sets the name of the column
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="na"></param>	
	void SetName(string na) {
		name = na;
	}
	
	
	/// <summary>
	/// Returns the value of the column
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>	
	T GetValue() {
		return value;
	}
	
	/// <summary>
	/// Gets the name of the column
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <returns></returns>	
	string GetName() {
		return name;
	}
};






/// <summary>
/// A Row class that contains all the values of a row
/// </summary>
class Row {	
private:
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
	/// <param name="whichColumnValue"></param>
	/// <param name="columns"></param>
	void PrintColumns(int whichColumnValue, vector<string> columns) {
		int maxSize = GetLargestColumnSize();
		
		for (Column<int> i : intColumn) {
			for (string col : columns) {
				if (col == "*" || col == i.GetName()) {
					if (whichColumnValue == 0) {
						PrintColumnName(i.GetName(), maxSize);
					}
					else {
						PrintColumnValue(to_string(i.GetValue()), maxSize);
					}
				}
			}
		}

		for (Column<string> i : strColumn) {
			for (string col : columns) {
				if (col == "*" || col == i.GetName()) {
					if (whichColumnValue == 0) {
						PrintColumnName(i.GetName(), maxSize);
					}
					else {
						PrintColumnValue(i.GetValue(), maxSize);
					}
				}
			}
		}

		for (Column<char*> i : charColumn) {
			for (string col : columns) {
				if (col == "*" || col == i.GetName()) {
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

	
	void PrintSpaces(int nameSize, int maxSize) {		
		for (int i = 0; i < maxSize - nameSize; i++) {
			cout << " ";
		}
	}

	
	void PrintColumnName(string name, int maxSize) {
		cout << name;

		PrintSpaces(name.length(), maxSize);

		cout << " | ";
	}

	void PrintHeaders(vector<string> columns) {
		int maxSize = GetLargestColumnSize();
		int totalSpace = (TotalColumns(columns) * (maxSize + 3)) + 1;
		
		// seperator indecator
		for (int i = 0; i < totalSpace; i++) {
			cout << "-";
		}
		cout << endl;

		cout << "| ";		

		// print column names
		PrintColumns(0, columns);
		
		// end headers
		cout << endl;



		// seperator indecator
		for (int i = 0; i < totalSpace; i++) {
			cout << "=";
		}
		cout << endl;

	}

public:	
	/// <summary>
	///  Arrays of columns for each type
	/// </summary>
	vector<Column<string>> strColumn;
	vector<Column<int>> intColumn;
	vector<Column<char*>> charColumn;	
	

	Row() {		
	}

	int TotalColumns(vector<string> columns) {
		int total = 0;
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
			total = columns.size();
		}

		return total;
	}

	void PrintRow(vector<string> columns) {
		PrintHeaders(columns);
		PrintSingleRow(columns);

	}

	void PrintSingleRow(vector<string> columns) {
		cout << "| ";

		int maxSize = GetLargestColumnSize();
		int totalSpace = (TotalColumns(columns) * (maxSize + 3)) + 1;

		PrintColumns(1, columns);

		cout << endl;

		// seperator indecator
		for (int i = 0; i < totalSpace; i++) {
			cout << "-";
		}
		cout << endl;
	}
};

