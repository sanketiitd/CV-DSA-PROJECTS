/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "minheap.h"
//Write your code below this line

MinHeap::MinHeap(){
    size=0;
    root=NULL;
}
void heapdown(HeapNode *root){
    if(!root){
        return;
    }
    while(1){
        HeapNode* left = root->left;
        HeapNode* right = root->right;

        if (left == NULL && right == NULL) {
            break; 
        }

        HeapNode* smaller = NULL;
        if (left != NULL && (right == NULL || left->val < right->val)) {
            smaller = left;
        } else {
            smaller = right;
        }

        if (root->val > smaller->val) {
            int temp = root->val;
            root->val = smaller->val;
            smaller->val = temp;
            root = smaller; 
        } 
        else{
            break;  
        }
    }
}

void heapup(HeapNode* leaf){
    if (leaf->par == NULL) {
        return; 
    }

    if (leaf->par->val > leaf->val) {
        int c=leaf->val;
        leaf->val=leaf->par->val;
        leaf->par->val=c;
        heapup(leaf->par); 
    }
}
string lets_go(string a){
    string ans="";
    int i=0;
    while(i<a.size()){
        ans=a[i]+ans;
        i++;
    }
    return ans;
}
string bin(int k){
    string res="";
    while(k>0){
        res+=to_string(k%2);
        k/=2;
    }
    return lets_go(res);
}
void MinHeap::push_heap(int num){
    if(!root){
        HeapNode* temp=new HeapNode(num);
        root=temp;size++;
    }
    else if(root->left==NULL){
        HeapNode* temp=new HeapNode(num);
        root->left=temp;
        temp->par=root;size++;
        heapup(temp);
    }
    else if(root->right==NULL){
        HeapNode* temp2=new HeapNode(num);
        root->right=temp2;
        temp2->par=root;size++;
        heapup(temp2);
    }
    else{
        size++;
        HeapNode *tempo= new HeapNode(num);
        string a=bin(size);
        HeapNode *par=root;
        int i = 1; 
        while (i < a.length()-1) {
            if (a[i] == '0') {
                par = par->left;
        }   else {
                par = par->right;
        }
            i++;
        }
        if(par->left){
            par->right=tempo;
            tempo->par=par;
        }
        else{
            par->left=tempo;
            tempo->par=par;;
        }
        heapup(tempo);
        }
}
void MinHeap::pop(){
    string s=bin(size);
    HeapNode* temp=root;
    int i = 1; 
    while (i <= s.length() - 1) {
        if (s[i] == '0') {
            temp = temp->left;
        }
        else{
        temp = temp->right;
        }
    i++; 
    }
    if(temp->par==NULL){
        root=NULL;
        size--;
        return;
    }
    root->val=temp->val;
    if(s[s.length()-1]=='1'){
        temp->par->right=NULL;
    }
    else{
        temp->par->left=NULL;
    }
    heapdown(root);
    size--;
}


int MinHeap::get_min(){
    return root->val;
}

void del(HeapNode* root){
    if(!root){
        return;
    }
    del(root->left);
    del(root->right);
    delete root;
}
MinHeap::~MinHeap(){
    del(root);
}