#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <fstream>
#include <set>
using namespace std;

/*
 * TODO: se un nodo è troppo distante non va guardato
 * 9 12 13NONVA
*/

struct edge {
    int to;
    int weight;
};

struct citta {
    vector<edge> adj;
};

int N;                      //no. of cities (aka students+1) 
int M;                      //no. of paths
int P;                      //city of Powarts
vector<citta> graph;

vector<int> visiteParenti = vector<int>(N);

void getInput(){
    ifstream in("input.txt");     
    // ifstream in("input0.txt");     
    // ifstream in("input1.txt");
    // ifstream in("input2.txt");
    // ifstream in("input3.txt");
    // ifstream in("input4.txt");

    // ifstream in("input5.txt");     
    // ifstream in("input6.txt");
    // ifstream in("input7.txt");
    // ifstream in("input8.txt");
    // ifstream in("input9.txt"); //1818 DOVREBBE 26056

    // ifstream in("input10.txt");     
    // ifstream in("input11.txt");
    // ifstream in("input12.txt"); //156, DOVREBBE 4400
    // ifstream in("input13.txt"); //4 DOVREBBE 16819
    // ifstream in("input14.txt"); //18 DOVREBBE 39347

    // ifstream in("input15.txt"); //15199 DOVREBBE 11321
    // ifstream in("input16.txt"); //28979 DOVREBBE 17517
    // ifstream in("input17.txt"); //12086 DOVREBBE 7508
    // ifstream in("input18.txt"); //31534 DOVREBBE 20220
    // ifstream in("input19.txt"); //40204 25317

    in >> N >> M >> P;                                                                      //input for N M P
    graph.resize(N);


    int a, b, w;
    edge e;
    for(int i=0; i<M; i++){ 
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


void dijkstra(){
    priority_queue< pair<int, int>, vector<pair<int,int>>, greater<pair<int,int>> > q;      //priority queue con min heap (il minore è il top)
    vector<int> nds(N, -1);                                                                  //vettore che tiene i nodi di snodo e quante volte vengono attraversati
    vector<int> dist(N, 10000); //not sure 10000 is correct, maybe 10001?                   //vettore che tenga le distanze di ogni nodo dal root

    q.push(make_pair(0, P));                                                                //pusho in prqueue il nodo di partenza, con distanza 0 (va messo prima 0 perche tiene ordinata la queue)
    dist[P] = 0;                                                                            //la distanza da root a root è 0 (riporto quel che ho messo nella queue appena sopra)

    vector< set<int> > parent(N);                                                           //vettore che tenga i parent di ogni nodo
    // parent[P].insert(P);                                                                    //il nodo root non ha padre, aka e' se stesso?


    while(!q.empty()){                                                                      //while queue is not empty
        int nodo = q.top().second;   q.pop();                                               //poppo l'indice del nodo (aka anche il numero del nodo effettivo) che ha distanza minore finora
        // cout << "nodo in esame: " << nodo << endl;
        for(int i=0; i<graph[nodo].adj.size(); i++){                                        //itero tra gli adiacenti del nodo

            if(graph[nodo].adj[i].to != P){
                int nodo_adj = graph[nodo].adj[i].to;                                           //prendo il nodo adiacente iterato e il suo peso
                int nodo_adj_weight = graph[nodo].adj[i].weight;

                // cout << "  adj in easme: " << nodo_adj << ", dist=" << dist[nodo_adj];

                if(dist[nodo_adj] > dist[nodo]+nodo_adj_weight){                                //se questo è un cammino piu veloce per arrivare a nodo_adj
                    dist[nodo_adj] = dist[nodo]+nodo_adj_weight;                                //segno la nuova distanza
                    // cout << " ===> " << dist[nodo_adj];
                    
                    parent[nodo_adj].insert(nodo);                                              //tengo traccia del parent da cui arrivo a nodo_adj
                    parent[nodo_adj].insert(parent[nodo].begin(), parent[nodo].end());          //inserisco anche i parent del nodo "vecchio"

                    q.push(make_pair(dist[nodo_adj], nodo_adj));                                //metto in queue la coppia con la distanza di questo nodo
            
                }
            }
        }    
    }


    //devo usare un iterator per iterare un set
    vector<int> arr = vector<int>(N, 0); //da cambiare in array, troppa memorai usata

    set<int>::iterator parentIt;
    // cout << endl << "parents:";

    for(int i=0; i<N; i++){
        // cout << endl << i << ": ";
        for(parentIt = parent[i].begin(); parentIt != parent[i].end(); parentIt++){
            // cout << *parentIt << " ";
            if (*parentIt != P) arr[*parentIt]++;
        }
    }

    int max = 0;
    for(int i=0; i<N; i++){
        // cout << i << ":" << arr[i] << endl;
        if (max <arr[i])
            max = arr[i];
    }

    // cout << max+1;
    ofstream out("output.txt");
    out << max+1;
    out.close();
}


int main(){
    // auto t1 = chrono::steady_clock::now();

    getInput();
    dijkstra();

    // auto t2 = chrono::steady_clock::now();
    // chrono::duration<double> s = t2-t1;
    // cout << endl << "time: " << s.count() << endl;

    return 0;
}

