// Searching on a B+ tree in C++
//Used From: https://www.programiz.com/dsa/b-plus-tree
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
const int MAX = 3;

// BP node
class Node {
    bool IS_LEAF;
    int* key, size;
    Node** ptr;
    friend class BPTree;

public:
    Node() {
        key = new int[MAX];         // number of keys in a node
        ptr = new Node * [MAX + 1]; // array of pointers to other nodes
    }
};



// BP tree
class BPTree {
    Node* root;             // root node
    void insertInternal(int, Node*, Node*); // insert internal function
    Node* findParent(Node*, Node*);         // find parent of given node

public:
    BPTree();                   // constructor
    void search(int);
    void insert(int);
    void display(Node*);
    Node* getRoot();
};

BPTree::BPTree() {
    root = NULL;
}

// Search operation
void BPTree::search(int x) {
    if (root == NULL) {
        cout << "Tree is empty\n";
    }
    else {
        Node* cursor = root;
        while (cursor->IS_LEAF == false) {
            for (int i = 0; i < cursor->size; i++) {
                if (x < cursor->key[i]) {
                    cursor = cursor->ptr[i];
                    break;
                }
                if (i == cursor->size - 1) {
                    cursor = cursor->ptr[i + 1];
                    break;
                }
            }
        }
        for (int i = 0; i < cursor->size; i++) {
            if (cursor->key[i] == x) {
                cout << "Found\n";
                return;
            }
        }
        cout << "Not found\n";
    }
}

