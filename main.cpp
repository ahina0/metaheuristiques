#include "Graphe.hpp"

int main(int argc, char ** argv){

    string filename = "test.txt";

    if (argc == 2){
        filename = argv[1];
    }

    Graph G(filename);

    G.print_graph();

    return 0;
}