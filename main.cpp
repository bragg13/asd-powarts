#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
using namespace std;

/*
 * TODO: se un nodo è troppo distante non va guardato
*/

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

    vector<int> dist(N, 10000); //not sure 10000 is correct, maybe 10001?                   //vettore che tenga le distanze di ogni nodo dal root

    q.push(make_pair(0, P));                                                                //pusho in prqueue il nodo di partenza, con distanza 0 (va messo prima 0 perche tiene ordinata la queue)
    dist[P] = 0;                                                                            //la distanza da root a root è 0 (riporto quel che ho messo nella queue appena sopra)

    vector<int> parent(N, 10000);                                                           //vettore che tenga i parent di ogni nodo
    parent[P] = P;                                                                          //il nodo root non ha padre, aka e' se stesso?

    while(!q.empty()){                                                                      //while queue is not empty
        int nodo = q.top().second;   q.pop();                                               //poppo l'indice del nodo (aka anche il numero del nodo effettivo) che ha distanza minore finora

        for(int i=0; i<graph[nodo].adj.size(); i++){                                        //itero tra gli adiacenti del nodo

            int nodo_adj = graph[nodo].adj[i].to;                                           //prendo il nodo adiacente iterato e il suo peso
            int nodo_adj_weight = graph[nodo].adj[i].weight;

            if(dist[nodo_adj] > dist[nodo]+nodo_adj_weight){                                //se questo è un cammino piu veloce per arrivare a nodo_adj

                dist[nodo_adj] = dist[nodo]+nodo_adj_weight;                                //segno la nuova distanza
                
                parent[nodo_adj] = nodo;                                                    //tengo traccia del parent da cui arrivo a nodo_adj

                q.push(make_pair(dist[nodo_adj], nodo_adj));                                //metto il queue la coppia con la distanza di questo nodo
            }
        }    
    }
    

    for(int i=0; i<N; i++){
        cout << P << " - " << i << " = " << dist[i] << endl;
        cout << " path: " << i << "-";
        int j=i;
        while(!parent[j]==P){
            cout << parent[j] << "-";
            j = parent[j];
        }
        cout << P << endl;
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
    // printGraph();
   
    cout << endl << endl;

    dijkstra();

    return 0;
}

