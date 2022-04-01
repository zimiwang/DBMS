#pragma once
#include <string>
#include <vector>
#include "utils.h"
#pragma once


using namespace std;


/// <summary>
/// A serialization class that contains function(s) for serialization 
/// </summary>
class Serialization {

private:
    // variables to save into the file
	template<typename T>
    typedef std::vector<T> mainVariables
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


public:

	/// <summary>
	/// serialize the inputted class
	/// </summary>
	/// <returns>returns 1 on success, 0 otherwise</returns>
	int serialize() 
	{
		for (std::vector<int>::iterator i = v.begin(); i != v.end(); ++i)
			cout << "term: " << i << " = " << mainVariables[i];


		return 1;
	}

	
};



/*
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

