//Description: Implementations for the 2-3 tree class.

#include "t3t.h"
#include "time.h"
#include <iomanip>
#include <sstream>

//Constructor
T3T::T3T(){
    root = NULL;
}

//Returns true if there are no nodes in the tree
bool T3T::isEmpty(){
    return root == NULL;
}

//Used to implement the search function in the main
//program.
void T3T::contains() const {
    string input;
    node *foundNode = NULL;
    cout << "Search word: ";
    cin >> input;

    if (containsHelper(input, root, foundNode)) {
        if (!foundNode->lkey.empty() && input == foundNode->lkey) {
            cout << "Line Numbers: " << foundNode->lval[0];
            for (int i = 1; i < foundNode->lval.size(); i++)
                cout << ", " << foundNode->lval[i];
            cout << '\n';
        }
        else if (!foundNode->rkey.empty())
        {
            cout << "Line Numbers: " << foundNode->rval[0];
            for (int i = 1; i < foundNode->rval.size(); i++)
                cout << ", " << foundNode->rval[i];
            cout << '\n';
        }
    }
    else
        cout << '\"' << input <<"\" is not in the document\n";
}

//Prints the index to the supplied receiver, either
//cout or the output file
void T3T::printTree(ostream & out) const {
    out << "2-3 Tree Index:\n-----------------------------------\n";
    printTreeHelper(root, out);
}

//Receives the specified input file and constructs 
//the actual tree. Prints a message when finished.
void T3T::buildTree(ifstream & input){
        int line = 1, numWords = 0, distWords = 0, treeHeight = 0;
    stringstream tempWord;
    double totalTime, finishTime, startTime = clock();
    
    while (!input.eof()) {
        string tempLine, tempWord;

        //Read a whole line of text from the file
        getline(input, tempLine);
        for (int i = 0; i < tempLine.length(); i++) {
            //Insert valid chars into tempWord until a delimiter( newline or space) is found
            while (tempLine[i] != ' '&& tempLine[i] != '\n' && i < tempLine.length() ) {
            tempWord.insert(tempWord.end(), tempLine[i]);
            i++;
            }
           
            //Trim any punctuation off end of word. Will leave things like apostrophes
            //and decimal points
            while(tempWord.length() > 0 && !isalnum(tempWord[tempWord.length() - 1]))
                tempWord.resize(tempWord.size() -1);   
            
            if (tempWord.length() > 0)
            {
                
                root = insertHelper(tempWord, line, root, distWords);
                //Increment our total number of words inserted
                numWords++;
                //Clear out tempWord so we can use it again
                tempWord.clear();
            }
            
        }
        line++;
    }

    //Do time and height calculation
    finishTime = clock();
    totalTime = (double) (finishTime - startTime)/CLOCKS_PER_SEC;
    treeHeight = findHeight(root);

    //Print output
    cout << setw(40) << std::left;
    cout << "Total number of words: " << numWords<< endl;

    cout << setw(40) << std::left 
    << "Total number of distinct words: " << distWords << endl;

    cout << setw(40) << std::left 
    <<"Total time spent building index: " << totalTime << endl;

    cout << setw(40) << std::left
    <<"Height of T3T is : " << treeHeight << endl;
}

//Used by contains() to see if a words is present or not. Will
//give contains() a pointer to the found node so that contains()
//can prints the lines the word was found on.
bool T3T::containsHelper(const string & x, node *t, node* &result) const{
    if (t == nullptr) 
        return false;                                   
    if (x.compare(t->lkey) == 0) { 
        result = t;
        return true;
    }
    if ((!t->rkey.empty()) && (x.compare(t->rkey) == 0)) {
        result = t;
        return true;
    }
    if (x.compare(t->lkey) < 0)                     
        return containsHelper(x, t->left, result);
    else if (t->rkey.empty())                       
        return containsHelper(x, t->center, result);
    else if (x.compare(t->rkey) < 0)                
        return containsHelper(x, t->center, result);
    else 
        return containsHelper(x, t->right, result); 
}

