#pragma once

// Searching a key on a B-tree in C++

#include <iostream>
#include "row.h"
using namespace std;


template <typename T> struct Key {
    T key;
    Row row;
};


template <typename T> class TreeNode {
    Key<T>* keys;
    int MAX = 3;
    TreeNode** C;
    int size;
    bool leaf;

public:
    TreeNode(bool leaf1) {        
        leaf = leaf1;

        keys = new Key<T>[2 * MAX - 1];
        C = new TreeNode<T> * [2 * MAX];

        size = 0;
    }

    void insertNonFull(T k, Row row) {
        int i = size - 1;

        if (leaf == true) {
            while (i >= 0 && keys[i].key > k) {
                keys[i + 1] = keys[i];
                i--;
            }

            keys[i + 1].key = k;
            keys[i + 1].row = row;
            size = size + 1;
        }
        else {
            while (i >= 0 && keys[i].key > k)
                i--;

            if (C[i + 1]->size == 2 * MAX - 1) {
                splitChild(i + 1, C[i + 1]);

                if (keys[i + 1].key < k)
                    i++;
            }
            C[i + 1]->insertNonFull(k, row);
        }
    }

    void splitChild(int i, TreeNode<T>* y) {
        TreeNode* z = new TreeNode(y->leaf);
        z->size = MAX - 1;
        

        for (int j = 0; j < MAX - 1; j++)
            z->keys[j] = y->keys[j + MAX];

        if (y->leaf == false) {
            for (int j = 0; j < MAX; j++)
                z->C[j] = y->C[j + MAX];
        }

        y->size = MAX - 1;
        for (int j = size; j >= i + 1; j--)
            C[j + 1] = C[j];

        C[i + 1] = z;

        for (int j = size - 1; j >= i; j--)
            keys[j + 1] = keys[j];

        keys[i] = y->keys[MAX - 1];
        size = size + 1;
    }

    
    void traverse() {
        int i;
        for (i = 0; i < size; i++) {
            if (leaf == false)
                C[i]->traverse();
            cout << " " << keys[i];
        }

        if (leaf == false)
            C[i]->traverse();
    }

    Row search(T k) {
        int i = 0;
        while (i < size && k > keys[i].key)
            i++;

        if (keys[i].key == k)
            return keys[i].row;

        if (leaf == true) {
            Row row;
            return row;
        }
            

        return C[i]->search(k);
    }
    template <typename T>
    friend class BTree;
};


template <typename T> class BTree {
    TreeNode<T>* root;
    int MAX = 3;
    bool empty;
    string keyName;
public:

    void SetKeyName(string name) {
        keyName = name;
    }

    string GetKeyName() {
        return keyName;
    }

    bool IsEmpty() {
        return empty;
    }

    BTree() {
        root = NULL;        
        empty = true;
    }

    void traverse() {
        if (root != NULL)
            root->traverse();
    }

    Row search(T k) {
        if (root == NULL) {
            // create empty row
            Row row;
            return row;
        }
        else {
            return root->search(k);
        }
        
    }

    template <typename T>
    void insert(T k, Row row) {
        if (root == NULL) {
            root = new TreeNode<T>(true);
            root->keys[0].key = k;
            root->keys[0].row = row;
            root->size = 1;
            empty = false;
        }
        else {
            if (root->size == 2 * MAX - 1) {
                TreeNode<T>* s = new TreeNode<T>(false);

                s->C[0] = root;

                s->splitChild(0, root);

                int i = 0;
                if (s->keys[0].key < k)
                    i++;
                s->C[i]->insertNonFull(k, row);

                root = s;
            }
            else
                root->insertNonFull(k, row);
        }
    }
};




