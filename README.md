# CS 4488 Team project 

## Table of Contents

  - [Description](#description)
  - [Tests](#tests)
  - [Installation](#installation)
  - [Instructions](#instructions)
  - [Login](#login)
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
19. Return Sum of Column  
20. Return Average of Column  
21. Logout  

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

    select */[Column Name] from [Table1,Table2] where [Table1].[Column]=[Table2].[Column];
    
Use join clause to join two tables.

    select */[Column Name] from [Table1] join [Table2] on [Column1]=[Column2];  

Use group by clause to organize entries by like column value.  

    select * from [Table] where {clause} {group by [Column Name]};  

Use Order by clause to arrange entries in descending numerical or alphabetical order.  

    select * from [Table] where {clause} {order by [Column Name]};   

The SUM() function returns the total sum of a numeric column.  

    select * sum([column name]) from [table name]; OR  
    select sum([column name]) from [table name];  

The MIN() function returns the smallest value of the selected column.  

    select * min([column name]) from [table name]; OR
    select min([column name]) from [table name];  

The MAX() function returns the largest value of the selected column.  

    select * max([column name]) from [table name]; OR
    select max([column name]) from [table name];

The COUNT() function returns the number of rows that matches a specified criterion.  

    select * count([column name]) from [table name]; OR
    select count([column name]) from [table name];

The AVG() function returns the average value of a numeric column.  

    select * avg([column name]) from [table name]; OR
    select avg([column name]) from [table name];

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

    rename table [Existing Table Name] to [New Table Name];  

**16. Rename Table**  
Rename an existing column in a table and give it a new column name.

    alter [Table Name] rename column [Existing Column Name] to [New Column Name];  

**17. Drop Column**  
Drop an existing column in a table.  

    alter table [Table Name] drop column [Existing Column Name];  

**18. Add Key**  
Add a key to the specified table. The key type has primary, secondary, and foreign.

    alter table [Table name] add [Key Type] key [Key Name];  

**19. Return Sum of Column**
Return the sum of rows from a specified column of a specified table.

    select sum([Column Name]) from [Table Name];  

**20. Return Average of Column**
Return the average of the rows from a specified column of a specified table.

    select avg([Column Name]) from [Table Name];  

**21. Logout**  
Re-login with a new account.  

    logout;  


**Please Note**
1. All commands should end with a semicolon. Without it, the system will not be able to detect the entered command.  
2. Most commands are not case-sensitive. Table, value, and database names are case-sensitive.  
3. Some commands are space-sensitive, so please strictly follow the instructions when entering the command line.
4. When entering an existing database name, table name, and column name, check the existence of these names multiple times. In some command lines if the wrong input will cause the program to crash and terminate.  

## Login  

Our Database Management Systems have a login screen. The DBMS can only be used by logging in. Here are the functions of the login interface of this project. All accounts' usernames and passwords are stored in the users.txt file. The file type chooses to use .txt because it can better manage and view existing accounts.  

By logging in, when a user creates a database that will belong to this user, other accounts cannot open that database. If this account cannot open particular database, please use logout command and switch to another account.    

1. User Login
2. Administrator Login
    1. Register a new user
    2. Delete a user
    3. Change password

**1. User Login**  
This function can log in to a registered account, which is saved in the users.txt file. When entering the password, the password will be replaced by (*). If the user is unsure if the wrong password has been entered, the entered password can be displayed by hitting the "TAB" key on the keyboard during password entry. Hit the "TAB" key again to display the password as asterisks again.  

Once the user logs in successfully, the interface will show  

    LOGIN SUCCESS! Welcome to use RDBMS
    Loading...

After a few more seconds of waiting, it can be loaded into the DBMS.  

If the user inputs a wrong username or password, it will show

    Incorrect username or password, please press any keys for back

**2. Administrator Login**  
There is an Administrator in this login interface, and the administrator has the ability to perform other operations on all users only when the Administrator account is logged in. This is the username and password of the Administrator account.  
        
    Username: admit  
    Password: isu4488



**3. Register a new user**  
This operation can register a new account for use. All registered accounts will be saved in the users.txt file.  

If the user inputs an existing username, it will show  
    
    This user has already registered. Please try another username  

Otherwise, it will show  

    User Registered Successfully  

**4. Delete a user**  
This operation deletes an existing account from the user.txt file. This operation only requires the user to provide a username to perform the delete operation.  

Once an account is deleted, then it will show  

    [Username] has been deleted successfully!

Otherwise, it will show  

    [Username] does not exist, please try again!

**5. Change password**  
This operation can modify the password of an existing account. After entering the correct username, you can enter a new password.  

Once the username is incorrect, then it will show  

    [Username] does not exist, please try again!

**Please Note**  
Since all accounts are stored in the users.txt file, please avoid adding or deleting accounts directly in users.txt. Because this may disrupt the format in users.txt and make it impossible to register an account normally.  

If you want to operate directly in the users.txt file, after deleting or adding an account, keep a new line after the bottom account. In this way, when a new account is registered again, the username and password will not be saved after the existing account.  

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
