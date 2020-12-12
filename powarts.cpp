#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <map>
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
    int count_apparizioni = 1;      //il grafo è connesso, il nodo appare almeno 1 volta in un percorso minimo
};


int N;                              //numero di citta
int M;                              //numero di archi
int P;                              //powarts
vector<citta> graph;
vector<int> parenti = vector<int>(N);
int _max = 1;

void getInput(){
    ifstream in("input9.txt");
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


// void pushaParenti(int nodo){
//     if(graph[nodo].predecessore == P){
//         return;
//     }
//     parenti.push_back(nodo);
//     pushaParenti(graph[nodo].predecessore);
//     cout << nodo << "-";
// }


void printPath(int nodo, map<int, int> &occorrenze){
    if(graph[nodo].predecessore == -1){
        return;
    }

    occorrenze[nodo]++;
    if (occorrenze[nodo] > _max)
        _max = occorrenze[nodo];
    printPath(graph[nodo].predecessore, occorrenze);
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
                // pushaParenti(nodo_adj);
                q.push(make_pair(graph[nodo_adj].distance, nodo_adj));											    //metto il queue la coppia con la distanza di questo nodo
            }
            // else if(!graph[nodo_adj].is_infinity && graph[nodo_adj].distance == graph[nodo].distance+nodo_adj_weight){
            //     //possibile nds
            //     if(graph[nodo_adj].predecessore == graph[nodo]){
            //         cout << "nodo di scambio" << endl;
            //         cout <<
            //     }
            // }
        }
    }
    // delete &q;
}


void findAttackedCities(){     
    map<int, int> occorrenze = map<int,int>();
    for(int i=0; i<N ; i++){
        // cout << i << "= ";
        printPath(i, occorrenze);
        // cout << endl;
    }
    cout << _max << endl;

    // int max = 1;
    // for(int i=0; i<occorrenze.size(); i++){
    //     // if(max < occorrenze[i])
    //     //     max = occorrenze[i];
    //     cout << i << ": " << occorrenze[i] << endl;
    // }

    // cout << "MAX= " << max << endl;
    //per ogni nodo del grafo itero sul suo cammino minimo per raggiungere Powarts
  	//ad ogni nodo che incontro aumento il numero di frequenza, che ci servirà a trovare il nodo su cui passano + cammini, cioè quello che verrà attaccato
    // for(int i = 0; i < N; i++){
    //     for(int j = graph[i].predecessore; j != P;j = graph[j].predecessore){
    //         graph[j].count_apparizioni++;
    //     }
    // }
    // int max = 1;
    // int nodo_attaccato = 0;         				//identifica il nodo che appartiene a più cammini e il numero di nodi che non rende più
    // for(int i = 0; i < N; i++){						// raggiungibili se attaccato
    //     if(graph[i].count_apparizioni > max){
    //         max = graph[i].count_apparizioni;
    //         nodo_attaccato = i;
    //     }
    // }
    
    // ofstream out("output.txt");
    // out << max << endl;								//stampa il numero dei nodi non più raggiungibili in tempo
    // std::vector<int> ritardatari;					//vettore per tenere traccia delle citta interessate dall'attacco
    
    // ritardatari.push_back(nodo_attaccato);      
    // for(int i = 0; i < N; i++){               		//Soluzione naive:Itera un'altra volta tra tutti i nodi, salva i nodi nel cui cammino minimo fa parte il nodo attaccato
    //     for(int j = graph[i].predecessore; j != P; j = graph[j].predecessore){
    //         if(j == nodo_attaccato){
    //             ritardatari.push_back(i);
    //             break;								//se trovo il nodo attaccato non ho bisogno di risalire il cammino ulteriormente
    //         }
    //     }
    // }

    // for(int i = 0; i < ritardatari.size(); i++){
    //     out << ritardatari[i] << endl;				//stampa i nodi non più raggiungibili in tempo
    // }
    
    // out.flush(); out.close();
}

int main(){
    getInput();
    dijkstra();
    findAttackedCities();
    return 0;
}
