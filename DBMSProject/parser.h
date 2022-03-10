#pragma once
#include "database.h"

#include <fstream>
#include <regex>
//#include <bits/stdc++.h>		// comment this out because no uses it and it is recommeded that you don'MinDegree use it.
#include "utils.h"
#include <iomanip>
#include "dictionary.h"
using namespace std;

class Parser {
	vector<string> KEYWORDS;


public:
	Parser() {
		KEYWORDS.push_back("between");
		KEYWORDS.push_back("and");
		KEYWORDS.push_back("where");
	}
	Dictionary static get_where_clause(string cmd);
	vector<string> static findKeyWords(vector<string>, string cmd);
	vector<string> static split_text(string input);
	vector<string> static get_select_columns(string cmd);
	static string* split_str(std::string str, char delim);
	vector<string> static get_create_columns(string cmd);
	vector<string> static get_where_clause(string cmd, string op);
	vector<string> split_text(string input, string delimeter);
	vector<string> static get_insert_columns(string cmd, string table_name);
	vector<vector<string> > static get_insert_rows(string cmd, string table_name);
	std::string static to_lower(std::string s);
	vector<string> static get_update_clause(string cmd);
	string static get_conditional(string stm);
	string static get_table_name(string cmd, string first_delim, string second_delim);
	string static get_table_name(string cmd);
	string static get_foreign_key(string cmd);
	vector<vector<string>> static get_update_clauses(string cmd);
};

/// Converts a string to lower

/// <summary>
/// Converts a given string to lowercase.
/// </summary>
/// <param name="s">String to be converted to lowercase.</param>
/// <returns>S but lowercase.</returns>
std::string Parser::to_lower(std::string s)
{
	std::string keywords[18] = {"open", "database", "create", "db", "info",
		"table" , "drop" , "select" , "from" , "update", "delete", "insert",
		"into", "rename", "column", "where", "on", "join"};
	//first save a copy of the input
	std::string input_copy = s;

	//split the copy into a vector of substrings on every instance of ' '
	vector<string> split;
	stringstream b(s);
	std::string current;
	
	while (getline(b, current, ' '))
	{
		split.push_back(current);
	}

	vector<string> new_(split);

	//sanitize all of the substrings to check for keywords later - this prooobably will work
	std::for_each(split.begin(), split.end(), [](string& st)
		{std::for_each(st.begin(), st.end(), [](char& c)
			{ c = ::tolower(c); }); });

	//check the sanitized strings against a dictionary of keywords, if the keyword is found,
	//merge the sanitized string, if it isnt, merge the original
	std::string finout = "";
	for (int i = 0; i < split.size(); i++)
	{
		bool iskey = false;
		for (int j = 0; j < 15; j++)
		{
			if (split[i].find(keywords[j]) != string::npos)
			{
				iskey = true;
			}
		}
		if (iskey == false)
		{
			//merge in the unsanitized string
			if (i == 0)
			{
				finout.append(new_[i]);
			}
			else {
				finout.append(" ");
				finout.append(new_[i]);
			}
		}
		else
		{
			//merge in the sanitized string
			if (i == 0)
			{
				finout.append(split[i]);
			}
			else {
				finout.append(" ");
				finout.append(split[i]);
			}
		}
	}

	//std::for_each(s.begin(), s.end(), [](char& c)
	//	{ c = ::tolower(c); });

	return finout;
}

vector<vector<string>> Parser::get_update_clauses(string cmd) {
	smatch sm;
	vector<vector<string>> ret;
	vector<string> upd_clause;
	vector<string> values;

	regex str_expr("set(?:\\s*)(.*)(?:\\s*where)");

	if (regex_search(cmd, sm, str_expr)) {
		try {

			values = Utils::split(sm[1], ",");

			for (string str : values)
			{

				upd_clause = Utils::split(str, "=");

				for (size_t i = 0; i < upd_clause.size(); i++)
				{
					upd_clause[i] = Utils::trim(upd_clause[i]);
					cout << "Update Clause: " << upd_clause[i] << endl;
				}

				ret.push_back(upd_clause);
			}
		}
			catch (const std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}
	}
	else {
		cout << "No Match!" << endl;
	}

	return ret;
}

string Parser::get_foreign_key(string cmd) {
	string keyword = "on ";
	string foreignKey;
	size_t first_delim_pos = cmd.find(keyword);
	size_t end_pos_of_first_delim = first_delim_pos + keyword.length();
	size_t last_delim_pos = cmd.find(" ", end_pos_of_first_delim + 1);	
	size_t last_delim_pos2 = cmd.find(";");

	if (last_delim_pos != string::npos) {
		foreignKey = cmd.substr(end_pos_of_first_delim, last_delim_pos - end_pos_of_first_delim);
	}
	else if (last_delim_pos2 != string::npos) {
		foreignKey = cmd.substr(end_pos_of_first_delim, last_delim_pos2 - end_pos_of_first_delim);
	}
	else {

	}

	return foreignKey;
}

/// Author: Andrew
/// Date: 11-28-2021
/// Parses an update command

