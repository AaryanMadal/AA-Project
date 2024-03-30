#include <iostream>
#include<bits/stdc++.h>

using namespace std;
int type;

bool is_dominated(const vector<vector<pair<int,int>>>& graph,const pair<set<int>, 
                    set<pair<int, int>>>& new_tree, const set<pair<set<int>, set<pair<int, int>>>>& trees) {
    int sum1=0;
    int sum2=0;
        for (const auto& edge : new_tree.second) {
            sum1+=graph[edge.first][edge.second].first;
            sum2+=graph[edge.first][edge.second].second;
        }
        
    for (const auto& tree : trees) {
        bool dominated = true;
        
        int alt1=0;
        int alt2=0;
        for (const auto& edge : tree.second) {
            alt1+=graph[edge.first][edge.second].first;
            alt2+=graph[edge.first][edge.second].second;
        }
        
        if(type==1){
            if (sum1>alt1) {
                dominated=false;
                return false;
            }
        }
        else if(type==2){
            if(sum2<alt2){
                dominated=false;
                return false;
            }
        }
        else {
            if((sum1>alt1&&sum2<alt2)||(sum1>alt1&&sum2==alt2)||(sum1==alt1&&sum2<alt2)){
                return false;
            }
        }
    }
    return true;
}
bool is_edge(pair<int,int>ed,const vector<vector<pair<int,int>>>& graph){
    for(int i=0;i<graph.size();i++){
        for(int j=0;j<graph[0].size();j++){
            if(graph[i][j].first!=0&&i==ed.first&&j==ed.second){
                return true;
            }
        }
    }
    return false;
}

set<pair<set<int>, set<pair<int, int>>>> pareto_optimal_mostp(const vector<vector<pair<int,int>>>& graph) {
    int n = graph.size();
    map<int, set<int>> X;
    map<int, set<pair<int, int>>> E;
    map<int, set<pair<set<int>, set<pair<int, int>>>>> L;

    // Step 0: Initialization
    X[1].insert(0);
    set<int> X_1 = {0};
    set<pair<int, int>> E_1;
    pair<set<int>, set<pair<int, int>>> S_1 = {X_1, E_1};
    L[1].insert(S_1);

    // Step 1: State-space exploration
    for (int q = 1; q < n; ++q) {
        for (const auto& S_h : L[q]) {
            set<int> X_h = S_h.first;
            set<pair<int, int>> E_h = S_h.second;
            for (int i : X_h) {
                for (int j = 0; j < n; ++j) {
                    if (X_h.find(j) == X_h.end()&&is_edge({i,j},graph)) {
                        set<int> X_q_plus_1 = X_h;
                        X_q_plus_1.insert(j);
                        set<pair<int, int>> E_q_plus_1 = E_h;
                        E_q_plus_1.insert({i, j});
                        pair<set<int>, set<pair<int, int>>> S_q_plus_1 = {X_q_plus_1, E_q_plus_1};
                        if (is_dominated(graph,S_q_plus_1, L[q + 1])) {
                            L[q + 1].insert(S_q_plus_1);
                            bool add_tree = true;
                            for (auto it = L[q + 1].begin(); it != L[q + 1].end(); ) {
                                if (!is_dominated(graph,*it, L[q + 1])) {
                                    it = L[q + 1].erase(it);
                                }
                                else{
                                    it++;
                                }
                            }
                           
                        }
                    }
                }
            }
        }
    }

    // Step 2: Exit step
    return L[n];
}

int main() {
    vector<vector<pair<int,int>>> graph = {
        {{0,0},{100,10},{150,8},{120,9}},
        {{100,10},{0,0},{200,7},{180,14}},
        {{150,8},{200,7},{0,0},{100,9}},
        {{120,9},{180,8},{100,9},{0,0}}
    };
    
    cout<<"Enter which constraint you want to use: 1=> Minimise Cost 2=> Maximise Accessibility 3=> Both combined: "<<endl;
    cin>>type;
    
    set<pair<set<int>, set<pair<int, int>>>> pareto_optimal_trees = pareto_optimal_mostp(graph);
    cout << "Pareto Optimal Trees:" << endl;
    for (const auto& tree : pareto_optimal_trees) {
        cout << "Nodes: { ";
        for (int node : tree.first) {
            cout << node << " ";
        }
        cout << "}" << endl;
        cout << "Edges: { ";
        for (const auto& edge : tree.second) {
            cout << "(" << edge.first << ", " << edge.second << ") ";
        }
        cout << "}" << endl;
        cout << endl;
    }
    return 0;
}