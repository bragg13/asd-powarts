#include <iostream>
#include <vector>
#include <fstream>
using namespace std;


struct edge {
    int to;
    int weight;
};

struct citta {
    vector<edge> adj;
    int distance;           //distance from root=povo
};

int N;                      //no. of cities (aka students+1) 
int M;                      //no. of paths
int P;                      //city of Powarts
vector<citta> graph;

void getInput(){
    //Input for N M P
    ifstream in("input.txt");
    in >> N >> M >> P;
    graph.resize(N);

    //Input for edges (bidirectional)
    //Metto i nodi nell'indice corretto
    for(int i=0; i<M; i++){
        int a, b, w;
        edge e;
        in >> a >> b >> w;
        e.weight = w;
        
        // a--[w]--b
        e.to = b;
        graph[a].adj.push_back(e);

        // b--[w]--a
        e.to = a;
        graph[b].adj.push_back(e);
    }
}

void printGraph(){
    for(int i=0; i<N; i++){
        cout << "Nodo: " << i << ", nodi vicini:" << endl;
        for(int j=0; j<graph[i].adj.size(); j++){
            cout << "    " << graph[i].adj[j].to << ", peso: " << graph[i].adj[j].weight << endl;
        }
    }
}

int main(){
    getInput();
    printGraph();
    return 0;
}

