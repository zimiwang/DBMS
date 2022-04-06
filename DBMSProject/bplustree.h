#pragma once
// Searching on a B+ tree in C++
// B+ tree used From: https://www.programiz.com/dsa/b-plus-tree
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include "row.h"
#include <list>

using namespace std;
const int MAX = 3;


struct PrimaryKey {
    int key;
    Row locationPtr;
};


// BP node
class Node {
    bool IS_LEAF;
    int size;
    PrimaryKey* key;    
    Node** ptr;
    friend class BPTree;

public:
    Node() {
        key = new PrimaryKey[MAX];         // number of keys in a node
        ptr = new Node * [MAX + 1]; // array of pointers to other nodes
        for (int i = 0; i < MAX; i++) {
            ptr[i] = nullptr;
        }
    };
};

//Node::Node() {
//    key = new PrimaryKey[MAX];         // number of keys in a node
//    ptr = new Node*[MAX + 1]; // array of pointers to other nodes
//    for (int i = 0; i < MAX; i++) {
//        ptr[i] = nullptr;
//    }
//}

// BP tree
class BPTree {
    Node* root;                                         // root node
    int minKey;
    int maxKey;
    string PrimaryKeyColumn;
    vector<string> SecondaryKeyColumns;

    void insertInternal(PrimaryKey x, Node* parentNode, Node* child) {
        if (parentNode->size < MAX) {
            int i = 0;
            while (x.key > parentNode->key[i].key && i < parentNode->size)       // Find which index to add the input value in the parentNode
                i++;
            for (int j = parentNode->size; j > i; j--) {                // move keys based on input value index i
                parentNode->key[j] = parentNode->key[j - 1];            // move previous parentNode-key index to the right
            }
            for (int j = parentNode->size + 1; j > i + 1; j--) {        // move the pointers based on the input value index i
                parentNode->ptr[j] = parentNode->ptr[j - 1];            // move previous parentNode-ptr index to the right
            }
            parentNode->key[i] = x;                                     // set the key value for parentNode
            parentNode->size++;                                         // increase parentNode size
            parentNode->ptr[i + 1] = child;                             // 
        }
        else {                                                          // NOTE: should create a virtualNode instead of virtual arrays
            Node* newInternal = new Node;                               // create a newNode
            PrimaryKey virtualNode[MAX + 1];                            // create a virtualNode key array
            Node* virtualPtr[MAX + 2];                                  // 
            for (int i = 0; i < MAX; i++) {                             // copy parentNode-keys to virtualNode key array
                virtualNode[i] = parentNode->key[i];
            }
            
            for (int i = 0; i < MAX + 1; i++) {                         // copy paretnNode-ptrs to virtualNode ptr array
                virtualPtr[i] = parentNode->ptr[i];
            }
            int i = 0, j;
            while (x.key > virtualNode[i].key && i < MAX)               // Find which index to add the input value in the virtualNode
                i++;
            for (int j = MAX; j > i; j--) {                             // move keys based on input value index i
                virtualNode[j] = virtualNode[j - 1];                    // move previous virtualtNode-key index to the right
            }
            virtualNode[i] = x;                                         // set the input value in virtualNode array based on the index i
            for (int j = MAX + 1; j > i + 1; j--) {                     // 
                virtualPtr[j] = virtualPtr[j - 1];
            }
            virtualPtr[i + 1] = child;
            newInternal->IS_LEAF = false;                               // set newNode IS_LEAF 
            parentNode->size = (MAX + 1) / 2;                           // resize parentNode
            newInternal->size = MAX - (MAX + 1) / 2;                    // set size for newNode
            for (i = 0, j = parentNode->size + 1; i < newInternal->size; i++, j++) {        // assign keys to newNode from virtualNode || POSSIBLE BUG, should be j = parentNode->size, not j = parentNode->size + 1
                newInternal->key[i] = virtualNode[j];
            }
            for (i = 0, j = parentNode->size + 1; i < newInternal->size + 1; i++, j++) {
                newInternal->ptr[i] = virtualPtr[j];
            }
            if (parentNode == root) {
                Node* newRoot = new Node;                                   // create new root node
                newRoot->key[0] = parentNode->key[parentNode->size];        // assign rootNode key from parentNode key
                newRoot->ptr[0] = parentNode;                               // assign rootNode pointer from currentNoode
                newRoot->ptr[1] = newInternal;                              // assign rootNode pointer from newNode
                newRoot->IS_LEAF = false;                                   // rootNode is not a leaf node
                newRoot->size = 1;                                          // set the size of rootNode
                root = newRoot;                                             // set the rootNode as the new root
            }
            else {
                insertInternal(parentNode->key[parentNode->size], findParent(root, parentNode), newInternal);
            }
        }
    };      // insert internal function


