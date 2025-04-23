#include "Graphe.hpp"
#include "Solution.hpp"


class Enumeration : public Solution {

    public:
    
        Enumeration(Graph &G, int p);

        double explicite();
        // void implicite();

};