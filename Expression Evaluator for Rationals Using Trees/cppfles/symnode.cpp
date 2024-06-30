/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symnode.h"
//
//
//Write your code below this line
int htt(SymNode *a){
    if(!a){
        return -1;
    }
    return a->height;
}
SymNode::SymNode(){
key="";
height=-1;
par=NULL;
right=NULL;
left=NULL;
}

SymNode::SymNode(string k){
key=k;
height=0;
par=NULL;
right=NULL;
left=NULL;
}
int max(int a,int b){
    if(a>b){
        return a;
    }
    return b;
}
SymNode* SymNode::LeftLeftRotation(){
    SymNode *b=this;
    SymNode *a=this->left;
    SymNode *parent=this->par;
    SymNode *T2=a->right;
    //rotate
    a->right=b; // shi
    a->par=parent; //shi 
    this->par=a; // shi
    b->left=T2; 
    if(T2){
    T2->par=b;
    }
    if(a->par!=NULL && this->key<a->par->key){
        a->par->left =a;
    }else{
        if(a->par!=NULL){
            a->par->right=a;
        }
    }
    // rotate done;
    //heights;

    b->height=max(htt(b->right),htt(b->left))+1;
    a->height=max(htt(a->right),htt(a->left))+1;
    //heights done;
    return a;
}

SymNode* SymNode::RightRightRotation(){
    SymNode *a=this;
    SymNode *b=a->right;
    SymNode *T2=b->left;
    SymNode *parent=this->par;
    //rotate
    b->left=a;     //shi
    b->par=a->par; //shi
    a->par=b;  //shi
    // x ho gyaa..
    a->right=T2; //shi
    if(T2){
    T2->par=a;
    }
    if(b->par!=NULL and this->key < b->par->key){
        b->par->left=b;
    }
    else{
        if(b->par!=NULL){
            b->par->right =b;
        }
    }
    // T2 ho gyaa...
    // rotate done;
    //heights;
    a->height=max(htt(a->right),htt(a->left))+1;
    b->height=max(htt(b->right),htt(b->left))+1;
    //heights done;
    return b;
}

SymNode* SymNode::LeftRightRotation(){
   SymNode *temp=this;
   temp->left=this->left->RightRightRotation();
   temp=this->LeftLeftRotation();
   return temp;
}

SymNode* SymNode::RightLeftRotation(){
   SymNode *temp=this;
   temp->right=this->right->LeftLeftRotation();
   temp=this->RightRightRotation();
   return temp;
}

SymNode::~SymNode(){
key="";
height=-1;
right=NULL;
left=NULL;
}
