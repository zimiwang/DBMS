#include <vector>
#include <windows.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include <regex>
#include <iostream>
#include <cerrno>
#include <conio.h>
#include <sstream>

class login {
public:

	/// <summary>
	/// Verify that the user exists and give a feedback of login.
	/// </summary>
	/// <param name="username"></param>
	/// <param name="password"></param>
	/// <returns></returns>
	int userVerification(std::string username, std::string password) {

		// Load the users.txt file
		std::ifstream read("../users.txt");

		if (read)
		{
			int accountFound = 0;

			std::string u, p;

			// Print after login successfully
			while (read >> u >> p)
			{
				if (u == username && p == password) {
					std::cout << "\nLOGIN SUCESS! Welcome to use RDBMS\n";

					std::cout << "Loading.";
					Sleep(400);
					std::cout << ".";
					Sleep(400);
					std::cout << ".";
					Sleep(400);
					std::cout << ".";
					Sleep(400);
					std::cout << ".";
					Sleep(400);
					std::cout << ".";
					Sleep(400);
					std::cout << ".";

					system("cls");
					std::cin.clear();

					accountFound = 1;
					break;
				}
			}
			if (accountFound == 0)
				return 0;
			else
				return 1;
		}
	};

	/// <summary>
	/// Print the user's login interface
	/// </summary>
	/// <returns></returns>
	int userLogin() {

		system("cls");

		std::cout << "\n------------- Student Login -------------" << std::endl;

		std::string username, password;

		std::cout << "Please enter username: ";
		std::cin >> username;
		std::cout << "Please enter password: ";
		password = passwordStar();

		int res = userVerification(username, password);

		if (res == 0)
		{
			std::cout << "\nIncorrect username or password, please press any keys for back\n";

			std::cin.get();
			std::cin.get();
		}

		return res;
	}
	
	/// <summary>
	/// Register a user and store username and password to users.txt
	/// </summary>
	/// <returns></returns>
	int userRegister() {

		std::string username, password, filename, line;
		std::ifstream read;
		int userFound = 0;

		system("cls");

		std::cout << "\n------------- User Registered -------------" << std::endl;

		std::cout << "Please enter username : ";
		std::cin >> username;
		std::cout << "Please enter password : ";
		std::cin >> password;

		read.open("../users.txt");

		if (read)
		{
			// Find if there is a username with the same name
			while (std::getline(read, line)) {

				std::size_t found = line.find(username + " ");

				if (found != std::string::npos)
				{
					userFound = 1;
					break;
				}
			}

			if (userFound == 1)
			{
				std::cout << "\nThis user has already registered. Please try another username ";
				read.close();

				std::cin.get();
				std::cin.get();

				return 0;
			}
		}
		read.close();

		//  Write the new username and password to the .txt file
		std::ofstream userFile;
		userFile.open("../users.txt", std::ios::app);

		userFile << username << ' ' << password << std::endl;
		userFile.close();

		std::cout << "\nUser Registered Successfully\n" << std::endl;

		std::cin.get();
		std::cin.get();

		return 0;
	}

	/// <summary>
	/// Set up an administrator account and the administrator account can have various permissions.
	/// Register a new user, delete a user and change password.
	/// </summary>
	/// <returns></returns>
	int addminstratorLogin() {

		system("cls");

		std::cout << "\n------------- Adminstrator Login -------------" << std::endl;

		std::string username, password;

		std::cout << "\nPlease enter username: ";
		std::cin >> username;

		std::cout << "Please enter password: ";
		password = passwordStar();

		// Default administrator account username and password
		if (username == "admit" && password == "isu4488")
		{
			int back = 0;
			while (1) {

				system("cls");

				std::cout << "\n*********** Welcome Back ***********";
				std::cout << "\n 1. Register a new user";
				std::cout << "\n 2. Delete a user";
				std::cout << "\n 3. Change password";
				std::cout << "\n 0. Back to last page \n";

				int choice;

				std::cout << "Please enter a number for your choice: \n";
				std::cin >> choice;

				switch (choice)
				{
				case 1: userRegister();
					break;
				case 2: deleteUser();
					break;
				case 3: changePassword();
					break;
				case 0: back = 1;
					break;
				default: std::cout << "\nInvalid choice. Please enter a number again \n";
					back = 1;
				}

				if (back == 1)
				{
					break;
				}
			}
		}
		else
		{
			std::cout << "\nIncorrect username or password. Please press any keys for back." << std::endl;

			std::cin.get();
			std::cin.get();
		}
		return 0;
	}

