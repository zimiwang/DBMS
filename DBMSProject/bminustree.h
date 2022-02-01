#pragma once
//functional b- tree for us to implement
//will be replaced by a hand-made one down the road
//source : https://www.geeksforgeeks.org/introduction-of-b-tree-2/
//emphasis : this code is NOT ours, we are using it as a placeholder for a future system

//I have changed variable names to help with readability. -Joshua Wheeler


/* The following program performs deletion on a B-Tree. It contains functions
   specific for deletion along with all the other functions provided in the
   previous articles on B-Trees. See https://www.geeksforgeeks.org/b-tree-set-1-introduction-2/
   for previous article.

   The deletion function has been compartmentalized into 8 functions for ease
   of understanding and clarity

   The following functions are exclusive for deletion
   In class BTreeNode:
    1) remove
    2) removeFromLeaf
    3) removeFromNonLeaf
    4) getPred
    5) getSucc
    6) borrowFromPrev
    7) borrowFromNext
    8) merge
    9) findKey

   In class BMinusTree:
     1) remove

  The removal of a key from a B-Tree is a fairly complicated process. The program handles
  all the 6 different cases that might arise while removing a key.

  Testing: The code has been tested using the B-Tree provided in the CLRS book( included
  in the main function ) along with other cases.

  Reference: CLRS3 - Chapter 18 - (499-502)
  It is advised to read the material in CLRS before taking a look at the code. */

#include<iostream>
using namespace std;

// A BMinusTree node
class BMinusNode
{
    int* keys;  // An array of keys
    int MinDegree;      // Minimum degree (defines the range for number of keys)
    BMinusNode** ChildPointers; // An array of child pointers
    int NumKeys;     // Current number of keys
    bool IsLeaf; // Is true when node is leaf. Otherwise false

public:

    BMinusNode(int _t, bool _leaf);   // Constructor

    // A function to traverse all nodes in a subtree rooted with this node
    void traverse();

    // A function to search a key in subtree rooted with this node.
    BMinusNode* search(int k);   // returns NULL if k is not present.

    // A function that returns the index of the first key that is greater
    // or equal to k
    int findKey(int k);

    // A utility function to insert a new key in the subtree rooted with
    // this node. The assumption is, the node must be non-full when this
    // function is called
    void insertNonFull(int k);

    // A utility function to split the child y of this node. i is index
    // of y in child array ChildPointers[].  The Child y must be full when this
    // function is called
    void splitChild(int i, BMinusNode* y);

    // A wrapper function to remove the key k in subtree rooted with
    // this node.
    void remove(int k);

    // A function to remove the key present in idx-th position in
    // this node which is a leaf
    void removeFromLeaf(int idx);

    // A function to remove the key present in idx-th position in
    // this node which is a non-leaf node
    void removeFromNonLeaf(int idx);

    // A function to get the predecessor of the key- where the key
    // is present in the idx-th position in the node
    int getPred(int idx);

    // A function to get the successor of the key- where the key
    // is present in the idx-th position in the node
    int getSucc(int idx);

    // A function to fill up the child node present in the idx-th
    // position in the ChildPointers[] array if that child has less than MinDegree-1 keys
    void fill(int idx);

    // A function to borrow a key from the ChildPointers[idx-1]-th node and place
    // it in ChildPointers[idx]th node
    void borrowFromPrev(int idx);

    // A function to borrow a key from the ChildPointers[idx+1]-th node and place it
    // in ChildPointers[idx]th node
    void borrowFromNext(int idx);

    // A function to merge idx-th child of the node with (idx+1)th child of
    // the node
    void merge(int idx);

    // Make BMinusTree friend of this so that we can access private members of
    // this class in BMinusTree functions
    friend class BMinusTree;
};

class BMinusTree
{
    BMinusNode* root; // Pointer to root node
    int t;  // Minimum degree
public:

    // Constructor (Initializes tree as empty)
    BMinusTree(int _t)
    {
        root = NULL;
        t = _t;
    }

