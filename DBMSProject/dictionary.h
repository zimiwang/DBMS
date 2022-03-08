#include <string>
#include <vector>
#include "utils.h"
#pragma once

using namespace std;

/// <summary>
/// A structure that holds a key and its keyname
/// </summary>
struct MapKey {
	int key;
	string keyName;
	bool empty;
};

/// <summary>
/// Like a python dictionary, but holds a vector for the values
/// </summary>
class Dictionary {
private:
	vector<MapKey> keys;
	vector<vector<string>> values;
	int keyCount;

	/// <summary>
	/// Finds the mapkey in keys by name
	/// </summary>
	/// <param name="name">The name of the key</param>
	/// <returns>Returns the MapKey with the given name</returns>
	MapKey findMapKey(string name) {
		for (MapKey key : keys) {
			if (key.keyName == name) {
				return key;
			}
		}

		// return empty map if MapKey was not found
		MapKey map;
		map.empty = true;
		return map;
	}

public:
	Dictionary() {
		keyCount = 0;
	}

	/// <summary>
	/// Add a new key to the dictionary. This must be done before adding values to a key
	/// </summary>
	/// <param name="key">The name of the key</param>
	void AddNewKey(string key) {
		// create Map Key
		MapKey mk;
		mk.keyName = key;
		mk.key = keyCount;
		mk.empty = false;

		// add map key to keys
		keys.push_back(mk);

		// increase keyCount
		keyCount++;

		// create new vector for key
		vector<string> newVector;
		values.push_back(newVector);
	}

	/// <summary>
	/// Adds a value to an existing key
	/// </summary>
	/// <param name="key">The name of the key</param>
	/// <param name="value">The value to add</param>
	void AddKeyValuePair(string key, string value) {
		// find map key
		MapKey mk = findMapKey(key);
		// if mk found
		if (!mk.empty) {
			// insert value into array
			values[mk.key].push_back(value);
		}
		else {
			cout << "an error occured while trying to add a key value pair" << endl;
		}
	}

	/// <summary>
	/// Gets the vector of values for an existing key
	/// </summary>
	/// <param name="key">The name of the key</param>
	/// <returns>Returns a vector of values for the key</returns>
	vector<string> GetValuesByKey(string key) {
		// find map key
		MapKey mk = findMapKey(key);
		if (!mk.empty) {
			return values[mk.key];
		}

		// return empty vector
		vector<string> empty;
		return empty;
	}


};