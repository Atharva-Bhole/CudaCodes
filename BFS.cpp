#include<iostream>
#include<queue>
#include<vector>
#include<omp.h>

using namespace std;

class Node{
    public:
    int data;
    Node *left, *right;

    Node(int val){
        data = val;
        left = right = nullptr;
    }
};

Node *insert(Node *root, int data){
    if(!root) return new Node(data);

    queue<Node *> q;
    q.push(root);

    while(!q.empty()){
        Node *temp = q.front();
        q.pop();

        if(!temp->left){
            temp->left = new Node(data);
            return root;
        }
        else{
            q.push(temp->left);
        }

        if(!temp->right){
            temp->right = new Node(data);
            return root;
        }
        else{
            q.push(temp->right);
        }
        
    }

    return root;
}

void bfs_parallel(Node *root){
    if(!root) return;

    vector<Node *> currentLevel{root};

    while(!currentLevel.empty()){
        vector<Node *> nextLevel;
        #pragma omp parallel
        {
            vector<Node *> localNext;
            #pragma omp nowait
            for(int i = 0; i < currentLevel.size(); i++){
                Node *node = currentLevel[i];

                #pragma omp critical
                cout<<node->data<<" ";

                if(node->left) localNext.push_back(node->left);
                if(node->right) localNext.push_back(node->right);
            }

            #pragma omp critical
            nextLevel.insert(nextLevel.end(), localNext.begin(), localNext.end());
        }

        currentLevel = nextLevel;
    }
}


int main(){
    Node *root = nullptr;
    int data;
    int n;
    cout<<"Enter total number of nodes";
    cin>>n;
    cout<<"\nEnter Data";
    for(int i = 0; i < n; i++){
        cin>>data;
        root = insert(data, root);
    }

    cout<<"\nParallel BFS Traversal"<<endl;
    bfs_parallel(root);

    return 0;
}