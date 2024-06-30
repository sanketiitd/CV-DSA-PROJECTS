/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "exprtreenode.h"

//Write your code below this line

ExprTreeNode::ExprTreeNode(){
left=NULL;
right=NULL;
type="";
id="";
num=-2;
}

ExprTreeNode::ExprTreeNode(string t,int v){
  this->type="VAL";
  this->id=t;
  this->num=v;
  left=NULL;
  right=NULL;
}

ExprTreeNode::~ExprTreeNode(){
    if(this->left){
      delete this->left;
    }
    if(this->right){
      delete this->right;
    }
}

