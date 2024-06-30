
#include "parser.h"

//Write your expression below this line

Parser::Parser(){
    last_deleted=-3;
    symtable=new SymbolTable;
}
void Parser::parse(vector<string> expression){
    if(expression.size()==3){
        ExprTreeNode *templ=new ExprTreeNode();
        templ->id=expression[0];
        ExprTreeNode *tempR=new ExprTreeNode();
         tempR->id=expression[2];
        if(templ->id=="ret"){
            templ->type="RET";
            if(symtable->search(expression[2])!=-2){
                tempR->type="VAR";
            }else{
                tempR->type="VAL";
            }
        }
        else if(templ->id=="del"){
            templ->type="DEL";
            // cout << "\n" << " came" <<endl;
            // cout << expression[2] <<endl;
            last_deleted=symtable->search(expression[2]);
            // cout << last_deleted << endl;
            symtable->remove(templ->id);
        }
        else{
        templ->type="VAR";
        if(symtable->search(tempR->id)==-2){
            tempR->type="VAL";
            tempR->num=stoi(tempR->id);
        }
        else{
            tempR->type="VAR";
        }
        if(symtable->search(templ->id)==-2){
        symtable->insert(templ->id);
        }
    }
        ExprTreeNode *tempM=new ExprTreeNode;
        tempM->id=expression[1];
        tempM->type="equal";
        tempM->right=tempR;
        tempM->left=templ;
        expr_trees.push_back(tempM);
    }
    else{
    int i=0;
    while(i<expression.size()){
    if(i==0){
        ExprTreeNode *tempo=new ExprTreeNode();
        string a=expression[0];
        tempo->id=a;
        if(a=="ret"){
            tempo->type="RET";
        }
        else{
            tempo->type="VAR";
        if(symtable->search(tempo->id)==-2){
        symtable->insert(tempo->id);
        }
        }
        expr_trees.push_back(tempo);
        i++;
    }
    while(expression[i]!=")"){
            if(i==expression.size()){
            break;
            }
            if(expression[i]=="("){
            i++;
            }
            else{
            ExprTreeNode *temp=new ExprTreeNode;
            temp->id=expression[i];
            if(expression[i]=="+"){
                temp->type="ADD";
            }
            if(expression[i]=="-"){
                temp->type="SUB";
            }
            if(expression[i]=="*"){
                temp->type="MUL";
            }
            if(expression[i]=="/"){
                temp->type="DIV";
            }
            if(expression[i]==":="){
                temp->type="equal";
            }
            if(symtable->search(expression[i])!=-2){
                temp->type="VAR";
            }
            if(temp->type==""){
            temp->type="VAL";

            temp->num=stoi(temp->id);
            }
            expr_trees.push_back(temp);
            i++;
            // cout << "YEH";
        }
    }
    if(expression[i]==")"){
    ExprTreeNode *right=expr_trees[expr_trees.size()-1];
    expr_trees.pop_back();
    ExprTreeNode *mid=expr_trees[expr_trees.size()-1];
    expr_trees.pop_back();
    ExprTreeNode *left=expr_trees[expr_trees.size()-1];
    expr_trees.pop_back();
    mid->left=left;
    mid->right=right;
    expr_trees.push_back(mid); 
    i++;
    }
    }
    ExprTreeNode *rightt=expr_trees[expr_trees.size()-1];
    expr_trees.pop_back();
    ExprTreeNode *root=expr_trees[expr_trees.size()-1];
    expr_trees.pop_back();
    ExprTreeNode *vari=expr_trees[expr_trees.size()-1];
    expr_trees.pop_back();
    root->right=rightt;
    root->left=vari;
    expr_trees.push_back(root);
    }
}
Parser::~Parser(){
}

