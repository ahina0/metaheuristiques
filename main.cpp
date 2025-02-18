#include "graphe.hpp"

int main(){

    string filename = "test.txt";

    graph G = read_graph(filename);

    print_graph(G);

    return 0;
}