    void removeInternal(PrimaryKey x, Node* currentNode, Node* child) {
        if (currentNode == root) {
            if (currentNode->size == 1) {
                if (currentNode->ptr[1] == child) {
                    delete[] child->key;
                    delete[] child->ptr;
                    delete child;
                    root = currentNode->ptr[0];
                    delete[] currentNode->key;
                    delete[] currentNode->ptr;
                    delete currentNode;
                    cout << "Changed root node\n";
                    return;
                }
                else if (currentNode->ptr[0] == child) {
                    delete[] child->key;
                    delete[] child->ptr;
                    delete child;
                    root = currentNode->ptr[1];
                    delete[] currentNode->key;
                    delete[] currentNode->ptr;
                    delete currentNode;
                    cout << "Changed root node\n";
                    return;
                }
            }
        }
        int pos;
        for (pos = 0; pos < currentNode->size; pos++) {
            if (currentNode->key[pos].key == x.key) {
                break;
            }
        }
        for (int i = pos; i < currentNode->size; i++) {
            currentNode->key[i] = currentNode->key[i + 1];
        }
        for (pos = 0; pos < currentNode->size + 1; pos++) {
            if (currentNode->ptr[pos] == child) {
                break;
            }
        }
        for (int i = pos; i < currentNode->size + 1; i++) {
            currentNode->ptr[i] = currentNode->ptr[i + 1];
        }
        currentNode->size--;
        if (currentNode->size >= (MAX + 1) / 2 - 1) {
            return;
        }
        if (currentNode == root)
            return;
        Node* parent = findParent(root, currentNode);
        int leftSibling, rightSibling;
        for (pos = 0; pos < parent->size + 1; pos++) {
            if (parent->ptr[pos] == currentNode) {
                leftSibling = pos - 1;
                rightSibling = pos + 1;
                break;
            }
        }
        if (leftSibling >= 0) {
            Node* leftNode = parent->ptr[leftSibling];
            if (leftNode->size >= (MAX + 1) / 2) {
                for (int i = currentNode->size; i > 0; i--) {
                    currentNode->key[i] = currentNode->key[i - 1];
                }
                currentNode->key[0] = parent->key[leftSibling];
                parent->key[leftSibling] = leftNode->key[leftNode->size - 1];
                for (int i = currentNode->size + 1; i > 0; i--) {
                    currentNode->ptr[i] = currentNode->ptr[i - 1];
                }
                currentNode->ptr[0] = leftNode->ptr[leftNode->size];
                currentNode->size++;
                leftNode->size--;
                return;
            }
        }
        if (rightSibling <= parent->size) {
            Node* rightNode = parent->ptr[rightSibling];
            if (rightNode->size >= (MAX + 1) / 2) {
                currentNode->key[currentNode->size] = parent->key[pos];
                parent->key[pos] = rightNode->key[0];
                for (int i = 0; i < rightNode->size - 1; i++) {
                    rightNode->key[i] = rightNode->key[i + 1];
                }
                currentNode->ptr[currentNode->size + 1] = rightNode->ptr[0];
                for (int i = 0; i < rightNode->size; ++i) {
                    rightNode->ptr[i] = rightNode->ptr[i + 1];
                }
                currentNode->size++;
                rightNode->size--;
                return;
            }
        }
        if (leftSibling >= 0) {
            Node* leftNode = parent->ptr[leftSibling];
            leftNode->key[leftNode->size] = parent->key[leftSibling];
            for (int i = leftNode->size + 1, j = 0; j < currentNode->size; j++) {
                leftNode->key[i] = currentNode->key[j];
            }
            for (int i = leftNode->size + 1, j = 0; j < currentNode->size + 1; j++) {
                leftNode->ptr[i] = currentNode->ptr[j];
                currentNode->ptr[j] = NULL;
            }
            leftNode->size += currentNode->size + 1;
            currentNode->size = 0;
            removeInternal(parent->key[leftSibling], parent, currentNode);
        }
        else if (rightSibling <= parent->size) {
            Node* rightNode = parent->ptr[rightSibling];
            currentNode->key[currentNode->size] = parent->key[rightSibling - 1];
            for (int i = currentNode->size + 1, j = 0; j < rightNode->size; j++) {
                currentNode->key[i] = rightNode->key[j];
            }
            for (int i = currentNode->size + 1, j = 0; j < rightNode->size + 1; j++) {
                currentNode->ptr[i] = rightNode->ptr[j];
                rightNode->ptr[j] = NULL;
            }
            currentNode->size += rightNode->size + 1;
            rightNode->size = 0;
            removeInternal(parent->key[rightSibling - 1], parent, rightNode);
        }
    };