    void traverse()
    {
        if (root != NULL) root->traverse();
    }

    // function to search a key in this tree
    BMinusNode* search(int k)
    {
        return (root == NULL) ? NULL : root->search(k);
    }

    // The main function that inserts a new key in this B-Tree
    void insert(int k);

    // The main function that removes a new key in thie B-Tree
    void remove(int k);

};

BMinusNode::BMinusNode(int t1, bool leaf1)
{
    // Copy the given minimum degree and leaf property
    MinDegree = t1;
    IsLeaf = leaf1;

    // Allocate memory for maximum number of possible keys
    // and child pointers
    keys = new int[2 * MinDegree - 1];
    ChildPointers = new BMinusNode * [2 * MinDegree];

    // Initialize the number of keys as 0
    NumKeys = 0;
}

// A utility function that returns the index of the first key that is
// greater than or equal to k
int BMinusNode::findKey(int k)
{
    int idx = 0;
    while (idx < NumKeys && keys[idx] < k)
        ++idx;
    return idx;
}

// A function to remove the key k from the sub-tree rooted with this node
void BMinusNode::remove(int k)
{
    int idx = findKey(k);

    // The key to be removed is present in this node
    if (idx < NumKeys && keys[idx] == k)
    {

        // If the node is a leaf node - removeFromLeaf is called
        // Otherwise, removeFromNonLeaf function is called
        if (IsLeaf)
            removeFromLeaf(idx);
        else
            removeFromNonLeaf(idx);
    }
    else
    {

        // If this node is a leaf node, then the key is not present in tree
        if (IsLeaf)
        {
            cout << "The key " << k << " is does not exist in the tree\n";
            return;
        }

        // The key to be removed is present in the sub-tree rooted with this node
        // The flag indicates whether the key is present in the sub-tree rooted
        // with the last child of this node
        bool flag = ((idx == NumKeys) ? true : false);

        // If the child where the key is supposed to exist has less that MinDegree keys,
        // we fill that child
        if (ChildPointers[idx]->NumKeys < MinDegree)
            fill(idx);

        // If the last child has been merged, it must have merged with the previous
        // child and so we recurse on the (idx-1)th child. Else, we recurse on the
        // (idx)th child which now has atleast MinDegree keys
        if (flag && idx > NumKeys)
            ChildPointers[idx - 1]->remove(k);
        else
            ChildPointers[idx]->remove(k);
    }
    return;
}

// A function to remove the idx-th key from this node - which is a leaf node
void BMinusNode::removeFromLeaf(int idx)
{

    // Move all the keys after the idx-th pos one place backward
    for (int i = idx + 1; i < NumKeys; ++i)
        keys[i - 1] = keys[i];

    // Reduce the count of keys
    NumKeys--;

    return;
}

// A function to remove the idx-th key from this node - which is a non-leaf node
void BMinusNode::removeFromNonLeaf(int idx)
{

    int k = keys[idx];

    // If the child that precedes k (ChildPointers[idx]) has atleast MinDegree keys,
    // find the predecessor 'pred' of k in the subtree rooted at
    // ChildPointers[idx]. Replace k by pred. Recursively delete pred
    // in ChildPointers[idx]
    if (ChildPointers[idx]->NumKeys >= MinDegree)
    {
        int pred = getPred(idx);
        keys[idx] = pred;
        ChildPointers[idx]->remove(pred);
    }

    // If the child ChildPointers[idx] has less that MinDegree keys, examine ChildPointers[idx+1].
    // If ChildPointers[idx+1] has atleast MinDegree keys, find the successor 'succ' of k in
    // the subtree rooted at ChildPointers[idx+1]
    // Replace k by succ
    // Recursively delete succ in ChildPointers[idx+1]
    else if (ChildPointers[idx + 1]->NumKeys >= MinDegree)
    {
        int succ = getSucc(idx);
        keys[idx] = succ;
        ChildPointers[idx + 1]->remove(succ);
    }

    // If both ChildPointers[idx] and ChildPointers[idx+1] has less that MinDegree keys,merge k and all of ChildPointers[idx+1]
    // into ChildPointers[idx]
    // Now ChildPointers[idx] contains 2MinDegree-1 keys
    // Free ChildPointers[idx+1] and recursively delete k from ChildPointers[idx]
    else
    {
        merge(idx);
        ChildPointers[idx]->remove(k);
    }
    return;
}

