#include "search.h"
class Node2{
    public:
        int page;
        int paragraph;
        int sentence_no;
        string sentence;
};
vector<vector<Node2>>ans;
SearchEngine::SearchEngine(){
    // Implement your function here  
}
void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here
    if(book_code>maxbk){
        ans.resize(book_code+1);
        maxbk=book_code;
    }
    Node2 a;
    a.page=page;
    a.paragraph=paragraph;
    a.sentence=sentence;
    a.sentence_no=sentence_no;
    ans[book_code].push_back(a);
    return ;
}
Node* SearchEngine::search(string pattern, int& n_matches){
    // Implement your function here
    Node* shead=new Node();
    Node* stail=new Node();
    shead->right=stail;
    stail->left=shead;
    //cout << sr << " ";
    // cout << ans.size()<<endl;
    for(int i=1;i<=maxbk;i++){
            for(int j=0;j<ans[i].size();j++){
                string work="";
                string temp=ans[i][j].sentence;
                for(int k=0;k<temp.size();k++){
                    if(temp[k]>='A' and temp[k]<='Z'){
                        work.push_back(temp[k]-'A'+'a');
                    }else{
                        work.push_back(temp[k]);
                    }
                }
            //cout << work<<" ";
                string pat;
                for(int w=0;w<pattern.length();w++){
                    if(pattern[w]>='A' and pattern[w]<='Z'){
                        pat.push_back(pattern[w]-'A'+'a');
                }
                    else{
                        pat.push_back(pattern[w]);
                }
                }
            string final;
            for(char c:pat){
                final.push_back(c);
            }
            final.push_back('&');
            for(char z:work){
                final.push_back(z);
            }
            // cout << final<<"work->" <<work << "pat->"<<pat;
            vector<int> yes;
            vector<int> arr(final.length());
            int l=0;
            int r=0;
            for(int k=1;k<final.length();k++){
                if(k>r){
                    l=r=k;
                    while(r<final.length() && final[r]==final[r-l]){
                        r++;
                    }
                    arr[k]=r-l;
                    r--;
                }
                else
                {
                    int m=k-l;
                    if(arr[m]<r-k+1){
                        arr[k]=arr[m];
                    }
                    else{
                        l=k;
                        while(r<final.length() && final[r]==final[r-l]){
                            r++;
                        }
                        arr[k]=r-l;
                        r--;
                    }
                }
            }
            for(int b=0;b<final.length();b++){
                if(arr[b]==pat.length()){
                    yes.push_back(b-pat.length()-1);
                }
            }
            for(int w=0;w<yes.size();w++){
                    n_matches++;
                    Node *tempo=new Node(i,ans[i][j].page,ans[i][j].paragraph,ans[i][j].sentence_no,yes[w]);
                    Node *temp2=stail->left;
                    temp2->right=tempo;
                    tempo->left=temp2;
                    tempo->right=stail;
                    stail->left=tempo;
                }
    }
    }
    // ////cout << "hlo";
    stail->left->right=nullptr;
    Node *head=shead->right;
    // Node*temp=head;
    // while(temp){
    //     cout << head->offset << " ";
    //     temp=temp->right;
    // }cout <<endl;
    delete shead;
    delete stail;
    return head;

}
SearchEngine::~SearchEngine(){
    // Implement your function here 
}
// int main(){
//     SearchEngine e;
//     e.insert_sentence(1,0,0,0,"adittya");
//     e.insert_sentence(2,0,0,0,"adii");
//     e.insert_sentence(3,0,0,0,"ado");
//     int n=0;
//     Node* temp=e.search("a",n);
//     while(temp){
//         cout << temp->offset <<" "<<temp->book_code <<endl;
//         temp=temp->right;
//     }
//     cout << n <<endl;
// }