//x is the word to insert, line is the line in the text file
//the word was found at, node is the node of the tree being
//examined, and distWord is incremented if a new word is created
//and used by buildTree
T3T::node *T3T::insertHelper(const string &x, int line, node *rt, int &distWord) {
    node* retval;
    if (rt == nullptr) { 
        distWord++;
        vector<int> v;
        v.push_back(line);
        return new node(x, v, "", vector<int>(), nullptr, nullptr, nullptr);
    }

    if (x == rt->lkey) {
        rt->lval.push_back(line);
        return rt;
    }
    if (x == rt->rkey){
        rt->rval.push_back(line);
        return rt;
    }

    if (rt->isLeaf()){ 
        distWord++;
        vector<int> v;
        v.push_back(line);
        return rt->add(new node(x, v, "", vector<int>(), nullptr, nullptr, nullptr));
    }

    if (x < rt->lkey) { 
        retval = insertHelper(x, line, rt->left, distWord);
        if (retval == rt->left) return rt;
        else return rt->add(retval);
    }
    else if((rt->rkey.empty()) || (x < rt->rkey)) {
        retval = insertHelper(x, line, rt->center, distWord);
        if (retval == rt->center) return rt;
        else return rt->add(retval);
    }
    else { 
        retval = insertHelper(x, line, rt->right, distWord);
        if (retval == rt->right) return rt;
        else return rt->add(retval);
    }
}

T3T::node *T3T::node::add(node *it) {
    if (rkey.empty()) { 
        if (lkey  < it->lkey) {
          rkey = it->lkey; rval = it->lval;
          center = it->left; right = it->center;
        }
        else {
          rkey = lkey; rval = lval; right = center;
          lkey = it->lkey; lval = it->lval;
          center = it->center;
        }
        return this;
    }
    else if (lkey >= it->lkey) { 
        node *N1 = new node(lkey, lval, "", vector<int>(), it, this, nullptr);
        it->setLeftChild(left);
        left = center; center = right; right = nullptr;
        lkey = rkey; lval = rval; rkey = ""; rval = vector<int>();
        return N1;
    }
    else if (rkey >= it->lkey) {
        it->setCenterChild(new node(rkey, rval, "", vector<int>(), it->center, right, nullptr));
        it->setLeftChild(this);
        rkey = ""; rval = vector<int>();right = nullptr;
        return it;  
    }
    else { 
        node *N1 = new node(rkey, rval, "", vector<int>(), this, it, nullptr);
        it->setLeftChild(right);
        right = nullptr; rkey = ""; rval = vector<int>();
        return N1;
    }
} 

//Called by printTree(), does the actual formatted printing
void T3T::printTreeHelper(node *t, ostream & out) const{
    if(t == nullptr)
        return;
    else {
        vector<int> lval = t->lval;
        out << setw(30) << std::left;
        out << t->lkey << " " << lval[0];
        for (int i = 1; i < lval.size(); i++)
            out << ", " << lval[i];
        out << endl;
        if (!t->rkey.empty()) {
            vector<int> rval = t->rval;
            out << setw(30) << std::left;
            out << t->rkey << " " << rval[0];
            for (int i = 1; i < rval.size(); i++)
                out << ", " << rval[i];
            out << endl;
        }
        printTreeHelper(t->left, out);
        printTreeHelper(t->center, out);
        if (!t->rkey.empty()) 
            printTreeHelper(t->right, out);
    }
}

//Returns height of tree. If tree has only one node, height is 1    
int T3T::findHeight(node *t){
    if(t == nullptr)
        return 0;
    else{
        int leftHeight = findHeight(t->left), centerHeight = findHeight(t->center), rightHeight = findHeight(t->right);
        if (leftHeight > rightHeight && leftHeight > centerHeight)
            return(leftHeight + 1);
        else if (rightHeight > leftHeight && rightHeight > centerHeight)
            return(rightHeight + 1);
        else
            return(centerHeight + 1);
    }
}