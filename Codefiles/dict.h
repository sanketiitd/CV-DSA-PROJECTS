// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class helpnode{
    public:
    int bcode,page,para;
    float sc;
    helpnode(int b,int p,int pr,float f=0.0){
        bcode=b;
        page=p;
        para=pr;
        sc=f;
    }
};
class Trienode_d {
public:
    int count;
    vector<helpnode*> where;
    vector<Trienode_d*> children;
    Trienode_d(){
        count=0;
        children.resize(128);
    }
    ~Trienode_d(){
        for(auto i:children){
            if (i!=NULL){
                delete i;
            }
        }
        for(auto i:where){            
            delete i;
        }
    }
};

class Trie_d {
public:
    Trienode_d* root;
    Trie_d(){
        root=new Trienode_d();
    }
    ~Trie_d(){
        delete root;
    }
    void insert(string& s,int book_code, int page, int paragraph){
        Trienode_d* t=root;
        for (int i=0;i<s.size();i++){
            if (t->children[s[i]]==NULL){
                t->children[s[i]]=new Trienode_d();
            }
            t=t->children[s[i]];
        }
        t->count++;
        helpnode* x=new helpnode(book_code,page,paragraph);
        t->where.push_back(x);
    }
    int cnt(string& s) const{
        Trienode_d* t=root;
        for (int i=0;i<s.size();i++){
            if (t->children[s[i]]==NULL){
                return 0;
            }
            t=t->children[s[i]];
        }
        return t->count;
    }
    
};
class Dict {
private:
    // You can add attributes/helper functions here
    Trie_d* trie;
        
public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict();

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string& sentence);

    int get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
    vector<helpnode*> get_where(string& s);
};
