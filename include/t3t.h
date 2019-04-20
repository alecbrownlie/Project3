//Description: Prototypes for the 2-3 tree class.

#ifndef T3T_H
#define T3T_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class T3T {
    public: 
        T3T();
        void contains() const;
        bool isEmpty();
        void printTree(ostream &out = cout) const;
        void buildTree(ifstream &input);
    private:
        struct node {
            node() {center = left = right = nullptr; }

            node(const string &lk, const string &rk, node* p1, node* p2, node* p3)
                :lkey(lk), rkey(rk), left(p1), center(p2), right(p3) { 
                    lval.resize(0); rval.resize(0);
                }

            string lkey;                    // The node's left key
            vector<int> lval;               // The tree's left index
            string rkey;                    // The node's right key
            vector<int> rval;               // The tree's right index

            node *left;                     // Pointer to left child
            node *center;                   // Pointer to middle child
            node *right;                    // Pointer to right child   

            bool isLeaf()                   { return left == nullptr; }
            void setLeftChild(node * l)     { left = l; }
            void setCenterChild(node * c)   { center = c; }
            node *add(node *it);
        };

        node *root;

        node *insertHelper(const string &x, int line, node *t, int &distWords);
        bool containsHelper(const string & x, node *t, node* &result) const;
        void printTreeHelper(node *t, ostream & out) const;
        int  findHeight(node *t);
};

#endif  