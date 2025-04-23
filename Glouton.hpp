#pragma once

#include "Solution.hpp"

class Glouton : public Solution {
    
    public:
        Glouton(Graph &G, int p);
        
        // Calcule la variation d'objectif d'un PnD
        double delta_obj(int sommet, int classe_depart, int classe_arrivee);    

        double gradient(int iter);

        double heuristique(int iter_max, int K);
        
};
