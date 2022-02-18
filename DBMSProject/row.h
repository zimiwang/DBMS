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
	void SetName(string na);
	T GetValue();
	string GetName();
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

template <typename T>
void Column<T>::SetName(string na) {
	name = na;
}

/// <summary>
/// Returns the value of the column
/// </summary>
/// <typeparam name="T"></typeparam>
/// <returns></returns>
template <typename T> T Column<T>::GetValue() {
	return value;
}
template <typename T> string Column<T>::GetName() {
	return name;
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

