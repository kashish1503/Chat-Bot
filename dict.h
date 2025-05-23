// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class AVLNode{
    public:
        string word;
        int count;
        vector<pair<int,vector<int>>> address;
        AVLNode* left;
        AVLNode* right;
        int height;
        AVLNode();
};



class Dict {
private:
    // You can add attributes/helper functions here
    AVLNode* root;
    int getHeight(AVLNode* node);
    int getBalanceFactor(AVLNode* node);
    
    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);
    AVLNode* insert(AVLNode* node, string word, int book_code, int page, int paragraph, int sentence_no);
    
    void inorder();

public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    int get_word_count(string word);

    void dump_dictionary(string filename);
    
// changed functions

    AVLNode* getroot();

    AVLNode* search(AVLNode* node, string word);

    vector<pair<int,vector<int>>> get_address(string word);

    /* -----------------------------------------*/
};