    Node* findParent(Node* cursor, Node* child) {
        Node* parent = NULL;
        if (cursor->IS_LEAF || (cursor->ptr[0])->IS_LEAF) {
            return NULL;
        }
        for (int i = 0; i < cursor->size + 1; i++) {
            if (cursor->ptr[i] == child) {
                parent = cursor;
                return parent;
            }
            else {
                parent = findParent(cursor->ptr[i], child);
                if (parent != NULL)
                    return parent;
            }
        }
        return parent;
    };                     // find parent of given node

    Node* searchInternal(int x) {
        Node* currentNode = root;
        while (currentNode->IS_LEAF == false) {             // Go through non leaf nodes until it gets to a leaf node
            for (int i = 0; i < currentNode->size; i++) {
                if (x < currentNode->key[i].key) {
                    currentNode = currentNode->ptr[i];
                    break;
                }
                if (i == currentNode->size - 1) {
                    currentNode = currentNode->ptr[i + 1];
                    break;
                }
            }
        }
        return currentNode;
    }

public:
    string Name;                                        // name of the table

    BPTree() {
        root = NULL;
        PrimaryKeyColumn = "";
    };                                           // constructor

    /// <summary>
    /// Checks to see if the tree has a primary key
    /// </summary>
    /// <returns>Returns True if the tree has a primary key</returns>
    bool HasPrimaryKey() {
        if (PrimaryKeyColumn != "") {
            return true;
        }
        return false;
    }

    /// <summary>
    /// Sets the primary key column
    /// </summary>
    /// <param name="column">The column name</param>
    void SetPrimaryKey(string column) {
        if (column == "") {
            cout << "Corruption in the data. Incoming Column Name is empty." << endl;
        }
        else {
            PrimaryKeyColumn = column;
        }

    }

    /// <summary>
    /// Adds a secondary key column
    /// </summary>
    /// <param name="column">The column name</param>
    void AddSecondaryKey(string column) {
        if (column == "") {
            cout << "Corruption in the data. Incoming Column Name is empty." << endl;
        }
        else {
            SecondaryKeyColumns.push_back(column);
        }
    }

    /// <summary>
    /// Checks if the given column name is a primarky key
    /// </summary>
    /// <param name="column"></param>
    /// <returns>Returns true if column name is a primary key</returns>
    bool IsPrimaryKey(string column) {
        if (column == PrimaryKeyColumn) {
            return true;
        }

        return false;
    }