/// <summary>
/// Parses command line input for valid command clause i.e. splits from "=".
/// </summary>
/// <param name="cmd">Command string passed in to be parsed.</param>
/// <returns>update clause</returns>
vector<string> Parser::get_update_clause(string cmd) {
	smatch sm;
	vector<string> ret;
	vector<string> values;

	regex str_expr("set(?:\\s*)(.*)(?:\\s*where)");

	if (regex_search(cmd, sm, str_expr)) {
		try
		{
			values = Utils::split(sm[1], "=");

			for (string str : values) {
				cout << "Update Clause: " << str << endl;

				ret.push_back(Utils::trim(str));
			}

		}
		catch (const std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}

	}
	else {
		cout << "No Match!" << endl;
	}

	return ret;

}

/// Author: Andrew
/// Date: 11-28-2021
/// Get's the where clause parameters for a select or update statement

/// <summary>
/// parses "where" command and the clause to be searched for
/// </summary>
/// <param name="cmd">overall command passed in</param>
/// <param name="op">operator passed in</param>
/// <returns>returns where clause</returns>
vector<string> Parser::get_where_clause(string cmd, string op) {
	smatch sm;
	vector<string> ret;
	vector<string> tmp;

	regex str_expr("where (.*)");

	if (regex_search(cmd, sm, str_expr)) {
		try
		{
			tmp = Utils::split(Utils::remove_char(sm[1], ';'), op);

			for (string str : tmp) {				
				ret.push_back(Utils::trim(str));
			}

		}
		catch (const std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}

	}

	return ret;
}

/// <summary>
/// Creates a dictionary of keywords used in a where clause. 
/// It finds keywords and adds the following words to a vector until it hits a new keyword.
/// When it hits a new keyword then it does the process all over again.
/// It saves this in a dictionary
/// </summary>
/// <param name="cmd">the command to parse</param>
/// <returns>A dictionary with key value pairs of keywords and their following words</returns>
Dictionary Parser::get_where_clause(string cmd) {
	// check what keywords are in the string
	Parser parser;	
	vector<string> wordsFound = Parser::findKeyWords(parser.KEYWORDS, cmd);

	Dictionary dic;
	bool keyWordFound = false;
	string keyword = "";

	vector<string> subs = Utils::split(cmd);
	for (string sub : subs) {
		// if keyword found then add it to dictionary
		if (Utils::contains(parser.KEYWORDS, sub)) {
			keyWordFound = true;
			keyword = sub;
			dic.AddNewKey(sub);
		}
		else if (keyWordFound) {
			// if keyword found add current keyword value to dictionary
			dic.AddKeyValuePair(keyword, sub);
		}
	}



	return dic;
}

/// Author: Andrew
/// Date: 09-28-2021
/// Splits the provided string on the specified delimiter

/// <summary>
/// Splits a string on a delimeter
/// </summary>
/// <param name="str">string to be split</param>
/// <param name="delim">where to split it</param>
/// <returns>split string array</returns>
std::string* Parser::split_str(std::string str, char delim) {
	int size = std::count(str.begin(), str.end(), delim) + 1;
	std::string* ret = new std::string[size];

	int i = 0;
	std::stringstream ssin(str.substr(str.find(":") + 1));

	while (ssin.good() && i < size) {
		getline(ssin, ret[i], delim);
		++i;
	}

	return ret;
}



/// <summary>
/// Appears to figure out whether or not what is asked to be inserted into a table is possible
/// </summary>
/// <param name="cmd">what to do</param>
/// <param name="table_name">where to put it</param>
/// <returns>string vector of column</returns>
vector<string> Parser::get_insert_columns(string cmd, string table_name) {
	smatch sm;
	vector<string> ret;
	vector<string> tmp;
;
	regex str_expr(table_name + "(?:\\s*\\()(.*)\\)(?:\\s*)values", regex::icase);	//regex str_expr("insert into " + table_name + " \\((.*)\\)", regex::icase)

	// Check if the match was found, and add to the vector
	if (regex_search(cmd, sm, str_expr)) {
		try
		{
			tmp = Utils::split(sm[sm.size() - 1], ",");

			for (string str : tmp) {
				ret.push_back(Utils::trim(str));
			}

		}
		catch (const std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}

	}
	else {
		cout << "Insert Columns: No Match!" << endl;
	}

	return ret;
}



/// Author: Andrew
/// Date: 12-12-2021
/// Gets the conditional statement from a command

/// <summary>
/// parses command for condition
/// </summary>
/// <param name="stm">statement</param>
/// <returns>the condition passed in</returns>
string Parser::get_conditional(string stm) {
	string ret = "";

	if (Utils::contains(stm, "<=")) {
		ret = "<=";
	}
	else if (Utils::contains(stm, ">=")) {
		ret = ">=";
	}
	else if (Utils::contains(stm, "=")) {
		ret = "=";
	}
	else if (Utils::contains(stm, "<")) {
		ret = "<";
	}
	else if (Utils::contains(stm, ">")) {
		ret = ">";
	}

	return ret;
}

