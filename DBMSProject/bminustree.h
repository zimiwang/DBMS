// template from https://www.programiz.com/dsa/b-tree


#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include "row.h"
#include <list>

using namespace std;

template <typename T> struct Key {
    T key;
    Row row;
};


template <class T> class TreeNode {
    Key<T>* keys;
    int MAX;
    TreeNode** C;

    int size;
    bool leaf;

public:
    TreeNode(int t1, bool leaf1) {
        MAX = t1;
        leaf = leaf1;

        keys = new Key[2 * MAX - 1];
        C = new TreeNode * [2 * MAX];

        size = 0;
    }

    void insertNonFull(T k) {
        int i = size - 1;

        if (leaf == true) {
            while (i >= 0 && keys[i]->t1 > k) {
                keys[i + 1] = keys[i];
                i--;
            }

            keys[i + 1] = k;
            size = size + 1;
        }
        else {
            while (i >= 0 && keys[i] > k)
                i--;

            if (C[i + 1]->size == 2 * MAX - 1) {
                splitChild(i + 1, C[i + 1]);

                if (keys[i + 1] < k)
                    i++;
            }
            C[i + 1]->insertNonFull(k);
        }
    }


    void splitChild(int i, TreeNode* y) {
        TreeNode* z = new TreeNode(y->t, y->leaf);
        z->size = MAX - 1;

        for (int j = 0; j < MAX - 1; j++)
            z->keys[j] = y->keys[j + t];

        if (y->leaf == false) {
            for (int j = 0; j < t; j++)
                z->C[j] = y->C[j + t];
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
        while (i < size && k > keys[i]->key)
            i++;

        if (keys[i]->key == k)
            return this;

        if (leaf == true)
            return NULL;

        return C[i]->search(k);
    }

    friend class BTree;
};



template <class T> class BTree {
    TreeNode* root;
    T t;

public:
    BTree();

    BTree(T temp) {
        root = NULL;
        int MAX = temp;
    }

    void traverse() {
        if (root != NULL)
            root->traverse();
    }

    Row search(T k) {
        return (root == NULL) ? NULL : root->search(k);
    }

    void insert(T k, Row row) {
        if (root == NULL) {
            // create new tree node
            root = new TreeNode(MAX, true);
            root->keys[0].key = k;              // create new key
            root->keys[0].row = row;
            root->size = 1;
        }
        else {
            if (root->size == 2 * MAX - 1) {
                TreeNode* s = new TreeNode(t, false);

                s->C[0] = root;

                s->splitChild(0, root);

                int i = 0;
                if (s->keys[0] < k)
                    i++;
                s->C[i]->insertNonFull(k);

                root = s;
            }
            else
                root->insertNonFull(k);
        }
    }
};




