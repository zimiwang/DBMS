# CS 4488 Team project 

## Table of Contents

  - [Description](#description)
  - [Tests](#tests)
  - [Installation](#installation)
  - [Instructions](#instructions)
  - [Authors](#authors)
  - [License](#license)
  - [Acknowledgments](#acknowledgments)

## Project name: Extend the ISU-RDBMS

## Description 
This team project is to extend the provided ISU relational DBMS using C++ to be a full-blown DBMS. This database management system works with standard SQL.

## Tests
The current test contains operations on SQL. Among them, operations on tables and databases are the primary test objects. Includes create, insert, update, etc. The project also uses unit tests to check the correctness of each function.  

After testing, some command lines have stringent syntax requirements, easily leading to program termination. In addition to this, some command lines have potential bugs which appear because the user does not strictly follow the format of the command line. If you enter the command line according to the instructions, you will avoid the appearance of bugs.
## Installation
**Clone the repository**
  
    git clone https://github.com/SilverTaurus1/DBMSProject.git
**Navigate into the repo**
  
    cd DBMSProject
**Run the code**

    main.cpp  

## Instructions  
The database management system is capable of performing the following operations:

1. Load Database
2. Create Database
3. Open Database
4. Drop Database
5. Create Table
6. Drop Table
7. Update Table
8. Select From
9. Delete From
10. Insert Into
11. List Databases
12. List Tables
13. Table Info
14. Database Info
15. Rename Table
16. Rename Column
17. Drop Column  
18. Add Key  


**Get help**  
Get a list of all command lines and their functionality.

    help;

**1. Load Database**  
Load a SQL file into a database management system. 

    load [Database Name] [File Name];

Once the command line was run successfully, you will get feedback on your display:

    Success!

If the file name does not exist, the DBMS creates a new SQL file with the name input and loads the newly created file.  

**2. Create Database**  
Create a database with the provided name.  

    create database [Database Name];  

If the database is successfully created, the DBMS will take you to the newly created database and the terminal name will be changed to the newly created database name.  

    [Database Name]@SQL>  

Note: Don't create the database in the same name.  

**3. Open Database**  
Open an existing database.  

    open databse [Database Name];

**4. Drop Database**  
Drop an existing database. Note that database names are case-sensitive.  

    drop database [Database Name];

If the database was removed successfully, you will see the following in the terminal.  

    File successfully deleted.  

If the database does not exist, or you have used the wrong database name, then you will see the following in the terminal.  

    Error deleting file: No such file or directory  

**5. Create Table**  
Create a table in an existing database. The column types are varchar or int.  

    create table [Table Name] ([Column Name] [Column Type], [Column Name] [Column Type], ...);

One database can have multiple tables.

**6. Drop Table**  
Drop an existing table in a database. The current state needs to be entered into a database.   

    drop table [Table Name];

Once the table is dropped, the list tables command line can check whether the table was dropped.  

    list tables;

**7. Update Table**  
Update a specified value based on the provided information.  

    update [Table Name] set [Column Name] = [New value] where [Column Name] = [Existing Value];  

Update multiple values based on the provided information.

    update [Table Name] set [Column Name 1] = [New value 1], [Column Name 2] = [New value 2], ... where [Column Name] = [Existing Value];  

Note: Both the column name after set and after where must correspond to the column name in the table. Otherwise, the program will crash and stop.  

**8. Select From**  
Select table values and print to terminal. This command line can select one or multiple table columns.  

Select all columns from table.  

    select [Column Name] from [Table Name];

Select one column from table.  

    select * from [Table Name];

Select multiple columns from table.  

    select [Column Name],[Column Name],... from [Table Name];

Use where clause to select a row.  

    select */[Column Name] from [Table Name] where [Column Name] = [Existing Value];
    
Use where clause to join two tables.

    select */[Column Name] from [Table1,Table2] where [Table1].[Column]=[Table2].[Column]
    
Use join clause to join two tables.

    select */[Column Name] from [Table1] join [Table2] on [Column1]=[Column2]

Note: If you type a nonexistent column, the table will give a wrong feedback.  

**9. Delete From**  
Delete an existing row by provided column value.  

    delete from [Table Name] where [Column Name] = [Existing Value];

Note: The column name after must correspond to the column name in the table. Otherwise, the program will crash and stop.  

**10. Insert Info**  
Insert values into an existing table.   

    insert into [Table Name] ([Column Name], [Column Name]) values ([New Value], [New Value]);

Note: Capitalization of table and column names so that the DBMS can recognize the names. If the values are not inserted into the columns correctly, then the values in the columns will display NULL.  

**11. List Databases**  
List all existing databases.  

    list databases;

**12. List Tables**  
List all existing tables in a database.  

    list tables;

**13. Table Info**  
Get the table information in a database. This will print the table names, column names, and the number of rows in that table.  

    table info [Table Name];

**14. Database Info**   
Get the Database information. This will print the database name, number of tables, name of tables, and the size of database.  

    db info;

**15. Rename Table**  
Rename an existing table and give it a new table name.  

    rename table [Existing Table Name] = [New Table Name];  

**16. Rename Table**  
Rename an existing column in a table and give it a new column name.

    alter [Table Name] rename column [Existing Column Name] to [New Column Name];  

**17. Drop Column**  
Drop an existing column in a table.  

    alter table [Table Name] drop column [Existing Column Name];  

**18. Add Key**  
Add a key to the specified table. The key type has primary, secondary, and foreign.

    alter table [Table name] add [Key Type] key [Key Name];  

**19. Sum rows**
Sum the rows of a specific column of a table.

    select * sum([column name]) from [table name];

**Please Note**
1. All commands should end with a semicolon. Without it, the system will not be able to detect the entered command.  
2. Most commands are not case-sensitive. Table, value, and database names are case-sensitive.  
3. Some commands are space-sensitive, so please strictly follow the instructions when entering the command line.
4. When entering an existing database name, table name, and column name, check the existence of these names multiple times. In some command lines if the wrong input will cause the program to crash and terminate.  

## Authors

  - **Daniel Cano** - *Team Lead* -  
    - Submit code
    - Make sure team is finishing tasks
    - Create and assign tasks
    - Keep track of project schedule

  - **Joshua Wheeler** - *Project Manager* -  
    - Liason to the user
    - Submits bi-weekly reports on team progress

  - **Nicholas Garrett** - *Humanoid Resources* -  
    - Take attendance
    - Firing employees (team members)
    - Track hours worked by team members
    - Track meeting length and frequency
  - **Ziming Wang** - *Record Clerk* -  
    - Document the code
    - Version control
    - Operating Instructions
    - Conduct routine tests to ensure project integrity
## License
MIT License

Copyright (c) [2022]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Acknowledgments

  - Hat tip to anyone whose code was used
  - Inspiration
  - etc