/// Author: Andrew Nunez
/// Date: 11-15-2021
/// Splits the provided string on the specified delimiter - again, no it doesn'MinDegree

/// <summary>
/// same as 2 above but with rows
/// </summary>
/// <param name="cmd">what to do</param>
/// <param name="table_name">which table to operate on</param>
/// <returns>string vector of row</returns>
vector<vector<string> > Parser::get_insert_rows(string cmd, string table_name) {
	smatch sm;
	vector<vector<string> > ret;
	cmd.erase(std::remove(cmd.begin(), cmd.end(), '\n'), cmd.end());

	regex str_expr(R"(values(?:\s*)\(([^()]+)\))", regex_constants::icase);

	// Check if the match was found, and add to the vector
	if (regex_search(cmd, sm, str_expr)) {
		try
		{
			vector<string> rows = Utils::split(sm[1], ")(");

			for (string row : rows) {
				row = Utils::remove_char(row, '\'');
				row = Utils::remove_char(row, '"');

				vector<string> values = Utils::split(row, ",");

				if (values.size() > 0) {
					ret.push_back(values);
				}

			}

		}
		catch (const std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}

	}
	else {
		cout << "Insert Rows: No Match!" << endl;
	}

	return ret;
}

/// Author: Andrew
/// Date: 10-28-2021
/// Splits the provided string on the specified delimiter - you guessed it, no it doesn'MinDegree

/// <summary>
/// gets a selected column from input command
/// </summary>
/// <param name="cmd">command recieved</param>
/// <returns>string vector of columns</returns>
vector<string> Parser::get_select_columns(string cmd) {
	smatch sm;
	vector<string> ret;

	regex str_expr("select (.*) from", regex_constants::icase);

	// Check if the match was found, and add to the vector
	if (regex_search(cmd, sm, str_expr)) {
		try
		{
			ret = Utils::split(sm[1], ",");

		}
		catch (const std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}

	}

	return ret;

}

/// Author: Andrew
/// Date: 10-28-2021
/// Gets the columns for create table command

/// <summary>
/// not quite certain but it gets columns while table is being created
/// </summary>
/// <param name="cmd">command passed in</param>
/// <returns>string vector of columns</returns>
vector<string> Parser::get_create_columns(string cmd) {
	smatch sm;
	vector<string> ret;

	regex str_expr("\\((.*)\\)");

	// Check if the match was found, and add to the vector
	if (regex_search(cmd, sm, str_expr)) {
		try
		{
			ret = Utils::split(sm[1], ",");

		}
		catch (const std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}

	}
	else {
		cout << "Invalid Command. Can't find Columns." << endl;
	}

	return ret;

}


std::string Parser::get_table_name(string cmd) {
	smatch sm;
	string ret;
	string keyword = "from";

	size_t found = cmd.find(keyword);
	

	return ret;

}

/// <summary>
/// table name retriever
/// </summary>
/// <param name="cmd">command to execute</param>
/// <param name="first_delim">first place to split</param>
/// <param name="second_delim">second place to split</param>
/// <returns>table name if it exists</returns>
std::string Parser::get_table_name(string cmd, string first_delim, string second_delim) {
	smatch sm;
	std::string ret;
	string exp = "";

	if (second_delim == "(") {
		exp = first_delim + "(?:\\s*)([A-Za-z0-9\\-_]*)(?:\\s*)\\(";
	}
	else {
		/*exp = first_delim + "(?:\\s*)([A-Za-z0-9\\-_]*)(?:\\s*)" + second_delim;		*/
		exp = first_delim + "(?:\\s*)([A-Za-z0-9\\-_]*)(?:\\s*)";
	}

	regex str_expr(exp, regex_constants::icase);

	// Check if the match was found, and add to the vector
	if (regex_search(cmd, sm, str_expr)) {
		try
		{			
			ret = Utils::trim(sm[1]);

		}
		catch (const std::exception& e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}

	}
	else {
		cout << "Invalid Command. Can't find table name." << endl;
	}

	return ret;
}

/// <summary>
/// splits a string based on a delimeter
/// </summary>
/// <param name="input">string to split</param>
/// <param name="delimeter">delimeter</param>
/// <returns>array of split strings</returns>
std::vector<std::string> split_text(std::string input, std::string delimeter)
{
	// Declare delimiters
	//string delimeters = " ,.-':;?()+*/%$#!\"@^&";

	// Get string split results
	auto results = Utils::split(input, delimeter);
	return results;
}


/// <summary>
	/// Finds keywords in a string and returns the keywords found
	/// </summary>
	/// <param name="keyWords">The keywords to use</param>
	/// <param name="cmd">The string to look at</param>
	/// <returns>Vector of keywords found</returns>
vector<string> Parser::findKeyWords(vector<string> keyWords, string cmd) {	
	vector<string> wordsFound;

	// check to see if keywords are in the cmd string
	for (string key : keyWords) {
		if (Utils::contains(cmd, key)) {
			// add found word to vector
			wordsFound.push_back(key);
		}
	}

	return wordsFound;

}
