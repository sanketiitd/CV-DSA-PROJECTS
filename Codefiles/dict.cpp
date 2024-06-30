// Do NOT add any other includes
#include "dict.h"

Dict::Dict(){
    // Implement your function here
    trie=new Trie_d();    
}

Dict::~Dict(){
    // Implement your function here
    delete trie;    
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string& sentence){
    // Implement your function here
    string s="";
    int n=sentence.size();  
    for(int i=0;i<n;i++){
        if(sentence[i]==' ' || sentence[i]=='@' || sentence[i]==';' || sentence[i]==']' || sentence[i]=='[' || sentence[i]=='?' || sentence[i]==')' || sentence[i]=='(' || sentence[i]=='\'' || sentence[i]=='\"' || sentence[i]=='!' || sentence[i]==':' || sentence[i]=='-' || sentence[i]==',' || sentence[i]=='.'){
            if(s.size()>0){
                trie->insert(s,book_code,page,paragraph);
            }
            s="";
        }else{
            s+=tolower(sentence[i]);
        }
        if(i==n-1){
            if(s.size()>0){
                trie->insert(s,book_code,page,paragraph);
            }
        }
    }
    return;
}

int Dict::get_word_count(string word){
    // Implement your function here  
    return trie->cnt(word);
}

void Dict::dump_dictionary(string filename){
    // Implement your function here 
    return;
}

vector<helpnode*> Dict::get_where(string& s){
        Trienode_d* t=trie->root;
        for (int i=0;i<s.size();i++){
            if (t->children[s[i]]==NULL){
                vector<helpnode*> v;
                return v;
            }
            t=t->children[s[i]];
        }
        return t->where;
}
