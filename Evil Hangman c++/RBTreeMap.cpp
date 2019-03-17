//
//  RBTreeMap.cpp
//  Evil Hangman c++
//
//  Created by Nic Pu on 3/15/19.
//  Copyright © 2019 Jingyao Pu. All rights reserved.
//
#include <string>
#include <set>
#include <iostream>
#include "RBTreeMap.hpp"

using namespace std;

enum Color {RED, BLACK};




Node* getHelper(Node *root, string key)
{
    if (root == NULL)
        return nullptr;
    
    if (key < root->key)
    {
        return getHelper(root->left, key);
    }
    else if (key > root->key)
    {
        return getHelper(root->right, key);
    }
    else{
        return root;
    }
}

Node* RBTree::get(string key){
    return getHelper(this->root,key);
}

void RBTree::maxFamilyHelper(Node *root,Node **max)
{
    if (root == NULL)
        return;
    if (root->info.size() > (*max)->info.size()){
        (*max) = root;
    }
    maxFamilyHelper(root->left, max);
    maxFamilyHelper(root->right, max);
}

Node* RBTree::getMaxFamily(){
    Node *max = this->root;
    maxFamilyHelper(this->root, &max);
    return max;
}

void RBTree::printHelper(Node *root){
    if (root == NULL)
        return;
    cout<<root->key<<"      "<<root->info.size()<<endl;
    printHelper(root->left);
    printHelper(root->right);
}

void RBTree::print(){
    printHelper(this->root);
}

/* A utility function to insert a new node with given key
 in BST */
Node* BSTInsert(Node* root, Node *pt)
{
    /* If the tree is empty, return a new node */
    if (root == NULL)
        return pt;
    if (pt->key < root->key)
    {
        root->left  = BSTInsert(root->left, pt);
        root->left->parent = root;
    }
    else if (pt->key > root->key)
    {
        root->right = BSTInsert(root->right, pt);
        root->right->parent = root;
    }
    return root;
}

void RBTree::rotateLeft(Node *&root, Node *&pt)
{
    Node *pt_right = pt->right;
    
    pt->right = pt_right->left;
    
    if (pt->right != NULL)
        pt->right->parent = pt;
    
    pt_right->parent = pt->parent;
    
    if (pt->parent == NULL)
        root = pt_right;
    
    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;
    
    else
        pt->parent->right = pt_right;
    
    pt_right->left = pt;
    pt->parent = pt_right;
}

void RBTree::rotateRight(Node *&root, Node *&pt)
{
    Node *pt_left = pt->left;
    
    pt->left = pt_left->right;
    
    if (pt->left != NULL)
        pt->left->parent = pt;
    
    pt_left->parent = pt->parent;
    
    if (pt->parent == NULL)
        root = pt_left;
    
    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;
    
    else
        pt->parent->right = pt_left;
    
    pt_left->right = pt;
    pt->parent = pt_left;
}

void RBTree::fixViolation(Node *&root, Node *&pt)
{
    Node *parent_pt = NULL;
    Node *grand_parent_pt = NULL;
    
    while ((pt != root) && (pt->color != BLACK) &&
           (pt->parent->color == RED))
    {
        
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;
        
        /*
         Parent of pt is left child of Grand-parent of pt */
        if (parent_pt == grand_parent_pt->left)
        {
            
            Node *uncle_pt = grand_parent_pt->right;
            
            /*
             The uncle of pt is also red
             Only Recoloring required */
            if (uncle_pt != NULL && uncle_pt->color == RED)
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
            
            else
            {
                /*
                 pt is right child of its parent
                 Left-rotation required */
                if (pt == parent_pt->right)
                {
                    rotateLeft(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                
                /*
                 pt is left child of its parent
                 Right-rotation required */
                rotateRight(root, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
        
        /*
         Parent of pt is right child of Grand-parent of pt */
        else
        {
            Node *uncle_pt = grand_parent_pt->left;
            
            /*
             The uncle of pt is also red
             Only Recoloring required */
            if ((uncle_pt != NULL) && (uncle_pt->color == RED))
            {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
            else
            {
                /*
                 pt is left child of its parent
                 Right-rotation required */
                if (pt == parent_pt->left)
                {
                    rotateRight(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                
                /* Case : 3
                 pt is right child of its parent
                 Left-rotation required */
                rotateLeft(root, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }
    
    root->color = BLACK;
}

// Function to insert a new node with given data
void RBTree::insert(string key, set<string> info)
{
    Node *pt = new Node(key,info);
    
    // Do a normal BST insert
    root = BSTInsert(root, pt);
    
    // fix Red Black Tree violations
    fixViolation(root, pt);
}



