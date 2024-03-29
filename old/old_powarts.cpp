#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <fstream>
#include <set>
using namespace std;

// START Spazio dichiarazione funzioni
void findAttackedCities(vector< set<int> >& parent, set<int>::iterator& parentIt);


// END Spazio dichiarazione funzioni


/*
 * TODO: se un nodo è troppo distante non va guardato
 *      _associare ad ogni nodo il relativo id (getInput) @R
 *      _cambiare to da int a citta @R
 * NOT WORKING: 
 *  12+
 * 
*/

struct path {
    vector<int> p;        //si potrebbe memorizzare anche come vector<citta> @R
};

struct edge {
    int to;
    int weight;
};

struct citta {
    int id;                    //id della citta (importandoquelli in input) @R
    vector<edge> adj;
    int distance = 10001;           //distance from root=povo
    path bestPath;          //insieme dei nodi facenti parte di tutti i bestPath da Powarts a citta @R
    int count_apparizioni = 1;
};



int N;                      //no. of cities (aka students+1) 
int M;                      //no. of paths
int P;                      //city of Powarts
vector<citta> graph;

void getInput(){
    ifstream in("input.txt");
    in >> N >> M >> P;                                                                      //input for N M P
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
    priority_queue< pair<int, int>, vector<pair<int,int>>, greater<pair<int,int>> > q;      //priority queue con min heap (il minore è il top)
    vector<int> nds(N);

    q.push(make_pair(0, P));                                                                //pusho in prqueue il nodo di partenza, con distanza 0 (va messo prima 0 perche tiene ordinata la queue)
    graph[P].distance = 0;                                                                            //la distanza da root a root è 0 (riporto quel che ho messo nella queue appena sopra)

    vector< set<int> > parent(N);                                                        //vettore che tenga i parent di ogni nodo
    parent[P].insert(P);                                                                 //il nodo root non ha padre, aka e' se stesso?

    while(!q.empty()){                                                                      //while queue is not empty
        int nodo = q.top().second;   q.pop();                                               //poppo l'indice del nodo (aka anche il numero del nodo effettivo) che ha distanza minore finora

        for(int i=0; i<graph[nodo].adj.size(); i++){                                        //itero tra gli adiacenti del nodo

            int nodo_adj = graph[nodo].adj[i].to;                                           //prendo il nodo adiacente iterato e il suo peso
            int nodo_adj_weight = graph[nodo].adj[i].weight;

            if(graph[nodo_adj].distance > graph[nodo].distance+nodo_adj_weight){                                //se questo è un cammino piu veloce per arrivare a nodo_adj

                graph[nodo_adj].distance = graph[nodo].distance+nodo_adj_weight;                                //segno la nuova distanza
                
                parent[nodo_adj].insert(nodo);                                              //tengo traccia del parent da cui arrivo a nodo_adj
                parent[nodo_adj].insert(parent[nodo].begin(), parent[nodo].end());          //inserisco anche i parent del nodo "vecchio"

                q.push(make_pair(graph[nodo_adj].distance, nodo_adj));                                //metto il queue la coppia con la distanza di questo nodo
         
            } else if (graph[nodo_adj].distance == graph[nodo].distance+nodo_adj_weight) {                      //ho trovato (th.) un nodo di snodo
                nds.push_back(nodo_adj);
            }
        }    
    }
    
    //devo usare un iterator per iterare un set
    set<int>::iterator parentIt;
    // cout << endl << "parents:";
    for(int i=0; i<N; i++){

        // cout << endl << i << ": ";
        for(parentIt = parent[i].begin(); parentIt != parent[i].end(); parentIt++){
            // cout << *parentIt << " ";
        }
    }

    findAttackedCities(parent, parentIt);  //gli argomenti sono la lista di cammini ottimali per ogni nodo e l'iteratore di questa lista (forse + veloce)
}

/*void bestPath(citta node,citta p){                                  //BOZZA DELL'ALGORITMO CHE TROVA IL VETTORE DEGLI ELEMENTI INDISPENSABILI PER IL BESTPATH DA P A NODE @R
    node.bestPath.p.push_back(node.id);
    
    vector<edge> e;
    for(edge &edge : node.adj){
        if(edge.weight == (node.distance - edge.to.distance)) {
            e.push_back(edge);   
        }
    }
    
    for(edge &edge : e){
        bestPath(e.to,p);
    }
}*/

void findAttackedCities(vector< set<int> >& parent, set<int>::iterator& parentIt){         
    for(int i = 0; i < N; i++){
        for(parentIt = parent[i].begin(); parentIt != parent[i].end(); parentIt++){
            if(*parentIt != P)
                graph[*parentIt].count_apparizioni++;
        }
    }

    int max = 1;
    for(int i = 0; i < N; i++){
        if(graph[i].count_apparizioni > max)
            max = graph[i].count_apparizioni;
    }

    cout << endl << "Gli studenti arrivati in ritardo sono " << max << endl;
    ofstream out("output.txt");
    out << max;
    out.flush(); out.close();
}

int main(){
    auto t1 = chrono::steady_clock::now();

    getInput();
    dijkstra();

    auto t2 = chrono::steady_clock::now();
    chrono::duration<double> s = t2-t1;
    cout << endl << "time: " << s.count() << endl;

    return 0;
}