    /// <summary>
    /// Checks if the given column name is a secondary key
    /// </summary>
    /// <param name="column"></param>
    /// <returns>Returns true if column name is a secondary key</returns>
    bool IsSecondaryKey(string column) {
        if (find(SecondaryKeyColumns.begin(), SecondaryKeyColumns.end(), column) != SecondaryKeyColumns.end()) {
            return true;
        }

        return false;
    }

    Row search(int x) {
        Row row;
        if (root == NULL) {
            cout << "Tree is empty\n";            
            return row;
        }
        else {
            Node* currentNode = searchInternal(x);
            for (int i = 0; i < currentNode->size; i++) {       // traverse the currentNode keys 
                if (currentNode->key[i].key == x) {               
                    return currentNode->key[i].locationPtr;
                }
            }            
            return row;
        }
    };

    vector<Row> getFullTable() {
        return searchMultiple(minKey, maxKey);
    }

    vector<Row> searchMultiple(int min, int max) {
        if (root == NULL) {
            cout << "Tree is empty\n";
        }
        else {
            Node* currentNode = searchInternal(min);
            vector<Row> rows;
            int currentValue = min;
            while (currentValue < max) {
                if (!currentNode) {
                    break;
                }
                for (int i = 0; i < currentNode->size; i++) {
                    if (currentNode->key[i].key == min) {
                        // add the row
                        rows.push_back(currentNode->key[i].locationPtr);
                    }
                    else if (currentNode->key[i].key == max) {
                        // add row and exit
                        rows.push_back(currentNode->key[i].locationPtr);
                        currentValue = currentNode->key[i].key;
                        break;
                    }
                    else if (currentNode->key[i].key > min && currentNode->key[i].key < max) {
                        // add row and change current value
                        rows.push_back(currentNode->key[i].locationPtr);
                        currentValue = currentNode->key[i].key;
                    }
                    else {
                        // could not find key
                        currentValue = max + 1;
                        break;
                    }
                }
                // next node
                for (int i = 2; i < MAX + 1; i++) {
                    if (currentNode->ptr[i]) {
                        currentNode = currentNode->ptr[i];
                        break;
                    }
                    if (!currentNode->ptr[i] && i == MAX) {
                        currentNode = NULL;
                        break;
                    }
                }
            }

            return rows;
        }
    }


