#pragma once
#include <iostream>
#include <fstream>
#include "Node.h"
#include "dict.h"
#include "search.h"

using namespace std;

class CSVTrienode {
public:
    long long int count;
    vector<CSVTrienode*> children;
    CSVTrienode(){
        count=0;
        children.resize(128);
    }
    ~CSVTrienode(){
        for(auto i:children){
            if (i!=NULL){
                delete i;
            }
        }
    }
};
class CSVTrie {
public:
    CSVTrienode* root;
    CSVTrie(){
        root=new CSVTrienode();
    }
    ~CSVTrie(){
        delete root;
    }
    void insert(string& s,long long int c){
        CSVTrienode* t=root;
        for (int i=0;i<s.size();i++){
            if (t->children[s[i]]==NULL){
                t->children[s[i]]=new CSVTrienode();
            }
            t=t->children[s[i]];
        }
        t->count=c;
        
    }
    long long int cnt(string& s) const{
        CSVTrienode* t=root;
        for (int i=0;i<s.size();i++){
            if (t->children[s[i]]==NULL){
                return 0;
            }
            t=t->children[s[i]];
        }
        return t->count;
    }
    
};
class ScoreTrienode{
    public:
    int bocode,pag,par;
    float score;
    vector<ScoreTrienode*> children;
    ScoreTrienode(){
        children.resize(13);
        score=0.0;
        bocode=-1;
        pag=-1;
        par=-1;
    }
    ~ScoreTrienode(){
        for(auto i:children){
            if (i!=NULL){
                delete i;
            }
        }
    }
};
class ScoreTrie{
public:
    ScoreTrienode* root;
    ScoreTrie(){
        root=new ScoreTrienode();
    }
    ~ScoreTrie(){
        delete root;
    }
    void insert(string& s,int book_code, int page, int paragraph,float inc){
        ScoreTrienode* t=root;
        for (int i=0;i<s.size();i++){
            int j;
            if(s[i]>='0' && s[i]<='9'){
                j=s[i]-'0';
            }else{
                j=s[i]-'A'+10;
            }
            if (t->children[j]==NULL){
                t->children[j]=new ScoreTrienode();
            }
            t=t->children[j];
        }
        t->score+=inc;
        if (t->bocode==-1){
            t->bocode=book_code;
            t->pag=page;
            t->par=paragraph;
        }
    }
};
class MaxHeap{
public:
    vector<helpnode*> heap;
    MaxHeap(){

    }
    ~MaxHeap(){
        for(auto i:heap){
            if(i!=NULL){delete i;}
        }
    }
    void swc(helpnode* a,helpnode* b){
        int t1=a->bcode;
        int t2=a->page;
        int t3=a->para;
        float t4=a->sc;
        a->bcode=b->bcode;
        a->page=b->page;
        a->para=b->para;
        a->sc=b->sc;
        b->bcode=t1;
        b->page=t2;
        b->para=t3;
        b->sc=t4;
    }
    void hd(int i){
        while (true) {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int lar = i;

        if (l < heap.size() && heap[l]->sc > heap[lar]->sc) {
            lar = l;
        }

        if (r < heap.size() && heap[r]->sc > heap[lar]->sc) {
            lar = r;
        }

        if (lar != i) {
            swc(heap[i], heap[lar]);
            i = lar;
        } else {
            break;
        }
        }
    }
    void TrieTovector(ScoreTrienode* root, vector<helpnode*>& v){
    if (root->score>0.0){
        helpnode* n=new helpnode(root->bocode,root->pag,root->par,root->score);
        v.push_back(n);
    }
    for (int i=0;i<13;i++){
        if(root->children[i]!=NULL){
            TrieTovector(root->children[i],v);
        }
    }
    }
    void build(ScoreTrienode* root){
        TrieTovector(root,heap);
        int n=heap.size();
        for (int i = n / 2 - 1; i >= 0; i--) {
            hd(i);
        }
    }
    helpnode* top(){
        return heap[0];
    }
    void buildfromvector(vector<helpnode*>&v){
        int n=v.size();
            for (int i = n / 2 - 1; i >= 0; i--){
                hd(i);
        }
    }
    void del(){
        swc(heap[0],heap[heap.size()-1]);
        delete heap[heap.size()-1];
        heap.pop_back();
        hd(0);
    }
};
class QNA_tool {

//private:
//changing
    public:
    Dict d;
    CSVTrie* Csv;
    // You are free to change the implementation of this function
    string query_llm(string filename, Node* root, int k, string API_KEY, string question);
    // filename is the python file which will call ChatGPT API
    // root is the head of the linked list of paragraphs
    // k is the number of paragraphs (or the number of nodes in linked list)
    // API_KEY is the API key for ChatGPT
    // question is the question asked by the user

    // You can add attributes/helper functions here

public:

    /* Please do not touch the attributes and
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    
    QNA_tool(); // Constructor
    ~QNA_tool(); // Destructor

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);
    // This function is similar to the functions in dict and search 
    // The corpus will be provided to you via this function
    // It will be called for each sentence in the corpus

    Node* get_top_k_para(string question, int k);
    // This function takes in a question, preprocess it
    // And returns a list of paragraphs which contain the question
    // In each Node, you must set: book_code, page, paragraph (other parameters won't be checked)

    std::string get_paragraph(int book_code, int page, int paragraph);
    // Given the book_code, page number, and the paragraph number, returns the string paragraph.
    // Searches through the corpus.

    void query(string question, string filename);
    // This function takes in a question and a filename.
    // It should write the final answer to the specified filename.

    /* -----------------------------------------*/
    /* Please do not touch the code above this line */

    // You can add attributes/helper functions here
};