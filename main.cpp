#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <fstream>
#include <set>
using namespace std;

/*
 * TODO: se un nodo è troppo distante non va guardato
 *      _associare ad ogni nodo il relativo id (getInput) @R
 *      _cambiare to da int a citta @R
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
    int distance;           //distance from root=povo
    path bestPath;          //insieme dei nodi facenti parte di tutti i bestPath da Powarts a citta @R
    int counter;
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
}

void dijkstra(){
    priority_queue< pair<int, int>, vector<pair<int,int>>, greater<pair<int,int>> > q;      //priority queue con min heap (il minore è il top)
    vector<int> nds(N);
    vector<int> dist(N, 10000); //not sure 10000 is correct, maybe 10001?                   //vettore che tenga le distanze di ogni nodo dal root

    q.push(make_pair(0, P));                                                                //pusho in prqueue il nodo di partenza, con distanza 0 (va messo prima 0 perche tiene ordinata la queue)
    dist[P] = 0;                                                                            //la distanza da root a root è 0 (riporto quel che ho messo nella queue appena sopra)

    vector< set<int> > parent(N);                                                        //vettore che tenga i parent di ogni nodo
    parent[P].insert(P);                                                                 //il nodo root non ha padre, aka e' se stesso?

    while(!q.empty()){                                                                      //while queue is not empty
        int nodo = q.top().second;   q.pop();                                               //poppo l'indice del nodo (aka anche il numero del nodo effettivo) che ha distanza minore finora

        for(int i=0; i<graph[nodo].adj.size(); i++){                                        //itero tra gli adiacenti del nodo

            int nodo_adj = graph[nodo].adj[i].to;                                           //prendo il nodo adiacente iterato e il suo peso
            int nodo_adj_weight = graph[nodo].adj[i].weight;

            if(dist[nodo_adj] > dist[nodo]+nodo_adj_weight){                                //se questo è un cammino piu veloce per arrivare a nodo_adj

                dist[nodo_adj] = dist[nodo]+nodo_adj_weight;                                //segno la nuova distanza
                
                parent[nodo_adj].insert(nodo);                                              //tengo traccia del parent da cui arrivo a nodo_adj
                parent[nodo_adj].insert(parent[nodo].begin(), parent[nodo].end());          //inserisco anche i parent del nodo "vecchio"

                q.push(make_pair(dist[nodo_adj], nodo_adj));                                //metto il queue la coppia con la distanza di questo nodo
         
            } else if (dist[nodo_adj] == dist[nodo]+nodo_adj_weight) {                      //ho trovato (th.) un nodo di snodo
                nds.push_back(nodo_adj);
            }
        }    
    }
    
    //devo usare un iterator per iterare un set
    set<int>::iterator parentIt;
    cout << endl << "parents:";
    for(int i=0; i<N; i++){

        cout << endl << i << ": ";
        for(parentIt = parent[i].begin(); parentIt != parent[i].end(); parentIt++){
            cout << *parentIt << " ";
        }
    }

    //vecchio ciclo per controllare i parents
    // for(int i=0; i<N; i++){
    //     cout << P << " - " << i << " = " << dist[i] << endl;
    //     cout << " path: " << i << "-";
    //     int j=i;
    //     while(!parent[j]==P){
    //         cout << parent[j] << "-";
    //         j = parent[j];
    //     }
    //     cout << P << endl;
    // }
}

void printGraph(){
    for(int i=0; i<N; i++){
        cout << "Nodo: " << i << ", nodi vicini:" << endl;
        for(int j=0; j<graph[i].adj.size(); j++){
            cout << "    " << graph[i].adj[j].to << ", peso: " << graph[i].adj[j].weight << endl;
        }
    }
}


/*void definePath(citta node){
    //trova i nodi con counter massimo ed esclude gli altri;
    
    
}*/

/*void bestPath(citta node,citta p){                                  //BOZZA DELL'ALGORITMO CHE TROVA IL VETTORE DEGLI ELEMENTI INDISPENSABILI PER IL BESTPATH DA P A NODE @R
    if(node == p){
        definePath(node);
        return;
    }
    node.bestPath.p.push_back(node.id);
    node.counter++;
    
    
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

int main(){
    auto t1 = chrono::steady_clock::now();

    getInput();
    // printGraph();
    dijkstra();

    auto t2 = chrono::steady_clock::now();
    chrono::duration<double> s = t2-t1;
    cout << endl << "time: " << s.count() << endl;

    return 0;
}