// Insert Operation
void BPTree::insert(int x) {
    // create root node 
    if (root == NULL) {
        root = new Node;        // new node
        root->key[0] = x;       // first key in the node
        root->IS_LEAF = true;   // set IS_LEAF
        root->size = 1;         // current size = 1
    }
    else {
        Node* currentNode = root;   // set current node to root
        Node* parent;               // parent node

        // if the curretNode is not a leaf node then decide to go left or right
        while (currentNode->IS_LEAF == false) {
            parent = currentNode;                                       // set parentNode to curretNode
            for (int i = 0; i < currentNode->size; i++) {               
                if (x < currentNode->key[i]) {                          // set currentNode to the node on the left
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
            while (x > currentNode->key[i] && i < currentNode->size)    // if the input value is greater than the currentNode key and the 
                i++;                                                    // index i is less than the currentNode size: increase the index i
            for (int j = currentNode->size; j > i; j--) {               // move keys based on input value index i
                currentNode->key[j] = currentNode->key[j - 1];          // move previous currentNode-key index to the right
            }
            currentNode->key[i] = x;                                    // add new key for the input value
            currentNode->size++;                                        // increase size of node

            currentNode->ptr[currentNode->size] = currentNode->ptr[currentNode->size - 1];  // 
            currentNode->ptr[currentNode->size - 1] = NULL;
        }
        else {
            // if currentNode is filled
            Node* newNode = new Node;                       // create new node
            int virtualNode[MAX + 1];                       // create an array of int MAX + 1
            for (int i = 0; i < MAX; i++) {                 
                virtualNode[i] = currentNode->key[i];       // copies the keys of the currentNode to virtualNode
            }
            int i = 0, j;                                   // Find which index to add the input value
            while (x > virtualNode[i] && i < MAX)           // Goes through virtualNode to find the index of new input value
                i++;
            for (int j = MAX; j > i; j--) {                 // move keys based on input vaule index i || POSSIBLE BUG: Should be j = MAX, j = MAX + 1 is out of index
                virtualNode[j] = virtualNode[j - 1];        // move previous virtualNode-key index to the right
            }
            virtualNode[i] = x;                             // add new key for the input value to virtualNode
            newNode->IS_LEAF = true;                        // assign the newNode IS_LEAF
            currentNode->size = (MAX + 1) / 2;              // reduce curretNode size
            newNode->size = MAX + 1 - (MAX + 1) / 2;        // assign newNode size

            currentNode->ptr[currentNode->size] = newNode;  // 
            newNode->ptr[newNode->size] = currentNode->ptr[MAX];   
            currentNode->ptr[MAX] = NULL;

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
}

// Insert Operation
void BPTree::insertInternal(int x, Node* parentNode, Node* child) {
    if (parentNode->size < MAX) {
        int i = 0;
        while (x > parentNode->key[i] && i < parentNode->size)       // Find which index to add the input value in the parentNode
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
        int virtualNode[MAX + 1];                                   // create a virtualNode key array
        Node* virtualPtr[MAX + 2];                                  // 
        for (int i = 0; i < MAX; i++) {                             // copy parentNode-keys to virtualNode key array
            virtualNode[i] = parentNode->key[i];
        }
        for (int i = 0; i < MAX + 1; i++) {                         // copy paretnNode-ptrs to virtualNode ptr array
            virtualPtr[i] = parentNode->ptr[i];
        }
        int i = 0, j;
        while (x > virtualNode[i] && i < MAX)                       // Find which index to add the input value in the virtualNode
            i++;
        for (int j = MAX + 1; j > i; j--) {                         // move keys based on input value index i
            virtualNode[j] = virtualNode[j - 1];                    // move previous virtualtNode-key index to the right
        }
        virtualNode[i] = x;                                         // set the input value in virtualNode array based on the index i
        for (int j = MAX + 2; j > i + 1; j--) {                     // 
            virtualPtr[j] = virtualPtr[j - 1];
        }
        virtualPtr[i + 1] = child;
        newInternal->IS_LEAF = false;                               // set newNode IS_LEAF 
        parentNode->size = (MAX + 1) / 2;                           // resize parentNode
        newInternal->size = MAX - (MAX + 1) / 2;                    // set size for newNode
        for (i = 0, j = parentNode->size; i < newInternal->size; i++, j++) {        // assign keys to newNode from virtualNode || POSSIBLE BUG, should be j = parentNode->size, not j = parentNode->size + 1
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
}

//void BPTree::remove(int x) {
//    if (root == NULL) {
//        cout << "Tree empty\n";
//    }
//    else {
//        Node* cursor = root;
//        Node* parent;
//        int leftSibling, rightSibling;
//        while (cursor->IS_LEAF == false) {
//            for (int i = 0; i < cursor->size; i++) {
//                parent = cursor;
//                leftSibling = i - 1;
//                rightSibling = i + 1;
//                if (x < cursor->key[i]) {
//                    cursor = cursor->ptr[i];
//                    break;
//                }
//                if (i == cursor->size - 1) {
//                    leftSibling = i;
//                    rightSibling = i + 2;
//                    cursor = cursor->ptr[i + 1];
//                    break;
//                }
//            }
//        }
//        bool found = false;
//        int pos;
//        for (pos = 0; pos < cursor->size; pos++) {
//            if (cursor->key[pos] == x) {
//                found = true;
//                break;
//            }
//        }
//        if (!found) {
//            cout << "Not found\n";
//            return;
//        }
//        for (int i = pos; i < cursor->size; i++) {
//            cursor->key[i] = cursor->key[i + 1];
//        }
//        cursor->size--;
//        if (cursor == root) {
//            for (int i = 0; i < MAX + 1; i++) {
//                cursor->ptr[i] = NULL;
//            }
//            if (cursor->size == 0) {
//                cout << "Tree died\n";
//                delete[] cursor->key;
//                delete[] cursor->ptr;
//                delete cursor;
//                root = NULL;
//            }
//            return;
//        }
//        cursor->ptr[cursor->size] = cursor->ptr[cursor->size + 1];
//        cursor->ptr[cursor->size + 1] = NULL;
//        if (cursor->size >= (MAX + 1) / 2) {
//            return;
//        }
//        if (leftSibling >= 0) {
//            Node* leftNode = parent->ptr[leftSibling];
//            if (leftNode->size >= (MAX + 1) / 2 + 1) {
//                for (int i = cursor->size; i > 0; i--) {
//                    cursor->key[i] = cursor->key[i - 1];
//                }
//                cursor->size++;
//                cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
//                cursor->ptr[cursor->size - 1] = NULL;
//                cursor->key[0] = leftNode->key[leftNode->size - 1];
//                leftNode->size--;
//                leftNode->ptr[leftNode->size] = cursor;
//                leftNode->ptr[leftNode->size + 1] = NULL;
//                parent->key[leftSibling] = cursor->key[0];
//                return;
//            }
//        }
//        if (rightSibling <= parent->size) {
//            Node* rightNode = parent->ptr[rightSibling];
//            if (rightNode->size >= (MAX + 1) / 2 + 1) {
//                cursor->size++;
//                cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
//                cursor->ptr[cursor->size - 1] = NULL;
//                cursor->key[cursor->size - 1] = rightNode->key[0];
//                rightNode->size--;
//                rightNode->ptr[rightNode->size] = rightNode->ptr[rightNode->size + 1];
//                rightNode->ptr[rightNode->size + 1] = NULL;
//                for (int i = 0; i < rightNode->size; i++) {
//                    rightNode->key[i] = rightNode->key[i + 1];
//                }
//                parent->key[rightSibling - 1] = rightNode->key[0];
//                return;
//            }
//        }
//        if (leftSibling >= 0) {
//            Node* leftNode = parent->ptr[leftSibling];
//            for (int i = leftNode->size, j = 0; j < cursor->size; i++, j++) {
//                leftNode->key[i] = cursor->key[j];
//            }
//            leftNode->ptr[leftNode->size] = NULL;
//            leftNode->size += cursor->size;
//            leftNode->ptr[leftNode->size] = cursor->ptr[cursor->size];
//            removeInternal(parent->key[leftSibling], parent, cursor);
//            delete[] cursor->key;
//            delete[] cursor->ptr;
//            delete cursor;
//        }
//        else if (rightSibling <= parent->size) {
//            Node* rightNode = parent->ptr[rightSibling];
//            for (int i = cursor->size, j = 0; j < rightNode->size; i++, j++) {
//                cursor->key[i] = rightNode->key[j];
//            }
//            cursor->ptr[cursor->size] = NULL;
//            cursor->size += rightNode->size;
//            cursor->ptr[cursor->size] = rightNode->ptr[rightNode->size];
//            cout << "Merging two leaf nodes\n";
//            removeInternal(parent->key[rightSibling - 1], parent, rightNode);
//            delete[] rightNode->key;
//            delete[] rightNode->ptr;
//            delete rightNode;
//        }
//    }
//}
//void BPTree::removeInternal(int x, Node* cursor, Node* child) {
//    if (cursor == root) {
//        if (cursor->size == 1) {
//            if (cursor->ptr[1] == child) {
//                delete[] child->key;
//                delete[] child->ptr;
//                delete child;
//                root = cursor->ptr[0];
//                delete[] cursor->key;
//                delete[] cursor->ptr;
//                delete cursor;
//                cout << "Changed root node\n";
//                return;
//            }
//            else if (cursor->ptr[0] == child) {
//                delete[] child->key;
//                delete[] child->ptr;
//                delete child;
//                root = cursor->ptr[1];
//                delete[] cursor->key;
//                delete[] cursor->ptr;
//                delete cursor;
//                cout << "Changed root node\n";
//                return;
//            }
//        }
//    }
//    int pos;
//    for (pos = 0; pos < cursor->size; pos++) {
//        if (cursor->key[pos] == x) {
//            break;
//        }
//    }
//    for (int i = pos; i < cursor->size; i++) {
//        cursor->key[i] = cursor->key[i + 1];
//    }
//    for (pos = 0; pos < cursor->size + 1; pos++) {
//        if (cursor->ptr[pos] == child) {
//            break;
//        }
//    }
//    for (int i = pos; i < cursor->size + 1; i++) {
//        cursor->ptr[i] = cursor->ptr[i + 1];
//    }
//    cursor->size--;
//    if (cursor->size >= (MAX + 1) / 2 - 1) {
//        return;
//    }
//    if (cursor == root)
//        return;
//    Node* parent = findParent(root, cursor);
//    int leftSibling, rightSibling;
//    for (pos = 0; pos < parent->size + 1; pos++) {
//        if (parent->ptr[pos] == cursor) {
//            leftSibling = pos - 1;
//            rightSibling = pos + 1;
//            break;
//        }
//    }
//    if (leftSibling >= 0) {
//        Node* leftNode = parent->ptr[leftSibling];
//        if (leftNode->size >= (MAX + 1) / 2) {
//            for (int i = cursor->size; i > 0; i--) {
//                cursor->key[i] = cursor->key[i - 1];
//            }
//            cursor->key[0] = parent->key[leftSibling];
//            parent->key[leftSibling] = leftNode->key[leftNode->size - 1];
//            for (int i = cursor->size + 1; i > 0; i--) {
//                cursor->ptr[i] = cursor->ptr[i - 1];
//            }
//            cursor->ptr[0] = leftNode->ptr[leftNode->size];
//            cursor->size++;
//            leftNode->size--;
//            return;
//        }
//    }
//    if (rightSibling <= parent->size) {
//        Node* rightNode = parent->ptr[rightSibling];
//        if (rightNode->size >= (MAX + 1) / 2) {
//            cursor->key[cursor->size] = parent->key[pos];
//            parent->key[pos] = rightNode->key[0];
//            for (int i = 0; i < rightNode->size - 1; i++) {
//                rightNode->key[i] = rightNode->key[i + 1];
//            }
//            cursor->ptr[cursor->size + 1] = rightNode->ptr[0];
//            for (int i = 0; i < rightNode->size; ++i) {
//                rightNode->ptr[i] = rightNode->ptr[i + 1];
//            }
//            cursor->size++;
//            rightNode->size--;
//            return;
//        }
//    }
//    if (leftSibling >= 0) {
//        Node* leftNode = parent->ptr[leftSibling];
//        leftNode->key[leftNode->size] = parent->key[leftSibling];
//        for (int i = leftNode->size + 1, j = 0; j < cursor->size; j++) {
//            leftNode->key[i] = cursor->key[j];
//        }
//        for (int i = leftNode->size + 1, j = 0; j < cursor->size + 1; j++) {
//            leftNode->ptr[i] = cursor->ptr[j];
//            cursor->ptr[j] = NULL;
//        }
//        leftNode->size += cursor->size + 1;
//        cursor->size = 0;
//        removeInternal(parent->key[leftSibling], parent, cursor);
//    }
//    else if (rightSibling <= parent->size) {
//        Node* rightNode = parent->ptr[rightSibling];
//        cursor->key[cursor->size] = parent->key[rightSibling - 1];
//        for (int i = cursor->size + 1, j = 0; j < rightNode->size; j++) {
//            cursor->key[i] = rightNode->key[j];
//        }
//        for (int i = cursor->size + 1, j = 0; j < rightNode->size + 1; j++) {
//            cursor->ptr[i] = rightNode->ptr[j];
//            rightNode->ptr[j] = NULL;
//        }
//        cursor->size += rightNode->size + 1;
//        rightNode->size = 0;
//        removeInternal(parent->key[rightSibling - 1], parent, rightNode);
//    }
//}

// Find the parent
Node* BPTree::findParent(Node* cursor, Node* child) {
    Node* parent;
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
}

// Print the tree
void BPTree::display(Node* cursor) {
    if (cursor != NULL) {
        for (int i = 0; i < cursor->size; i++) {
            cout << cursor->key[i] << " ";
        }
        cout << "\n";
        if (cursor->IS_LEAF != true) {
            for (int i = 0; i < cursor->size + 1; i++) {
                display(cursor->ptr[i]);
            }
        }
    }
}

// Get the root
Node* BPTree::getRoot() {
    return root;
}

//int main() {
//    BPTree node;
//    node.insert(5);
//    node.insert(15);
//    node.insert(25);
//    node.insert(35);
//    node.insert(45);
//    node.insert(55);
//    node.insert(40);
//    node.insert(30);
//    node.insert(20);
//    node.display(node.getRoot());
//
//    node.search(15);
//}