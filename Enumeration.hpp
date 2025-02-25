#include "Graphe.hpp"
#include "Solution.hpp"
#include <cmath>


class Enumeration : public Solution{

    public:
    
    Enumeration(Graph &G, int p);


    void explicite();
    void implicite();

};