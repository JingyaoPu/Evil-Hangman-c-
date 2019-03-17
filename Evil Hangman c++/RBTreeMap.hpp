//
//  RBTreeMap.hpp
//  Evil Hangman c++
//
//  Created by Nic Pu on 3/15/19.
//  Copyright © 2019 Jingyao Pu. All rights reserved.
//

#ifndef RBTreeMap_hpp
#define RBTreeMap_hpp

#include <stdio.h>
#include <set>
#include <string>
using namespace std;
struct Node
{
    string key;
    set<string> info;
    bool color;
    Node *left, *right, *parent;
    
    // Constructor
    Node(string key, set<string> info)
    {
        this->key = key;
        this->info = info;
        left = right = parent = NULL;
    }
};

// Class to represent Red-Black Tree
class RBTree
{
private:
    Node *root;
protected:
    void rotateLeft(Node *&, Node *&);
    void rotateRight(Node *&, Node *&);
    void fixViolation(Node *&, Node *&);
public:
    // Constructor
    RBTree() { root = NULL; }
    void insert(string key, set<string> info);
    Node* get(string key);
    Node* getMaxFamily();
    void maxFamilyHelper(Node *root,Node **max);
    void print();
    void printHelper(Node *root);
};
#endif /* RBTreeMap_hpp */