	/// <summary>
	/// Delete an existing user from users.txt
	/// </summary>
	/// <returns></returns>
	int deleteUser() {

		system("cls");

		std::cout << "\n------------- User Delete -------------" << std::endl;

		std::string username, line, temp;
		std::string data = "";
		bool isExist = false;

		std::cout << "\nPlease enter the username you want to delete: " << std::endl;
		std::cin >> username;

		std::ifstream read("../users.txt");

		if (read)
		{
			// Find if there is a user with the same username as the entered username, and save only other users except the entered username
			while (std::getline(read, line)) {

				std::istringstream is(line);
				is >> temp;
				if (temp == username) {

					isExist = true;
					continue;
				}
				else
				{
					data += (line + "\n");
				}
			}
		}

		read.close();

		// Delete user if user exists
		if (isExist)
		{
			std::ofstream os("../users.txt");
			os << data;
			os.close();
			std::cout << "\n" << username << " has been deleted successfully! " << std::endl;

			std::cin.get();
			std::cin.get();
			return 0;
		}
		else
		{
			std::cout << "\n" << username << " does not exist, please try again! " << std::endl;

			std::cin.get();
			std::cin.get();
			return 0;
		}
	}

	/// <summary>
	/// Change a password of an existing user
	/// </summary>
	/// <returns></returns>
	int changePassword() {

		system("cls");

		std::cout << "\n------------- Password Change -------------" << std::endl;

		std::string username, new_password, line, temp;
		std::string data = "";
		bool isExist = false;
		std::ifstream read("../users.txt");

		std::cout << "\nPlease enter the username: " << std::endl;
		std::cin >> username;
		std::cout << "\nPlease enter a new password: " << std::endl;
		std::cin >> new_password;

		if (read)
		{
			// Find if the entered username exists and save the new password
			while (std::getline(read, line))
			{
				std::istringstream is(line);
				is >> temp;

				if (temp == username)
				{
					if (new_password.empty()) {

						data += (line + "\n");
						isExist = false;
					}
					else
					{
						data += username + " " + new_password + "\n";
					
						isExist = true;
					}
				}
				else
				{
					data += (line + "\n");
				}
			}
		}

		// If the user exists, save the new password to the .txt file
		if (isExist)
		{
			read.close();
			std::ofstream os("../users.txt");
			os << data;
			os.close();

			std::cout << "\n" << username << " has been changed the password successfully! " << std::endl;

			std::cin.get();
			std::cin.get();
			return 0;
		}
		else
		{
			std::cout << "\n" << username << " does not exist, please try again! " << std::endl;

			std::cin.get();
			std::cin.get();
			return 0;
		}

	}

	/// <summary>
	/// Make the password appear as asterisks and use the TAB key on the keyboard to toggle the display.
	/// </summary>
	/// <returns></returns>
	std::string passwordStar() {

		std::string password;
		bool ifstar = true;
		int count = 0;
		char ch;

		while ((ch = _getch()) != 13)
		{
			// Delete the key, it will back the input by one space
			if (ch == 8)
			{
				if (!password.empty())
				{
					password.pop_back();
					putchar('\b');
					putchar(' ');
					putchar('\b');
				}
			}

			// Click the TAB key, the entered password will be displayed, and if you click it again, the asterisk will be displayed again
			else if (ch == 9) {

				if (!password.empty())
				{
					if (ifstar)
					{
						for (size_t i = 0; i < count; i++)
						{
							putchar('\b');
							putchar(' ');
							putchar('\b');
						}
						std::cout << password;
						ifstar = false;
					}

					else
					{
						for (size_t i = 0; i < count; i++)
						{
							putchar('\b');
							putchar(' ');
							putchar('\b');
						}

						for (size_t i = 0; i < count; i++)
						{
							std::cout << "*";
						}
						ifstar = true;
					}
				}
			}

			// If click SHIFT, CAPS, CTRL and ALT, then continue
			else if (ch == 16 || ch == 17 || ch == 18 || ch == 20) {
				continue;
			}

			else
			{
				password += ch;
				std::cout << "*";
				count++;
			}
		}
		return password;
	}
};