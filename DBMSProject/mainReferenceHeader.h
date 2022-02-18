#pragma once

// main function
/* 
*	This file should contain references to all the functions in main.cpp, so that they can be tested in tests.cpp
*/


// function initializers
void color(int s);

void setup_intro();

int exitDBMS();
int helpMenu();
int noSemiColon();
int openDatabase();
int createDatabase();
int listDatabases();
int loadSQLfile();
int dropDatabase();
int noDBopen();
int listTables();
int dbInfo();
int select();
int createTable();
int insertInto();
int tableInfo();
int dropTable();
int update();
int deleteFrom();
int renameTable();
int renameColumn();

int main(int argc, char** argv);

void color(int s);

bool has_special_char(std::string const& s);

void setup_intro();