// A function to get predecessor of keys[idx]
int BMinusNode::getPred(int idx)
{
    // Keep moving to the right most node until we reach a leaf
    BMinusNode* cur = ChildPointers[idx];
    while (!cur->IsLeaf)
        cur = cur->ChildPointers[cur->NumKeys];

    // Return the last key of the leaf
    return cur->keys[cur->NumKeys - 1];
}

int BMinusNode::getSucc(int idx)
{

    // Keep moving the left most node starting from ChildPointers[idx+1] until we reach a leaf
    BMinusNode* cur = ChildPointers[idx + 1];
    while (!cur->IsLeaf)
        cur = cur->ChildPointers[0];

    // Return the first key of the leaf
    return cur->keys[0];
}

// A function to fill child ChildPointers[idx] which has less than MinDegree-1 keys
void BMinusNode::fill(int idx)
{

    // If the previous child(ChildPointers[idx-1]) has more than MinDegree-1 keys, borrow a key
    // from that child
    if (idx != 0 && ChildPointers[idx - 1]->NumKeys >= MinDegree)
        borrowFromPrev(idx);

    // If the next child(ChildPointers[idx+1]) has more than MinDegree-1 keys, borrow a key
    // from that child
    else if (idx != NumKeys && ChildPointers[idx + 1]->NumKeys >= MinDegree)
        borrowFromNext(idx);

    // Merge ChildPointers[idx] with its sibling
    // If ChildPointers[idx] is the last child, merge it with with its previous sibling
    // Otherwise merge it with its next sibling
    else
    {
        if (idx != NumKeys)
            merge(idx);
        else
            merge(idx - 1);
    }
    return;
}

// A function to borrow a key from ChildPointers[idx-1] and insert it
// into ChildPointers[idx]
void BMinusNode::borrowFromPrev(int idx)
{

    BMinusNode* child = ChildPointers[idx];
    BMinusNode* sibling = ChildPointers[idx - 1];

    // The last key from ChildPointers[idx-1] goes up to the parent and key[idx-1]
    // from parent is inserted as the first key in ChildPointers[idx]. Thus, the  loses
    // sibling one key and child gains one key

    // Moving all key in ChildPointers[idx] one step ahead
    for (int i = child->NumKeys - 1; i >= 0; --i)
        child->keys[i + 1] = child->keys[i];

    // If ChildPointers[idx] is not a leaf, move all its child pointers one step ahead
    if (!child->IsLeaf)
    {
        for (int i = child->NumKeys; i >= 0; --i)
            child->ChildPointers[i + 1] = child->ChildPointers[i];
    }

    // Setting child's first key equal to keys[idx-1] from the current node
    child->keys[0] = keys[idx - 1];

    // Moving sibling's last child as ChildPointers[idx]'s first child
    if (!child->IsLeaf)
        child->ChildPointers[0] = sibling->ChildPointers[sibling->NumKeys];

    // Moving the key from the sibling to the parent
    // This reduces the number of keys in the sibling
    keys[idx - 1] = sibling->keys[sibling->NumKeys - 1];

    child->NumKeys += 1;
    sibling->NumKeys -= 1;

    return;
}

