// Do NOT add any other includes
#include "dict.h"

AVLNode::AVLNode(){
    word = "";
    count = 0;
    address = {};
    left = NULL;
    right = NULL;
    height = 0;
}

Dict::Dict(){
    root = new AVLNode();  
}

int Dict::getHeight(AVLNode* node) {
    return root->height;
}

AVLNode* Dict::getroot(){
    return root;
}

void inorderUtil(AVLNode* node){
    if (node == nullptr) {
        return;
    }
    inorderUtil(node->left);
    cout << node->word << " ";
    inorderUtil(node->right);
}

void Dict::inorder(){
    AVLNode* node = root;
    inorderUtil(node);
}

AVLNode* Dict::rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}

AVLNode* Dict::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}

int Dict::getBalanceFactor(AVLNode* node) {
    if (node == nullptr) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

string toLower(string c) {
    string ans = "";
    for(auto it: c){
        if (it >= 'A' && it <= 'Z') {
            ans += it - 'A' + 'a';
        }else{ans += it;} 
    }
    return ans;
}

AVLNode* Dict::insert(AVLNode* node, string word, int book_code, int page, int paragraph, int sentence_no) {
    word = toLower(word);
    if (node == nullptr) {
        AVLNode* newNode = new AVLNode();
        newNode->word = word;
        newNode->count = 1;
        newNode->address.push_back({1,{book_code, page, paragraph, sentence_no}});
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->height = 1;
        return newNode;
    }
    if (word < node->word)
        node->left = insert(node->left, word, book_code, page, paragraph, sentence_no);
    else if (word > node->word)
        node->right = insert(node->right, word, book_code, page, paragraph, sentence_no);
    else {
        node->count++;
        int n = node->address.size();
        if(node->address[n-1].second[2]!=paragraph){
            node->address.push_back({1,{book_code, page, paragraph, sentence_no}});
        }else{
            node->address[n-1].first++;
        }
        return node;
    }
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalanceFactor(node);
    if (balance > 1 && word < node->left->word)
        return rightRotate(node);
    if (balance < -1 && word > node->right->word)
        return leftRotate(node);
    if (balance > 1 && word > node->left->word) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && word < node->right->word) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

AVLNode* Dict::search(AVLNode* node, string word) {
    if (node == nullptr || node->word == word) {
        return node;
    }
    if (word < node->word) {
        return search(node->left, word);
    }
    return search(node->right, word);
}

void Delete(AVLNode* root){
    if(root != NULL){
        Delete(root->left);
        Delete(root->right);
        delete root;
    }
}

Dict::~Dict(){
    Delete(root);
    root = NULL;
}

bool isPunctuation(char c) {
    static const std::string punctuation = " .,-:!\"\'()?-[]“”‘’˙;@";
    return punctuation.find(c) != std::string::npos;
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    std::string word = "";
    for (char c : sentence) {
        if (isPunctuation(c)) { 
            if (!word.empty()) {
                insert(root, word, book_code, page, paragraph, sentence_no);
                word = "";
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        insert(root, word, book_code, page, paragraph, sentence_no);
    }
    // inorder();
    // cout << endl;
    return;
}

int Dict::get_word_count(string word){
    word = toLower(word);
    if(search(root, word)){
        AVLNode* node = search(root, word);
        return node->count;
    }
    return 0;
}

vector<pair<int,vector<int>>> Dict::get_address(string word){
    word = toLower(word);
    if(search(root, word)){
        AVLNode* node = search(root, word);
        return node->address;
    }
    return {};
}

void inOrderDump(AVLNode* node, std::ofstream& outFile) {
    if (node == nullptr) {
        return;
    }
    inOrderDump(node->left, outFile);
    toLower(node->word);
    outFile << node->word << ", " << node->count << std::endl;
    inOrderDump(node->right, outFile);
}

void Dict::dump_dictionary(string filename){
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Failed to open output file: " << filename << endl;
        return;
    }
    inOrderDump(root->left, outFile);
    inOrderDump(root->right, outFile);
    outFile.close();
}
