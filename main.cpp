#include <iostream>
#include <vector>
#include <fstream>
using namespace std;


struct edge {
    int from, to;
    int weight;
}

struct citta {
    vector<edge> adj;
    int distance;           //distance from root=povo
}

int N;                      //no. of cities (aka students+1) 
int M;                      //no. of paths
int P;                      //city of Powarts
vector<citta> graph;


int main(){
    return 0;
}

