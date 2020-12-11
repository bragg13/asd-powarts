#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <unordered_set>
using namespace std;

// START Spazio dichiarazione funzioni
void getInput();
void dijkstra();
void findAttackedCities();


// END Spazio dichiarazione funzioni


struct edge {
    int to;
    int weight;
};

struct citta {            
    vector<edge> adj;
    bool is_infinity = true;
    int distance;
    int predecessore;       
    int count_apparizioni = 1;   //il grafo è connesso, il nodo appare almeno 1 volta in un percorso minimo
};



int N;                      
int M;                   
int P;                      
vector<citta> graph;

void getInput(){
    ifstream in("input.txt");
    in >> N >> M >> P;
    graph.resize(N);


   for(int i=0; i<M; i++){ 
       int a, b, w;
       in >> a >> b >> w;

       edge e;

       e.weight = w;
       
       // a--[w]--b
       e.to = b;
       graph[a].adj.push_back(e);

       // b--[w]--a
       e.to = a;
       graph[b].adj.push_back(e);
   }
    in.close();
}

void dijkstra(){
    priority_queue< pair<int, int>, vector<pair<int,int>>, greater<pair<int,int>> > q;

    q.push(make_pair(0, P));
    graph[P].distance = 0; 
    graph[P].is_infinity = false;
    graph[P].predecessore = P;
    int nodo;

    while(!q.empty()){
        nodo = q.top().second;
        q.pop();
        for(int i=0; i<graph[nodo].adj.size(); i++){

            int nodo_adj = graph[nodo].adj[i].to;
            int nodo_adj_weight = graph[nodo].adj[i].weight;              
            if(graph[nodo_adj].is_infinity || graph[nodo_adj].distance > graph[nodo].distance+nodo_adj_weight){
                
                graph[nodo_adj].distance = graph[nodo].distance+nodo_adj_weight;
                graph[nodo_adj].is_infinity = false;
                graph[nodo_adj].predecessore = nodo;
                q.push(make_pair(graph[nodo_adj].distance, nodo_adj));
            }
        }    
    }
}


void findAttackedCities(){   
    unordered_set<int>::iterator it;      
    for(int i = 0; i < N; i++){
        for(int j = graph[i].predecessore; j != P;j = graph[j].predecessore){
            graph[j].count_apparizioni++;
        }
    }
    int max = 1;
    int nodo_attaccato = 0;
    for(int i = 0; i < N; i++){
        if(graph[i].count_apparizioni > max){
            max = graph[i].count_apparizioni;
            nodo_attaccato = i;
        }
    }
    ofstream out("output.txt");
    out << max << endl;
    std::vector<int> ritardatari;
    ritardatari.push_back(nodo_attaccato);
    for(int i = 0; i < N; i++){               //Soluzione naive
        for(int j = graph[i].predecessore; j != P; j = graph[j].predecessore){
            if(j == nodo_attaccato){
                ritardatari.push_back(i);
                break;
            }
        }
    }

    for(int i = 0; i < ritardatari.size(); i++){
        out << ritardatari[i] << endl;
    }
    out.flush(); out.close();
}

int main(){

    getInput();
    dijkstra();
    findAttackedCities();
    return 0;
}
