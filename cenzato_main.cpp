#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
using namespace std;

// START Spazio dichiarazione funzioni
void findAttackedCities();


// END Spazio dichiarazione funzioni


struct edge {
    int to;
    int weight;
};

struct citta {
    int id;                    //id della citta (importandoquelli in input) @R
    vector<edge> adj;
    int distance = 10001;           //distance from root=povo
    std::vector<int> bestPath;          //insieme dei nodi facenti parte di tutti i bestPath da Powarts a citta @R
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

    q.push(make_pair(0, P));                                                                //pusho in prqueue il nodo di partenza, con distanza 0 (va messo prima 0 perche tiene ordinata la queue)
    graph[P].distance = 0;                                                                            //la distanza da root a root è 0 (riporto quel che ho messo nella queue appena sopra)

    graph[P].bestPath.push_back(P);                                                                 //il nodo root non ha padre, aka e' se stesso?
    int nodo;

    while(!q.empty()){                                                                      //while queue is not empty
        nodo = q.top().second;   q.pop();                                               //poppo l'indice del nodo (aka anche il numero del nodo effettivo) che ha distanza minore finora

        for(int i=0; i<graph[nodo].adj.size(); i++){                                        //itero tra gli adiacenti del nodo

            int nodo_adj = graph[nodo].adj[i].to;                                           //prendo il nodo adiacente iterato e il suo peso
            int nodo_adj_weight = graph[nodo].adj[i].weight;

            if(graph[nodo_adj].distance > graph[nodo].distance+nodo_adj_weight){                                //se questo è un cammino piu veloce per arrivare a nodo_adj

                graph[nodo_adj].distance = graph[nodo].distance+nodo_adj_weight;                                //segno la nuova distanza
                
                graph[nodo_adj].bestPath.push_back(nodo);                                              //tengo traccia del parent da cui arrivo a nodo_adj
                graph[nodo_adj].bestPath.insert(graph[nodo_adj].bestPath.begin(),graph[nodo].bestPath.begin(), graph[nodo].bestPath.end());          //inserisco anche i parent del nodo "vecchio"

                q.push(make_pair(graph[nodo_adj].distance, nodo_adj));                                //metto il queue la coppia con la distanza di questo nodo
         
            }
        }    
    }
}


void findAttackedCities(){   
    int tmp;      
    for(int i = 0; i < N; i++){
        for(int j = 0; j < graph[i].bestPath.size(); j++){
            tmp = graph[i].bestPath[j];
            if(tmp != P)
            graph[tmp].count_apparizioni++;
        }
    }

    int max = 1;
    for(int i = 0; i < N; i++){
        if(graph[i].count_apparizioni > max)
            max = graph[i].count_apparizioni;
    }

    ofstream out("output.txt");
    out << max;
    out.flush(); out.close();
}

int main(){

    getInput();
    dijkstra();
    findAttackedCities();
    return 0;
}
