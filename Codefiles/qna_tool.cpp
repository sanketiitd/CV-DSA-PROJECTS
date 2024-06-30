#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;


bool is_present(vector<string> noob, string word)
{
    for(auto it:noob){
        if(it==word){
            return true;
        }
    }
    return false;
}

QNA_tool::QNA_tool(){
    // Implement your function here
    Csv=new CSVTrie();
    fstream fin;
    fin.open("unigram_freq.csv",ios::in);
    string word,c;
    getline(fin,word,',');
    getline(fin,c);
    while(getline(fin,word,',')){
        getline(fin,c);
        Csv->insert(word,stoll(c));
    }  
}

QNA_tool::~QNA_tool(){
    // Implement your function here  
    delete Csv;
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here  
    d.Dict::insert_sentence(book_code,page,paragraph,sentence_no,sentence); 
    return;
}

Node* QNA_tool::get_top_k_para(string question, int k) {
    // Implement your function here
    ScoreTrie* T=new ScoreTrie();
    string s="";
    int n=question.size();  
    for(int i=0;i<n;i++){
        if(question[i]==' ' || question[i]=='@' || question[i]==';' || question[i]==']' || question[i]=='[' || question[i]=='?' || question[i]==')' || question[i]=='(' || question[i]=='\'' || question[i]=='\"' || question[i]=='!' || question[i]==':' || question[i]=='-' || question[i]==',' || question[i]=='.'){
            if(s.size()>0){
                vector<helpnode*> v=d.get_where(s);
                double inc=(double(d.get_word_count(s))+1.0)/(double(Csv->cnt(s))+1.0);
                for (auto j:v){
                    string sc='A'+to_string(j->bcode)+'B'+to_string(j->page)+'C'+to_string(j->para);
                    T->insert(sc,j->bcode,j->page,j->para,inc);
                }
            }
            s="";
        }else{
            s+=tolower(question[i]);
        }
        if(i==n-1){
            if(s.size()>0){
                vector<helpnode*> v=d.get_where(s);
                double inc=(double(d.get_word_count(s))+1.0)/(double(Csv->cnt(s))+1.0);
                for (auto j:v){
                    string sc='A'+to_string(j->bcode)+'B'+to_string(j->page)+'C'+to_string(j->para);
                    T->insert(sc,j->bcode,j->page,j->para,inc);
                }
            }
        }
    }
    MaxHeap H;
    H.build(T->root);
    delete T;
    Node* head=new Node(H.top()->bcode,H.top()->page,H.top()->para,1,0);
    head->left=NULL;
    head->right=NULL;
    Node* tail=head;
    H.del();
    for (int i=0;i<k-1;i++){
        Node* n=new Node(H.top()->bcode,H.top()->page,H.top()->para,1,0);
        H.del();
        tail->right=n;
        n->left=tail;
        n->right=NULL;
        tail=n;
    }
    return head;
}

