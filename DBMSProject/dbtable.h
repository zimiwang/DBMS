#pragma once


#include <string>
#include <vector>
#include "parser.h"
#include "row.h"
#include "bplustree.h"
#include "bminustree.h"


using namespace std;

class DBTable {
private:
	vector<string> secondaryKeys;
	string primaryKey;
	string table_name;
	string database_name;
	string primaryKeyName = "ID";
	vector<string> columns;

	BPTree primaryTree;
	vector<BTree<string>> secondaryStringTree;
	vector<BTree<char>> secondaryCharTree;
	vector<BTree<int>> secondaryIntTree;
	

public:

	DBTable(string name, vector<string> cols) {
		table_name = name;
		columns = cols;
	}


};