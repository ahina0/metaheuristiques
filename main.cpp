#include "Graphe.hpp"
#include "Solution.hpp"

int main(int argc, char ** argv){

    string filename = "test.txt";

    if (argc == 2){
        filename = argv[1];
    }

    Graph G(filename);

    G.print_graph();

    Solution S(G,5);

    S.print_solution();

    cout << S.nbAretesInter() << endl;

    return 0;
}