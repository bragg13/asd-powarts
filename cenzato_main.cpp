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
    int id;                
    vector<edge> adj;
    int distance = 10001;          
    std::unordered_set<int> bestPath;       
    int count_apparizioni = 1;
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

    graph[P].bestPath.insert(P);
    int nodo;

    while(!q.empty()){
        nodo = q.top().second;
        q.pop();
        for(int i=0; i<graph[nodo].adj.size(); i++){

            int nodo_adj = graph[nodo].adj[i].to;
            int nodo_adj_weight = graph[nodo].adj[i].weight;

            if(graph[nodo_adj].distance > graph[nodo].distance+nodo_adj_weight){

                graph[nodo_adj].distance = graph[nodo].distance+nodo_adj_weight;
                
                graph[nodo_adj].bestPath.insert(nodo);
                graph[nodo_adj].bestPath.insert(graph[nodo].bestPath.begin(), graph[nodo].bestPath.end());
                q.push(make_pair(graph[nodo_adj].distance, nodo_adj));
            }
        }    
    }
}


void findAttackedCities(){   
    unordered_set<int>::iterator it;      
    for(int i = 0; i < N; i++){
        for(it = graph[i].bestPath.begin(); it != graph[i].bestPath.end(); it++){
            if(*it != P)
            graph[*it].count_apparizioni++;
        }
    }
    int max = 1;
    //int nodo_attaccato = 0;
    for(int i = 0; i < N; i++){
        if(graph[i].count_apparizioni > max){
            max = graph[i].count_apparizioni;
            //nodo_attaccato = i;
        }
    }
    ofstream out("output.txt");
    out << max << endl;
    /*std::vector<int> ritardatari;
    ritardatari.push_back(nodo_attaccato);
    for(int i = 0; i < N; i++){               //Soluzione naive
        if(graph[i].bestPath.find(nodo_attaccato) != graph[i].bestPath.end())
            ritardatari.push_back(i); 
    }

    for(int i = 0; i < ritardatari.size(); i++){
        out << ritardatari[i] << endl;
    }*/
    out.flush(); out.close();
}

int main(){

    getInput();
    dijkstra();
    findAttackedCities();
    return 0;
}