    void insert(int x, Row location) {
        // create root node 
        if (root == NULL) {
            root = new Node;                        // new node
            root->key[0].key = x;                   // first key in the node
            root->key[0].locationPtr = location;
            root->IS_LEAF = true;   // set IS_LEAF
            root->size = 1;         // current size = 1
            minKey = x;
            maxKey = x;
        }
        else {
            // set max and min nodes
            if (x > maxKey) {
                maxKey = x;
            }
            if (x < minKey) {
                minKey = x;
            }

            Node* currentNode = root;   // set current node to root
            Node* parent = NULL;               // parent node

            // if the curretNode is not a leaf node then decide to go left or right
            while (currentNode->IS_LEAF == false) {
                parent = currentNode;                                       // set parentNode to curretNode
                for (int i = 0; i < currentNode->size; i++) {
                    if (x < currentNode->key[i].key) {                          // set currentNode to the node on the left
                        currentNode = currentNode->ptr[i];
                        break;
                    }
                    if (i == currentNode->size - 1) {                       // set currentNode to the node on the right
                        currentNode = currentNode->ptr[i + 1];
                        break;
                    }
                }
            }
            // size of currentNode is not filled
            if (currentNode->size < MAX) {
                int i = 0;                                                  // Find which index to add the input value
                while (x > currentNode->key[i].key && i < currentNode->size)    // if the input value is greater than the currentNode key and the 
                    i++;                                                    // index i is less than the currentNode size: increase the index i
                for (int j = currentNode->size; j > i; j--) {               // move keys based on input value index i
                    currentNode->key[j] = currentNode->key[j - 1];          // move previous currentNode-key index to the right
                }
                currentNode->key[i].key = x;                                    // add new key for the input value
                currentNode->key[i].locationPtr = location;
                currentNode->size++;                                        // increase size of node

                currentNode->ptr[currentNode->size] = currentNode->ptr[currentNode->size - 1];  // 
                currentNode->ptr[currentNode->size - 1] = nullptr;
            }
            else {
                // if currentNode is filled
                Node* newNode = new Node;                               // create new node pointer
                PrimaryKey virtualNode[MAX + 1];                        // create an array of int MAX + 1
                for (int i = 0; i < MAX; i++) {
                    virtualNode[i].key = currentNode->key[i].key;       // copies the keys of the currentNode to virtualNode
                    virtualNode[i].locationPtr = currentNode->key[i].locationPtr;
                }
                int i = 0, j;                                   // Find which index to add the input value
                while (x > virtualNode[i].key && i < MAX)           // Goes through virtualNode to find the index of new input value
                    i++;
                for (int j = MAX; j > i; j--) {                 // move keys based on input vaule index i || POSSIBLE BUG: Should be j = MAX, j = MAX + 1 is out of index
                    virtualNode[j] = virtualNode[j - 1];        // move previous virtualNode-key index to the right
                }
                virtualNode[i].key = x;                             // add new key for the input value to virtualNode
                virtualNode[i].locationPtr = location;
                newNode->IS_LEAF = true;                        // assign the newNode IS_LEAF
                currentNode->size = (MAX + 1) / 2;              // reduce curretNode size
                newNode->size = MAX + 1 - (MAX + 1) / 2;        // assign newNode size

                currentNode->ptr[currentNode->size] = newNode;  // 
                newNode->ptr[newNode->size] = currentNode->ptr[MAX];
                newNode->ptr[MAX] = nullptr;
                currentNode->ptr[MAX] = nullptr;

                for (i = 0; i < currentNode->size; i++) {       // assign keys to curretNode from virtualNode for new currentNode size
                    currentNode->key[i] = virtualNode[i];
                }
                for (i = 0, j = currentNode->size; i < newNode->size; i++, j++) {   // assgin keys to newNode from virtualNode
                    newNode->key[i] = virtualNode[j];
                }
                if (currentNode == root) {
                    Node* rootNode = new Node;                  // create new root node
                    rootNode->key[0] = newNode->key[0];         // assign rootNode key from newNode key
                    rootNode->ptr[0] = currentNode;             // assign rootNode pointer from currentNoode
                    rootNode->ptr[1] = newNode;                 // assign rootNode pointer from newNode
                    rootNode->IS_LEAF = false;                  // rootNode is no longer leaf node
                    rootNode->size = 1;                         // set the size of rootNode
                    root = rootNode;
                }
                else {
                    insertInternal(newNode->key[0], parent, newNode);
                }
            }
        }
    };


    void display(Node* cursor) {
        if (cursor != NULL) {
            for (int i = 0; i < cursor->size; i++) {
                cout << cursor->key[i].key << " ";
            }
            cout << "\n";
            if (cursor->IS_LEAF != true) {
                for (int i = 0; i < cursor->size + 1; i++) {
                    display(cursor->ptr[i]);
                }
            }
        }
    };

