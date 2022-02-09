
template <class T>

struct Column {
	T* value;
};

class Row {
	int numColumns;
	Column* columns;

public:
	Row(int);
};


Row::Row(int numberOfColumns) {
	numColumns = numberOfColumns;
	columns = new Column[numberOfColumns];
}