// A function to borrow a key from the ChildPointers[idx+1] and place
// it in ChildPointers[idx]
void BMinusNode::borrowFromNext(int idx)
{

    BMinusNode* child = ChildPointers[idx];
    BMinusNode* sibling = ChildPointers[idx + 1];

    // keys[idx] is inserted as the last key in ChildPointers[idx]
    child->keys[(child->NumKeys)] = keys[idx];

    // Sibling's first child is inserted as the last child
    // into ChildPointers[idx]
    if (!(child->IsLeaf))
        child->ChildPointers[(child->NumKeys) + 1] = sibling->ChildPointers[0];

    //The first key from sibling is inserted into keys[idx]
    keys[idx] = sibling->keys[0];

    // Moving all keys in sibling one step behind
    for (int i = 1; i < sibling->NumKeys; ++i)
        sibling->keys[i - 1] = sibling->keys[i];

    // Moving the child pointers one step behind
    if (!sibling->IsLeaf)
    {
        for (int i = 1; i <= sibling->NumKeys; ++i)
            sibling->ChildPointers[i - 1] = sibling->ChildPointers[i];
    }

    // Increasing and decreasing the key count of ChildPointers[idx] and ChildPointers[idx+1]
    // respectively
    child->NumKeys += 1;
    sibling->NumKeys -= 1;

    return;
}

// A function to merge ChildPointers[idx] with ChildPointers[idx+1]
// ChildPointers[idx+1] is freed after merging
void BMinusNode::merge(int idx)
{
    BMinusNode* child = ChildPointers[idx];
    BMinusNode* sibling = ChildPointers[idx + 1];

    // Pulling a key from the current node and inserting it into (MinDegree-1)th
    // position of ChildPointers[idx]
    child->keys[MinDegree - 1] = keys[idx];

    // Copying the keys from ChildPointers[idx+1] to ChildPointers[idx] at the end
    for (int i = 0; i < sibling->NumKeys; ++i)
        child->keys[i + MinDegree] = sibling->keys[i];

    // Copying the child pointers from ChildPointers[idx+1] to ChildPointers[idx]
    if (!child->IsLeaf)
    {
        for (int i = 0; i <= sibling->NumKeys; ++i)
            child->ChildPointers[i + MinDegree] = sibling->ChildPointers[i];
    }

    // Moving all keys after idx in the current node one step before -
    // to fill the gap created by moving keys[idx] to ChildPointers[idx]
    for (int i = idx + 1; i < NumKeys; ++i)
        keys[i - 1] = keys[i];

    // Moving the child pointers after (idx+1) in the current node one
    // step before
    for (int i = idx + 2; i <= NumKeys; ++i)
        ChildPointers[i - 1] = ChildPointers[i];

    // Updating the key count of child and the current node
    child->NumKeys += sibling->NumKeys + 1;
    NumKeys--;

    // Freeing the memory occupied by sibling
    delete(sibling);
    return;
}

// The main function that inserts a new key in this B-Tree
void BMinusTree::insert(int k)
{
    // If tree is empty
    if (root == NULL)
    {
        // Allocate memory for root
        root = new BMinusNode(t, true);
        root->keys[0] = k;  // Insert key
        root->NumKeys = 1;  // Update number of keys in root
    }
    else // If tree is not empty
    {
        // If root is full, then tree grows in height
        if (root->NumKeys == 2 * t - 1)
        {
            // Allocate memory for new root
            BMinusNode* s = new BMinusNode(t, false);

            // Make old root as child of new root
            s->ChildPointers[0] = root;

            // Split the old root and move 1 key to the new root
            s->splitChild(0, root);

            // New root has two children now.  Decide which of the
            // two children is going to have new key
            int i = 0;
            if (s->keys[0] < k)
                i++;
            s->ChildPointers[i]->insertNonFull(k);

            // Change root
            root = s;
        }
        else  // If root is not full, call insertNonFull for root
            root->insertNonFull(k);
    }
}

