#include <string>
#pragma once


using namespace std;

template <typename T> class Column {
	string name;
	T* value;
public:	
	void AddValue(T*);

	T* GetValue();
};

template <typename T> void Column<T>::AddValue(T* val) {
	value = val;
}

template <typename T> T* Column<T>::GetValue() {
	return value;
}

class Row {	
public:	
	Column<string>* strColumn;
	Column<int>* intColumn;
	Column<char>* charColumn;
	
	//Row(int, int, int);
	Row(int str = 0, int ints = 0, int chars = 0) {
		strColumn = new Column<string>[str];
		intColumn = new Column<int>[ints];
		charColumn = new Column<char>[chars];
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