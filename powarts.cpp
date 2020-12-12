#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <map>
using namespace std;

// ===== Dichiarazione Funzioni =====
void getInput();
void dijkstra();
void findAttackedCities();
// ===== Dichiarazione Funzioni =====


struct edge {
    int to;
    int weight;
};

struct citta {            
    vector<edge> adj;               //edges adiacenti
    bool is_infinity = true;        //gestione di INF in dijkstra
    int distance;                   //distanza da povo
    int predecessore;               //il nodo precedente
    int count_apparizioni = 1;      //il grafo è connesso, il nodo appare almeno 1 volta in un percorso minimo
};


int N;                              //numero di citta
int M;                              //numero di archi
int P;                              //powarts
vector<citta> graph;                //il grafo
int _max = 0;                       //numero di citta attaccate
int _maxI = 0;                      //indice della citta attaccata

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


/*
* Tiene traccia delle occorrenze dei nodi e quali nodi attraversano altri attraverso una mappa
*/
void printPath(int nodoOrig, int nodo, map<int, vector<int>> &occorrenze){
    if(graph[nodo].predecessore == -1){                         //se il predecessore è -1 è powarts, chiudo
        return;
    }
    
    occorrenze[nodo].push_back(nodoOrig);                       //nodo è attraversato dal best path di nodoOriginale

    if (occorrenze[nodo].size() > _max){                        //tengo traccia del massimo
        _max = occorrenze[nodo].size();
        _maxI = nodo;
    }

    printPath(nodoOrig, graph[nodo].predecessore, occorrenze);  //ricorsivamente procedo con il predecessore
    // cout << nodo << " ";

}

void dijkstra(){
    priority_queue< pair<int, int>, vector<pair<int,int>>, greater<pair<int,int>> > q;          				    //priority queue con min heap (il minore è il top)
    // priority_queue< pair<int, int> > vitalNodes;

    q.push(make_pair(0, P));																					    //pusho il nodo root con distanza 0 (da se stesso)
    graph[P].distance = 0; 
    graph[P].is_infinity = false;																				    //boolean per faking di INFINITY
    graph[P].predecessore = -1;
    int nodo;

    while(!q.empty()){
        nodo = q.top().second;																					    //prendo il nodo con distanza minima da root 
        q.pop();

        for(auto i : graph[nodo].adj){
            int nodo_adj = i.to;						            					    					    //nodo adiacente
            int nodo_adj_weight = i.weight;              			                							    //peso dell'adiacente
            if(graph[nodo_adj].is_infinity || graph[nodo_adj].distance > graph[nodo].distance+nodo_adj_weight){	    //se il nodo non è stato scoperto ancora or trovo un percorso minore
                graph[nodo_adj].distance = graph[nodo].distance+nodo_adj_weight;								    //nuova distanza
                graph[nodo_adj].is_infinity = false;					
                graph[nodo_adj].predecessore = nodo;                                                                //salvo il predecessore del nodo in esame
                q.push(make_pair(graph[nodo_adj].distance, nodo_adj));											    //metto il queue la coppia con la distanza di questo nodo
            }
            
        }
    }
}


void findAttackedCities(){     
    map<int, vector<int> > occorrenze;              //<nodo, <nodi che ci passano> >

    for(int i=0; i<N ; i++){
        // cout << i << "= ";
        printPath(i, i, occorrenze);
        // cout << "____" << i << ": " << occorrenze[i] << endl;
        // cout << endl;
    }
    /* ========= DEBUG =========
        cout << "attacked: " << _max << "@" << _maxI << endl;
        cout << "citta: " << endl;
        for(int i=0; i<occorrenze[_maxI].size(); i++){
            cout << occorrenze[_maxI][i] << endl;
        }
       ========= DEBUG ========= */

    ofstream out("output.txt");
    out << _max << endl;
    for(int i : occorrenze[_maxI]){
        out << i << endl;
    }
    out.close();
}

int main(){
    getInput();
    dijkstra();
    findAttackedCities();
    return 0;
}
