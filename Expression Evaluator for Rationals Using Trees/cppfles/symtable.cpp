/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "symtable.h"
//
//
//Write your code below this line
SymbolTable::SymbolTable(){
    size=0;
    root=NULL;
}
int ht(SymNode *a){
    if(!a){
        return -1;
    }
    return max(ht(a->left),ht(a->right))+1;
}
int maxi(int a,int b){
    if(a>b){
        return a;
    }
    return b;
}
int balance(SymNode *needy){
    if(!needy){
        return -1;
    } 
    return ht(needy->left)-ht(needy->right);
}
void htchange(SymNode *t){
     if(!t){
        return ;
     }
     t->height=max(ht(t->left),ht(t->right))+1; 
     htchange(t->par);
}
SymNode* inos(SymNode *node){
        while(node->left){
            node=node->left;
        }
        return node;
}
SymNode *insertkro(SymNode *root,string k){
    if(root==NULL){
         SymNode *temp=new SymNode(k);
         root=temp;
         return root;
    }
    else{
        if(root->key>k){
            root->left=insertkro(root->left,k);
            root->left->par=root;
        }
        else if(k>root->key){
            root->right=insertkro(root->right,k);
            root->right->par=root;
        }
        htchange(root);
            int bal=balance(root);
            //left -left            
            if(bal>1){
                if(root->left){
                if(k<root->left->key){
                    root=root->LeftLeftRotation(); // left left
                }
                else if(k>root->left->key){
                    root=root->LeftRightRotation(); // left right
                }
            }}
            // right-right
            if(bal<-1){
                if(root->right){
                if(k>root->right->key){
                    root=root->RightRightRotation();  //right -right 
                    }
                else if(k<root->right->key){
                    root=root->RightLeftRotation(); //right -left
                    }
                }
            }
            // right->left
            return root;
        }
}
void SymbolTable::insert(string k){
    if(search(k)==-2){
    size++;
    root=insertkro(root,k);
    root->par=NULL;
    htchange(root);
    }
}
bool checkpar(SymNode *node){
    if(!node){
        return false;
    }
    if(balance(node)>1){
        return true;
    }
    if(balance(node)<-1){
        return true;
    }
    return false;

}

void removekro(SymNode *&root,string k)
{
    if (root){
        SymNode *temp = root;
        SymNode *parent = NULL;
        while (temp->key != k){
            parent = temp;
            if (temp->key < k){
                temp = temp->right;
            }
            else{
                temp = temp->left;
            }
        }
        if (temp->left == NULL && temp->right == NULL){
            if(!parent){
                // delete root;
                return;
            }
            if (parent->key < temp->key){
                SymNode *temp=parent->right;
                parent->right = NULL;    
            }
            else{
                SymNode *temp2=parent->left;
                parent->left = NULL;
            }
            parent->height=maxi(ht(parent->left),ht(parent->right))+1;
        }
        else if (temp->left == NULL){
            if (parent != NULL){
                if (parent->key < temp->key){
                    parent->right = temp->right;
                    temp->right->par=parent->right;
                }
                else{
                    parent->left = temp->right;
                }
                parent->height=maxi(ht(parent->left),ht(parent->right))+1;
            }
            else
            {
                root=temp->right;
            }
            temp->right->par = temp->par;
        }
        else if (temp->right == NULL){
            if (parent != NULL){
                if (parent->key < temp->key){
                    parent->right = temp->left;
                }
                else{
                    parent->left = temp->left;
                }
                parent->height=maxi(ht(parent->left),ht(parent->right))+1;
            }
            else{
                root=temp->left;
            }
            temp->left->par = temp->par;
        }
        else{
            SymNode *tempo = inos(temp->right);
            string a = temp->key;
            temp->key = tempo->key;
            tempo->key = a;
            temp->address=tempo->address;
          
            if (tempo->right == NULL){
                if (tempo->key == temp->right->key){
                    // SymNode *yes=temp->right;
                    temp->right = NULL;
                    // delete yes;
                }
                else{
                    // SymNode *yes2=tempo->par->left;
                    tempo->par->left = NULL;
                    // delete yes2;
                }
            }
            else{
                if (tempo->key == temp->right->key){
                    temp->right = tempo->right;
                    tempo->right->par = temp;
                }
                else{
                    tempo->par->left = tempo->right;
                    tempo->right->par = tempo->par;
                }
            }
            parent = tempo->par;
            // end
        }
        while(parent!=NULL){   
            int change=0;
            htchange(parent);
            int bal = balance(parent);
            if (bal == 2){
            int bal2=balance(parent->left);
                if(bal2==-1){
                    parent->left=parent->left->RightRightRotation();
                    if(parent->par==NULL){
                        change=1;
                    }
                    parent=parent->LeftLeftRotation();
                }
                else{
                    if(!parent->par){
                        change=1;
                    }
                    parent=parent->LeftLeftRotation();
                }
                if (change){
                    root = parent;
                }
            }
            else if (bal==-2){   
                int bal2=balance(parent->right);
                if(bal2==1){
                    parent->right=parent->right->LeftLeftRotation();
                    if(!parent->par){
                        change=1;
                    }
                    parent=parent->RightRightRotation();
                }
                else{
                    if(!parent->par){
                        change=1;
                    }
                    parent=parent->RightRightRotation();
                }
                if (change){
                    root=parent;
                }
            }
            parent=parent->par;
        }
    }
}
void SymbolTable::remove(string k){
    if(search(k)!=-2){
    size--;
    removekro(root,k);
    }
    
    return;
}
int searchkro(SymNode *node,string k){
    if(!node){
        return -2;
    }
    if(k==node->key){
        return node->address;
    }
    else if(k<node->key){
        return searchkro(node->left,k);
    }   
    else{
        return searchkro(node->right,k);
    }
}
int SymbolTable::search(string k){
    return searchkro(root,k);
}
void addgiver(SymNode*root,string k,int ind){
    if(!root){
        return;
    }
    if(k==root->key){
        root->address=ind;
    }
    if(k>root->key){
        addgiver(root->right,k,ind);
    }
    if(k<root->key){
        addgiver(root->left,k,ind);
    }
}
void SymbolTable::assign_address(string k,int idx){
    addgiver(root,k,idx);
}

int SymbolTable::get_size(){
    return size;
}

SymNode* SymbolTable::get_root(){
    return root;
}
void deletetree(SymNode *temp){
    if(!temp){
        return;
    }
    deletetree(temp->left);
    deletetree(temp->right);
    delete temp;
}
SymbolTable::~SymbolTable(){
    deletetree(root);
}