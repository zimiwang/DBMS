#pragma once
#include <string>
#include <vector>
#include "database.h"
#include <vector>
#include <windows.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <streambuf>
#include <list>
#include <algorithm>
#include <regex>
#include <iostream>
#include <cerrno>
#include "utils.h"
#include <typeinfo>

using namespace std;



/// <summary>
/// A serialization class that contains function(s) for serialization 
/// </summary>
class Serialization {

private:

	template <typename T>
	std::vector<T*> variableVector =
	{
		std::string database_name,
		std::vector<Table> tables,
		std::vector<Table> join_tables,
		std::vector<BTree<string>> secondaryStringTrees,
		std::vector<BTree<int>> secondaryIntTrees,
		std::vector<BTree<char>> secondaryCharTrees,
		std::vector<BPTree> primary_key_trees,
		const string PRIMARY_KEY
	};


	const std::string saveFile = "serializationTesting.txt";


	void typeSerializationHandler(const char* type)
	{
		int i;
		char c;
		char* cA;
		string s;
		if (type == typeid(i).name())
		{
			std::cout << "Look, I got an int! " << i << std::endl;
		}
		else if (type == typeid(c).name())
		{
			std::cout << "Look, I got an char! " << s << std::endl;
		}
		else if (type == typeid(cA).name())
		{
			std::cout << "Look, I got an char*! " << s << std::endl;
		}
		else if (type == typeid(s).name())
		{
			std::cout << "Look, I got an string! " << s << std::endl;
		}
		/*
		void operator () (const& obj)
		{
			std::cout << "Look, I got a UDT! And inside it a " << obj.x << std::endl;
		}
		*/
	};


public:

	/// <summary>
	/// serialize the inputted class
	/// </summary>
	int serialize()
	{
		cout << "serialize start: \n";

		// print those elements
		// use iterator with for loop
		for (auto const& e : variableVector)
		{
			typeSerializationHandler(typeid(e).name());
		}

		cout << "serialize end\n";
		return 1;
	}

	Serialization();
	~Serialization();


};




/*
//* example serialization code
#include <span class="code-string">"obj.hpp"
</span>#include <span class="code-keyword"><assert.h>
</span>#include <span class="code-keyword"><fstream>
</span>#include <span class="code-keyword"><boost/archive/text_iarchive.hpp>
</span>#include <span class="code-keyword"><boost/archive/text_oarchive.hpp>
</span>
int main()
{
  const char* fileName = "saved.txt";

  // Create one object o1.
  const Obj o1(-2, false);
  const Obj* const p1 = &o1;

  // Save data
  {
	// Create an output archive
	std::ofstream ofs(fileName);
	boost::archive::text_oarchive ar(ofs);
	// Save only the pointer. This will trigger serialization
	// of the object it points too, i.e., o1.
	ar & p1;
  }

  // Restore data
  Obj* restored_p1;
  {
	// Create and input archive
	std::ifstream ifs(fileName);
	boost::archive::text_iarchive ar(ifs);
	// Load
	ar & restored_p1;
  }

  // Make sure we read exactly what we saved.
  assert(restored_p1 != p1);
  assert(*restored_p1 == o1);

  return 0;
}
*/

