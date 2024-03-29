#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
using namespace std;

// START Spazio dichiarazione funzioni
void getInput();
void dijkstra();
void findAttackedCity();
void print_ritardatari(int nodo_attaccato, ofstream & out);
void print_ritardatari_nds(int nodo_attaccato, ofstream & out);
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
    int count_apparizioni = 1;      //il grafo è connesso, il nodo appare almeno 1 volta in un percorso minimo
    int grand_parent;               //numero del nodo + vicino a Powarts collegato da un percorso ottimale(inizializzato al numero della città stessa)
};


int N;                              //numero di citta
int M;                              //numero di archi
int P;                              //powarts
vector<citta> graph;
vector<int> nds;
int ndstot = 0;

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
       graph[a].grand_parent = a; 
       // b--[w]--a
       e.to = a;
       graph[b].adj.push_back(e);
       graph[b].grand_parent = b;
   }
    in.close();
}

void dijkstra(){
    priority_queue< pair<int, int>, vector<pair<int,int>>, greater<pair<int,int>> > q;          				//priority queue con min heap (il minore è il top)

    q.push(make_pair(0, P));																					//pusho il nodo root con distanza 0 (da se stesso)
    graph[P].distance = 0; 
    graph[P].is_infinity = false;																				//boolean per faking di INFINITY
    graph[P].predecessore = P;
    int nodo;

    nodo = q.top().second;																					    //ciclo poco elegante per togliere Powarts dalla queue
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
        nds.push_back(nodo_adj);
    }    

    while(!q.empty()){
        nodo = q.top().second;																					//prendo il nodo con distanza minima da root 
        q.pop();
        for(int i=0; i<graph[nodo].adj.size(); i++){															//visito gli adiacenti
            int nodo_adj = graph[nodo].adj[i].to;																//nodo adiacente
            int nodo_adj_weight = graph[nodo].adj[i].weight;              										//peso dell'adiacente
            if(graph[nodo_adj].is_infinity || graph[nodo_adj].distance > graph[nodo].distance+nodo_adj_weight){	//se il nodo non è stato scoperto ancora or trovo un percorso minore
                for(int i=0; i<nds.size(); i++){
                    if(nds[i]==nodo_adj){
                        // cout << "!nds: " << nodo_adj << endl;
                        nds.erase(nds.begin()+i);
                        ndstot--;
                    }
                }
                graph[nodo_adj].distance = graph[nodo].distance+nodo_adj_weight;								//nuova distanza
                graph[nodo_adj].is_infinity = false;					
                graph[nodo_adj].predecessore = nodo;															//salvo il predecessore del nodo in esame

                graph[graph[nodo_adj].grand_parent].count_apparizioni--;                                        //è stato trovato un percorso + veloce, diminuisco count al vecchio grand_parent
                graph[nodo_adj].grand_parent = graph[nodo].grand_parent;                                        //cambio grand_parent al nodo
                graph[graph[nodo].grand_parent].count_apparizioni++;                                            //aggingo 1 al count del nuovo grand_parent
                
                q.push(make_pair(graph[nodo_adj].distance, nodo_adj));											//metto il queue la coppia con la distanza di questo nodo


            }
            else if (!graph[nodo_adj].is_infinity && graph[nodo_adj].distance == graph[nodo].distance+nodo_adj_weight){
                if(graph[nodo_adj].predecessore != nodo){
                    graph[nodo_adj].grand_parent = nodo_adj;
                    // cout << "nds: " << nodo_adj << endl;
                    nds.push_back(nodo_adj);
                    ndstot++;
                }
            }
        }    
    }

}


void findAttackedCity(){
    int max = 0;
    int nodo_attaccato = 0;
    int tmp;
    
    for(int i = 0; i < nds.size();i++){
        if(graph[nds[i]].count_apparizioni > max){
            max = graph[nds[i]].count_apparizioni;
            nodo_attaccato = nds[i];
        }
    }

    ofstream out("output.txt");
    
    //se ho dei nodi di scambio
    if(nds.size() > graph[P].adj.size()){
        print_ritardatari_nds(nodo_attaccato, out);
    }else{
        out << max << endl;								//stampa il numero dei nodi non più raggiungibili in tempo
        print_ritardatari(nodo_attaccato, out);
    }

    out.flush(); out.close();
}

void print_ritardatari(int attacked, ofstream &out){
    queue<int> q;
    q.push(attacked);

    int i;
    while(!q.empty()){
        i = q.front();
        q.pop();
        out << i << endl;
        for(edge e : graph[i].adj){
            if(graph[e.to].predecessore == i)
                q.push(e.to);
        }
    }
}

void print_ritardatari_nds(int attacked, ofstream &out){
    queue<int> q;
    q.push(attacked);
    int totCitta = 0;
    string cities = "";

    int i;
    while(!q.empty()){  
        i = q.front();
        q.pop();
        cities += to_string(i);
        cities += "\n";
        totCitta++;
        for(edge e : graph[i].adj){
            if(graph[e.to].predecessore == i)
                q.push(e.to);
        }
    }
    out << totCitta << endl;
    out << cities;
}


int main(){
    getInput();
    dijkstra();
    findAttackedCity();
    return 0;
}
