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
    		node(const string &leftKey, const string &rightKey, node *leftNode, node *centerNode, node *rightNode)
    			:lkey(leftKey), rkey(rightKey), left(leftNode), center(centerNode), right(rightNode) {
    				lval.resize(0);
    				rval.resize(0);
    		}
    		string lkey;       				// The node's left key
    		vector<int> lval;
  			string rkey;        			// The node's right key
  			vector<int> rval;
  			node *left;   					// Pointer to left child
  			node *center; 					// Pointer to middle child
  			node *right;					// Pointer to right child	

    		bool isLeaf() 					{ return left == nullptr; }
    		node *getLeftChild() 			{ return left; }
			node *getRightChild() 			{ return right; }
			node *getCenterChild() 			{ return center; }
			string getLeftKey() 			{ return lkey; }
			vector<int> getLeftVal()		{ return lval;}
			string getRightKey() 			{ return rkey; }
			vector<int> getRightVal()		{ return rval;}
			void setLeftChild(node * l) 	{ left = l; }
			void setCenterChild(node * c) 	{ center = c; }
			void setRightChild(node * r) 	{ right = r; }
    		node *add(node *it);
    	};

    	node *root;

    	node *insertHelper(const string &x, int line, node *t, int &distWords);
		bool containsHelper(const string & x, node *t, node* &result) const;
		void printTreeHelper(node *t, ostream & out) const;
		int  findHeight(node *t);
};

#endif	