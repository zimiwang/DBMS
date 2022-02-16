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
	void AddValue(T val);
	T GetValue();
};


/// <summary>
/// Adds a value
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="val"></param>
template <typename T> 
void Column<T>::AddValue(T val) {
	value = val;
}

/// <summary>
/// Returns the value of the column
/// </summary>
/// <typeparam name="T"></typeparam>
/// <returns></returns>
template <typename T> T Column<T>::GetValue() {
	return value;
}


/// <summary>
/// A Row class that contains all the values of a row
/// </summary>
class Row {	
public:	
	/// <summary>
	///  Arrays of columns for each type
	/// </summary>
	vector<Column<string>> strColumn;
	vector<Column<int>> intColumn;
	vector<Column<char*>> charColumn;
	
	//Row(int, int, int);
	Row() {
		//strColumn = new Column<string>[str];
		//intColumn = new Column<int>[ints];
		//charColumn = new Column<char*>[chars];
	}
};

/// <summary>
/// Base Column
/// </summary>
//class Column {
//	string name;
//public: 
//	string SetName(string n) {
//		name = n;
//	}
//	string GetName() {
//		return name;
//	}
//
//};
//
///// <summary>
///// Column for strings
///// </summary>
//class StringColumn : Column {	
//	string* value;
//public:
//	void AddValue(string* val) {
//		value = val;
//	}
//
//	string* GetValue() {
//		return value;
//	}
//};
//
///// <summary>
///// Column for Ints
///// </summary>
//class IntColumn : Column{
//	int* value;
//public:
//	void AddValue(int* val) {
//		value = val;
//	}
//
//	int* GetValue() {
//		return value;
//	}
//};
//
///// <summary>
///// Column for chars
///// </summary>
//class CharColumn : Column {
//	char* value;
//public:
//	void AddValue(char* val) {
//		value = val;
//	}
//
//	char* GetValue() {
//		return value;
//	}
//};


/// <summary>
/// A row class that contains columns with values
/// </summary>
//class Row {
//public:
//	StringColumn* strColumn;
//	IntColumn*    intColumn;
//	CharColumn*   charColumn;
//
//	/*Row(int, int, int);*/
//	Row(int str = 0, int ints = 0, int chars = 0) {
//		strColumn = new StringColumn[str];
//		intColumn = new IntColumn[ints];
//		charColumn = new CharColumn[chars];
//	}
//};