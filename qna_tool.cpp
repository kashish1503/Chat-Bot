#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;

QNA_tool::QNA_tool(){

}

QNA_tool::~QNA_tool(){
    
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    d.insert_sentence(book_code, page, paragraph, sentence_no, sentence);
}

bool is_punctuation(char c) {
    return (c == '.' || c == ',' || c == '?' || c == '!' || c == ':' || c == ';' || c == '"' || c == ' ');
}

void remove_punctuation(std::string& str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (is_punctuation(str[i])) {
            str.erase(i, 1);
            i--;
        }
    }
}

bool ispresent(string token){
    std::vector<std::string> common{"the", "of", "and", "a", "in", "to", "that", "is", "it", "was", "for", "with", "on", "as", "by", "but","at", "from", "or", "an", "he", "she", "you", "i", "we", "they", "his", "her", "its", "our", "their", "me", "us", "them", "my", "your","mine", "yours", "ours", "theirs", "this", "that", "these", "those", "which", "who", "whose", "whom", "when", "where", "why","how","more", "most", "some", "any", "many", "few", "several", "much", "little", "enough","only", "just", "even", "still", "also", "again", "ever"};
    for(auto it: common){
        if(it==token){
            return true;
        }
    }
    return false;
}

string tolower(string c) {
    string ans = "";
    for(auto it: c){
        if (it >= 'A' && it <= 'Z') {
            ans += it - 'A' + 'a';
        }else{ans += it;} 
    }
    return ans;
}

int get_count(string word){
    std::ifstream file("unigram_freq.csv");

    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return 1;
    }

    std::string header;
    if (std::getline(file, header)) {
        // You can optionally process the header if needed
    }

    // Define a map to store word frequencies
    vector<pair<string,long long>> wordFrequency;

    std::string line;

    // Read each line of the CSV file
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word, frequency;

        // Split the line into word and frequency
        if (std::getline(iss, word, ',') && std::getline(iss, frequency)) {
            long long f = stoll(frequency);
            wordFrequency.push_back({word, f});
        }
    }

    // Close the file
    file.close();

    for(auto it: wordFrequency){
        if(word == it.first){
            return it.second;
        }
    }
    return -1;
}

int QNA_tool::score_word(string word){
    int f1 = d.get_word_count(word);
    int f2 = get_count(word);
    return (f1+1)/(f2+1);
}

bool para_exist(vector<pair<int,vector<int>>> paras, int pgh, int &idx){
    int n = paras.size();
    for(int i=0; i<n; i++){
        if(paras[i].second[2]==pgh){
            idx = i;
            return true;
        }
    }
    return false;
}

void sortVector(vector<pair<int, vector<int>>>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j].first > arr[j + 1].first) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

Node* QNA_tool::get_top_k_para(string question, int k) {
    stringstream ss(question);
    vector<string> tokens; 
    std::string token;
    while (ss >> token) {
        remove_punctuation(token); 
        for (char &c : token) {
            c = tolower(c);
        }
        if(!ispresent(token)){
            tokens.push_back(token);
        }
    }
    vector<pair<int,vector<int>>> paras;
    for (auto it : tokens) {
        string word = tolower(it);
        AVLNode* node = d.search(d.getroot(), word); 
        if (node) {
            for(auto it: d.get_address(word)){
                int idx;
                bool f = para_exist(paras, it.second[2], idx);
                if(f){
                    // cout << "got1" << endl;
                    paras[idx].first += it.first*score_word(word);
                }else{
                    // cout << "got2" << endl;
                    int score = it.first*score_word(word);
                    paras.push_back({score, it.second});
                }
            }
        }
    }
    sortVector(paras);
    Node* top_k = NULL;
    Node* head = NULL;
    int i=0;
    int n = paras.size();
    if(n>k){
        while(i<k){
            Node* newNode = new Node(paras[i].second[0], paras[i].second[1], paras[i].second[2], paras[i].second[3], 0);
            if (!top_k) {
                head = newNode;
            } else {
                top_k->right = newNode;
                newNode->left = top_k;
            }
            top_k = newNode;
            i++;
        }
    }else{
        while(i<n){
            Node* newNode = new Node(paras[i].second[0], paras[i].second[1], paras[i].second[2], paras[i].second[3], 0);
            if (!top_k) {
                head = newNode;
            } else {
                top_k->right = newNode;
                newNode->left = top_k;
            }
            top_k = newNode;
            i++;
        }
    }
    top_k = head;
    return top_k;
}

void QNA_tool::query(string question, string filename){
    Node* topParagraphs = get_top_k_para(question, 5);
    // Ensure topParagraphs is not NULL
    if (topParagraphs == nullptr) {
        cerr << "Error: No relevant paragraphs found." << endl;
        return;
    }
    // Call query_llm to obtain answers
    query_llm(filename, topParagraphs, 5, "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", question);    // need to add the API key
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    
    std::string filename = "mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph)
        ){
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY, string question){

    // first write the k paragraphs into different files

    Node* traverse = root;
    int num_paragraph = 0;

    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        if(traverse!=NULL) traverse = traverse->right;
        num_paragraph++;
    }
    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
    outfile << question;
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();
 
    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}