// A utility function to insert a new key in this node
// The assumption is, the node must be non-full when this
// function is called
void BMinusNode::insertNonFull(int k)
{
    // Initialize index as index of rightmost element
    int i = NumKeys - 1;

    // If this is a leaf node
    if (IsLeaf == true)
    {
        // The following loop does two things
        // a) Finds the location of new key to be inserted
        // b) Moves all greater keys to one place ahead
        while (i >= 0 && keys[i] > k)
        {
            keys[i + 1] = keys[i];
            i--;
        }

        // Insert the new key at found location
        keys[i + 1] = k;
        NumKeys = NumKeys + 1;
    }
    else // If this node is not leaf
    {
        // Find the child which is going to have the new key
        while (i >= 0 && keys[i] > k)
            i--;

        // See if the found child is full
        if (ChildPointers[i + 1]->NumKeys == 2 * MinDegree - 1)
        {
            // If the child is full, then split it
            splitChild(i + 1, ChildPointers[i + 1]);

            // After split, the middle key of ChildPointers[i] goes up and
            // ChildPointers[i] is splitted into two.  See which of the two
            // is going to have the new key
            if (keys[i + 1] < k)
                i++;
        }
        ChildPointers[i + 1]->insertNonFull(k);
    }
}

// A utility function to split the child y of this node
// Note that y must be full when this function is called
void BMinusNode::splitChild(int i, BMinusNode* y)
{
    // Create a new node which is going to store (MinDegree-1) keys
    // of y
    BMinusNode* z = new BMinusNode(y->MinDegree, y->IsLeaf);
    z->NumKeys = MinDegree - 1;

    // Copy the last (MinDegree-1) keys of y to z
    for (int j = 0; j < MinDegree - 1; j++)
        z->keys[j] = y->keys[j + MinDegree];

    // Copy the last MinDegree children of y to z
    if (y->IsLeaf == false)
    {
        for (int j = 0; j < MinDegree; j++)
            z->ChildPointers[j] = y->ChildPointers[j + MinDegree];
    }

    // Reduce the number of keys in y
    y->NumKeys = MinDegree - 1;

    // Since this node is going to have a new child,
    // create space of new child
    for (int j = NumKeys; j >= i + 1; j--)
        ChildPointers[j + 1] = ChildPointers[j];

    // Link the new child to this node
    ChildPointers[i + 1] = z;

    // A key of y will move to this node. Find location of
    // new key and move all greater keys one space ahead
    for (int j = NumKeys - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    // Copy the middle key of y to this node
    keys[i] = y->keys[MinDegree - 1];

    // Increment count of keys in this node
    NumKeys = NumKeys + 1;
}

// Function to traverse all nodes in a subtree rooted with this node
void BMinusNode::traverse()
{
    // There are NumKeys keys and NumKeys+1 children, traverse through NumKeys keys
    // and first NumKeys children
    int i;
    for (i = 0; i < NumKeys; i++)
    {
        // If this is not leaf, then before printing key[i],
        // traverse the subtree rooted with child ChildPointers[i].
        if (IsLeaf == false)
            ChildPointers[i]->traverse();
        cout << " " << keys[i];
    }

    // Print the subtree rooted with last child
    if (IsLeaf == false)
        ChildPointers[i]->traverse();
}

// Function to search key k in subtree rooted with this node
BMinusNode* BMinusNode::search(int k)
{
    // Find the first key greater than or equal to k
    int i = 0;
    while (i < NumKeys && k > keys[i])
        i++;

    // If the found key is equal to k, return this node
    if (keys[i] == k)
        return this;

    // If key is not found here and this is a leaf node
    if (IsLeaf == true)
        return NULL;

    // Go to the appropriate child
    return ChildPointers[i]->search(k);
}

void BMinusTree::remove(int k)
{
    if (!root)
    {
        cout << "The tree is empty\n";
        return;
    }

    // Call the remove function for root
    root->remove(k);

    // If the root node has 0 keys, make its first child as the new root
    //  if it has a child, otherwise set root as NULL
    if (root->NumKeys == 0)
    {
        BMinusNode* tmp = root;
        if (root->IsLeaf)
            root = NULL;
        else
            root = root->ChildPointers[0];

        // Free the old root
        delete tmp;
    }
    return;
}