    void remove(int x) {
        if (root == NULL) {
            cout << "Tree empty\n";
        }
        else {
            Node* currentNode = root;
            Node* parent = NULL;
            int leftSibling, rightSibling;
            // move down the tree
            while (currentNode->IS_LEAF == false) {
                for (int i = 0; i < currentNode->size; i++) {
                    parent = currentNode;
                    leftSibling = i - 1;
                    rightSibling = i + 1;
                    if (x < currentNode->key[i].key) {
                        currentNode = currentNode->ptr[i];
                        break;
                    }
                    if (i == currentNode->size - 1) {
                        leftSibling = i;
                        rightSibling = i + 2;
                        currentNode = currentNode->ptr[i + 1];
                        break;
                    }
                }
            }
            bool found = false;
            int pos;
            for (pos = 0; pos < currentNode->size; pos++) {
                if (currentNode->key[pos].key == x) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Not found\n";
                return;
            }
            for (int i = pos; i < currentNode->size; i++) {
                currentNode->key[i] = currentNode->key[i + 1];
            }
            currentNode->size--;
            if (currentNode == root) {
                for (int i = 0; i < MAX + 1; i++) {
                    currentNode->ptr[i] = NULL;
                }
                if (currentNode->size == 0) {
                    cout << "Tree died\n";
                    delete[] currentNode->key;
                    delete[] currentNode->ptr;
                    delete currentNode;
                    root = NULL;
                }
                return;
            }
            currentNode->ptr[currentNode->size] = currentNode->ptr[currentNode->size + 1];
            currentNode->ptr[currentNode->size + 1] = NULL;
            if (currentNode->size >= (MAX + 1) / 2) {
                return;
            }
            if (leftSibling >= 0) {
                Node* leftNode = parent->ptr[leftSibling];
                if (leftNode->size >= (MAX + 1) / 2 + 1) {
                    for (int i = currentNode->size; i > 0; i--) {
                        currentNode->key[i] = currentNode->key[i - 1];
                    }
                    currentNode->size++;
                    currentNode->ptr[currentNode->size] = currentNode->ptr[currentNode->size - 1];
                    currentNode->ptr[currentNode->size - 1] = NULL;
                    currentNode->key[0] = leftNode->key[leftNode->size - 1];
                    leftNode->size--;
                    leftNode->ptr[leftNode->size] = currentNode;
                    leftNode->ptr[leftNode->size + 1] = NULL;
                    parent->key[leftSibling] = currentNode->key[0];
                    return;
                }
            }
            if (rightSibling <= parent->size) {
                Node* rightNode = parent->ptr[rightSibling];
                if (rightNode->size >= (MAX + 1) / 2 + 1) {
                    currentNode->size++;
                    currentNode->ptr[currentNode->size] = currentNode->ptr[currentNode->size - 1];
                    currentNode->ptr[currentNode->size - 1] = NULL;
                    currentNode->key[currentNode->size - 1] = rightNode->key[0];
                    rightNode->size--;
                    rightNode->ptr[rightNode->size] = rightNode->ptr[rightNode->size + 1];
                    rightNode->ptr[rightNode->size + 1] = NULL;
                    for (int i = 0; i < rightNode->size; i++) {
                        rightNode->key[i] = rightNode->key[i + 1];
                    }
                    parent->key[rightSibling - 1] = rightNode->key[0];
                    return;
                }
            }
            if (leftSibling >= 0) {
                Node* leftNode = parent->ptr[leftSibling];
                for (int i = leftNode->size, j = 0; j < currentNode->size; i++, j++) {
                    leftNode->key[i] = currentNode->key[j];
                }
                leftNode->ptr[leftNode->size] = NULL;
                leftNode->size += currentNode->size;
                leftNode->ptr[leftNode->size] = currentNode->ptr[currentNode->size];
                removeInternal(parent->key[leftSibling], parent, currentNode);
                delete[] currentNode->key;
                delete[] currentNode->ptr;
                delete currentNode;
            }
            else if (rightSibling <= parent->size) {
                Node* rightNode = parent->ptr[rightSibling];
                for (int i = currentNode->size, j = 0; j < rightNode->size; i++, j++) {
                    currentNode->key[i] = rightNode->key[j];
                }
                currentNode->ptr[currentNode->size] = NULL;
                currentNode->size += rightNode->size;
                currentNode->ptr[currentNode->size] = rightNode->ptr[rightNode->size];
                cout << "Merging two leaf nodes\n";
                removeInternal(parent->key[rightSibling - 1], parent, rightNode);
                delete[] rightNode->key;
                delete[] rightNode->ptr;
                delete rightNode;
            }
        }
    };

    Node* getRoot() {
        return root;
    };

    bool operator==(const BPTree& rhs) const
    {
        return rhs.Name == this->Name;
    }
};