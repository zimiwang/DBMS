
// needs Column and Colum_Base to have an array of generic types

class Column {
	
};

/// <summary>
/// Creates a column with a value
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T > class Column_Base : public Column {
	string name; // the name of the column. 
	T* value;
};


/// <summary>
/// Creates a row data structure that holds all the column values for that row
/// </summary>
class Row {
	int numColumns;
	Column* columns;	

public:
	Row(int numberOfColumns) {
		numColumns = numberOfColumns;
		columns = new Column[numberOfColumns];
	}
};