void QNA_tool::query(string question, string filename){
            // vector<string> noob={"what","are","the","of","on","mahatma"};
             string gpt_out=query_llm(filename, NULL, 0, "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","I have a large corpus of books, and I want to find the top k paragraphs most relevant to the query '" + question + "'. Give a list of keywords (which are single words) that I can use to find the top k paragraphs. just give the string of words nothing else only the string of words relevant to it. give only a few words, and dont give irrelevant words." );
            //string gpt_out=query_llm(filename, NULL, 0, "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","I have a large corpus of books, and I want to find the top k paragraphs most relevant to the query -: '" + question + "'. Give a list of keywords (which are single words) that I can use to find the top k paragraphs. They may or may not be in the query. However, it is essential that they are relevant only to the query which i have given in single inverted commas'. just give a comma separated list of words and nothing else. dont give irrelevant words. In addition to this, also add the past participle and present participle of relevant verbs to the list" );

            // if(gpt_out=="0"){
            //     cout << "hloooooooooooooo";
            // }
            string question_2=question+'.'+gpt_out;
            // cout << "question_2------>"<<question_2 <<endl;
            vector<string> noob={"i","mahatma","views","me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", "yours", "yourself", "yourselves", "he", "him", "his", "himself", "she", "her", "hers", "herself", "it", "its", "itself", "they", "them", "their", "theirs", "themselves", "what", "which", "who", "whom", "this", "that", "these", "those", "am", "is", "are", "was", "were", "be", "been", "being", "have", "has", "had", "having", "do", "does", "did", "a", "an", "the", "and", "but", "if", "or", "because", "as", "until", "while", "of", "at", "by", "for", "with", "about","into", "through","before", "after", "above", "below", "to", "from", "up", "down", "in", "out", "on", "off", "again", "further", "then", "once", "all", "any", "both", "each", "few", "more", "most", "other", "some", "such", "no", "nor", "not", "only", "own", "same", "so", "than", "too", "very","can", "will", "just", "should", "now"};
            ScoreTrie* T=new ScoreTrie();
            string s="";
            int n=question_2.size();  
            for(int i=0;i<n;i++){
            if(question_2[i]==' ' || question_2[i]=='@' || question_2[i]==';' || question_2[i]==']' || question_2[i]=='[' || question_2[i]=='?' || question_2[i]==')' || question_2[i]=='(' || question_2[i]=='\'' || question_2[i]=='\"' || question_2[i]=='!' || question_2[i]==':' || question_2[i]=='-' || question_2[i]==',' || question_2[i]=='.'){
            if(s.size()>0 && !is_present(noob,s)){
                vector<helpnode*> v=d.get_where(s);
                double inc=(double(d.get_word_count(s))+1.0)/(double(Csv->cnt(s))+1.0);
                for (auto j:v){
                    string sc='A'+to_string(j->bcode)+'B'+to_string(j->page)+'C'+to_string(j->para);
                    T->insert(sc,j->bcode,j->page,j->para,inc);
                }
            }
            s="";
        }else{
            s+=tolower(question_2[i]);
        }
        if(i==n-1){
            if(s.size()>0 and !is_present(noob,s)){
                vector<helpnode*> v=d.get_where(s);
                double inc=(double(d.get_word_count(s))+1.0)/(double(Csv->cnt(s))+1.0);
                for (auto j:v){
                    string sc='A'+to_string(j->bcode)+'B'+to_string(j->page)+'C'+to_string(j->para);
                    T->insert(sc,j->bcode,j->page,j->para,inc);
                }
            }
        }
    }

        MaxHeap H;
        H.build(T->root);
        delete T;
        Node* head=new Node(H.top()->bcode,H.top()->page,H.top()->para,1,0);
        int ur_mom=H.heap.size();
        head->left=NULL;
        head->right=NULL;
        Node* tail=head;
        H.del();
        for (int i=0;i<4;i++){
        Node* n=new Node(H.top()->bcode,H.top()->page,H.top()->para,1,0);
        // cout <<" bk->" <<n->book_code<<"para->" << n->paragraph<< "page->" << n->page << endl;
        // cout << get_paragraph(n->book_code,n->page,n->paragraph) <<endl<<endl;
        H.del();
        tail->right=n;
        n->left=tail;
        n->right=NULL;
        tail=n;
    }
    cout<<query_llm(filename,head,5, "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", " Answer the following question on the basis of the text: " + question);

// -----------------------------------------------------------------------------------------   work area    ------------------------------------------------------------------------------//     std::cout << "Q: " << question << std::endl;
    // std::cout << "A: " << "Studying COL106 :)" << std::endl;
    return;

}
std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){

    //cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    
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

string QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY, string question){

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
        traverse = traverse->right;
        num_paragraph++;
    }

//     // write the query to query.txt
//     ofstream outfile("query.txt");
//     // outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
//     outfile << question;
//     // You can add anything here - show all your creativity and skills of using ChatGPT
//     outfile.close();
 
//     // you do not need to necessarily provide k paragraphs - can configure yourself

//     // python3 <filename> API_KEY num_paragraphs query.txt
//     string command = "python3 ";
//     command += filename;
//     command += " ";
//     command += API_KEY;
//     command += " ";
//     command += to_string(k);
//     command += " ";
//     command += "query.txt > answer.txt";  // Redirect output to answer.txt

//     system(command.c_str());

//     // Read the answer from the file into a string
//     ifstream answerFile("answer.txt");
//     stringstream buffer;
//     buffer << answerFile.rdbuf();
//     string answer = buffer.str();

//     // Print or use the answer as needed
//     cout << "Answer:->>>>>>>>>>>>>>>>>>>>>>>> " << answer << endl;

//     return answer;
// }
// ... (your code to prepare paragraphs)

    // write the query to query.txt
    ofstream queryFile("query.txt");
    queryFile << question;
    queryFile.close();
    // Run the ChatGPT API request
    string command = "python3 ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt > answer.txt";  // Redirect output to answer.txt

    system(command.c_str());

    // Read the answer from the file into a string
    ifstream answerFile("answer.txt");
    stringstream buffer;
    buffer << answerFile.rdbuf();
    string answer = buffer.str();

    // Print or use the answer as needed
    return answer;
}

