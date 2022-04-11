// ============================================================================
//    Author: Kenneth Perkins
//    Date:   Jan 23, 2021
//    Taken From: http://programmingnotes.org/
//    File:  Utils.h
//    Description: Handles general utility functions
// ============================================================================
#pragma once
#include <vector>
#include <string>

namespace Utils {

	/// *** Unreachable Methods
	/// std::string get_string_between_string_and_space(const std::string str, const std::string start)


	std::string WHITESPACE = " \n\r\t\f\v";
	
	/// <summary>
	/// Splits a string into tokens and saves them into a vector
	/// </summary>
	/// <param name="source">String to be broken up into substrings (tokens)</param>
	/// <param name="delimiters">String containing the delimiter character(s)</param>
	/// <returns>A vector containing all the found tokens in the string</returns>
	std::vector<std::string> split(const std::string& source, const std::string& delimiters = " ") {
		std::size_t prev = 0;
		std::size_t currentPos = 0;
		std::vector<std::string> results;

		while ((currentPos = source.find_first_of(delimiters, prev)) != std::string::npos) {
			if (currentPos > prev) {
				results.push_back(source.substr(prev, currentPos - prev));
			}
			prev = currentPos + 1;
		}
		if (prev < source.length()) {
			results.push_back(source.substr(prev));
		}
		return results;
	}


	/// <summary>
	/// Rmoves the specified char from the string
	/// </summary>
	/// <param name="str">The string to be used</param>
	/// <param name="delim">The char to remove</param>
	/// <returns>The string without the char</returns>
	std::string remove_char(std::string str, char delim)
	{
		str.erase(std::remove(str.begin(), str.end(), delim), str.end());

		return str;
	}


	
	/// <summary>
	/// Trims whitespace from the left side of the string
	/// </summary>
	/// <param name="s">The string to remove whitespace from</param>
	/// <returns>The string without the whitespace</returns>
	std::string ltrim(const std::string& s)
	{
		size_t start = s.find_first_not_of(WHITESPACE);
		return (start == std::string::npos) ? "" : s.substr(start);
	}

	
	/// <summary>
	/// Trims whitespace from the right side of the string
	/// </summary>
	/// <param name="s">The string to remove whitespace from</param>
	/// <returns>The string without the whitespace</returns>
	std::string rtrim(const std::string& s)
	{
		size_t end = s.find_last_not_of(WHITESPACE);
		return (end == std::string::npos) ? "" : s.substr(0, end + 1);
	}

	
	/// <summary>
	/// Trims whitespace from both sides of the string
	/// </summary>
	/// <param name="s">The string to remove whitespace from</param>
	/// <returns>The string without the whitespace</returns>
	std::string trim(const std::string& s) {
		return rtrim(ltrim(s));
	}

	


	/// <summary>
	/// Determines if a string is a substring of another
	/// </summary>
	/// <param name="str">The string to compare</param>
	/// <param name="subStr">The substring to find</param>
	/// <returns>True if the substring is a substring of the string to compare. False otherwise</returns>
	bool contains(std::string str, std::string subStr) {
		bool ret = false;

		if (str.find(subStr) != std::string::npos) {
			ret = true;
		}

		return ret;

	}

	/// <summary>
	/// Trims all columns in a vector
	/// </summary>
	/// <param name="columns"></param>
	/// <returns></returns>
	std::vector<std::string> trimColumns(std::vector<std::string> columns) {
		std::vector<std::string> newColumns;
		for (std::string col : columns) {
			newColumns.push_back(trim(col));
		}

		return newColumns;
	}
	
	
	/// <summary>
	/// Get the first occurance of a string between the first occurence of two other strings
	/// </summary>
	/// <param name="str">The string to use</param>
	/// <param name="start">The string of where it will start</param>
	/// <param name="end">The string of where it will end</param>
	/// <returns>The first occurance of a string between the start string and the end string</returns>
	std::string get_string_between_two_strings(const std::string str, const std::string start, const std::string end)
	{
		signed first_delim_pos = str.find(start);
		unsigned end_pos_of_first_delim = first_delim_pos + start.length();
		unsigned last_delim_pos = str.find(end);

		return trim(str.substr(end_pos_of_first_delim,
			last_delim_pos - end_pos_of_first_delim));
	}

	/// ***Unreachable Method
	/// <summary>
	/// 
	/// </summary>
	/// <param name="str"></param>
	/// <param name="start"></param>
	/// <returns></returns>
	std::string get_string_between_string_and_space(const std::string str, const std::string start) {
		std::size_t first_delim_pos = str.find(start);
		std::size_t end_first_pos = first_delim_pos + start.length();
		std::size_t last_post = str.find(' ', end_first_pos);

		std::string subString = trim(str.substr(end_first_pos, last_post - end_first_pos));
		return subString;
	}

	bool contains(std::vector<std::string> vec, std::string value) {
		for (std::string sub : vec) {
			if (sub == value) {
				return true;
			}
		}
		return false;
	}
}
#pragma once
