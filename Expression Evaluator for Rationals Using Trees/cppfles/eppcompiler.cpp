/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include"eppcompiler.h"
//Write your code below this line

EPPCompiler::EPPCompiler(){

}

EPPCompiler::EPPCompiler(string out_file,int mem_limit){
    memory_size=mem_limit;
    output_file=out_file;
    for(int i=0;i<mem_limit;i++){
        least_mem_loc.push_heap(i);
    }
}

void EPPCompiler::compile(vector<vector<string>> code){
    fstream out ;
    out.open(output_file , ios :: out) ;
    out.close() ;
    vector<string> ans;
    for(int i=0;i<code.size();i++){
        targ.parse(code[i]);
        
        if(code[i][0]=="del"){
            least_mem_loc.push_heap(targ.last_deleted);
        }
        else if(targ.expr_trees[i]->left->type=="VAR"){
            if(targ.symtable->search(targ.expr_trees[i]->left->id)==-1){
            targ.symtable->assign_address(targ.expr_trees[i]->left->id,least_mem_loc.get_min());
            least_mem_loc.pop();
            }
        }
        vector<string> yes=generate_targ_commands();
        for(int i=0;i<yes.size();i++){
            ans.push_back(yes[i]);
        }
    }
    write_to_file(ans);
}
void postorder(vector<string> &s,ExprTreeNode *root,Parser targ){
        if(!root){
            return;
        }
        if(root->right==NULL and root->left==NULL){
            if(root->type=="VAL"){
                string c="PUSH "+root->id;
                s.push_back(c);
            }
            if(root->type=="VAR"){
                string d="PUSH mem["+to_string(targ.symtable->search(root->id))+"]";
                s.push_back(d);
            }
        }
        postorder(s,root->right,targ);
        postorder(s,root->left,targ);
        if(root->type=="ADD" ||root->type=="SUB"||root->type=="MUL"||root->type=="DIV"){
        s.push_back(root->type);
        }
}
vector<string> EPPCompiler::generate_targ_commands(){
        vector<string> s;
        ExprTreeNode*root=targ.expr_trees[targ.expr_trees.size()-1];
        postorder(s,root->right,targ);
        string b="";
        if(root->left->type=="DEL"){
            if(targ.last_deleted==-3){
                // cout << "hlo";
                // cout << root->right->id <<endl;
                targ.last_deleted=targ.symtable->search(root->right->id);
            }
            b="DEL = mem["+to_string(targ.last_deleted)+"]";
        }
        else if(root->left->id=="ret"){
            b = "RET = POP";
        }
        else if(root->left->type=="VAR"){
            // b="mem["+ to_string(targ.symtable->search(root->left->id))+"] = POP";
            b = "mem[" ;
            b =  b+ to_string(targ.symtable->search(root->left->id)) ;
            b += "] = POP";
        }
        s.push_back(b);
        return s;
}

void EPPCompiler::write_to_file(vector<string> commands){
 ofstream obj(output_file,ios::app);
//  obj.open() ;
 for(int i=0;i<commands.size();i++){
    // cout << commands[i] << endl;
    obj << commands[i] <<"\n";
 }
 obj.close();

}

EPPCompiler::~EPPCompiler(){
  
}
