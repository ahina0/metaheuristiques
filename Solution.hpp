#pragma once

#include "Graphe.hpp"


class Solution{

    public:

        // Attributs
        int p;
        int n;
        unordered_map <int, int> sol;   // Vecteur ou unordered map?
    
        // Constructeur
        Solution(Graph G, int nbClassess);

        // Méthodes communes
        calcul nbAretesInter
        calcul valeurObj
        
        //affich solution

        // Méthodes virtuelles
        //calcul vois
        //algo

};

