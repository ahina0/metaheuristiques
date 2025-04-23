#pragma once

#include <chrono>
using namespace std::chrono;

#include "Graphe.hpp"


class Solution : public Graph{

    public:

        // Attributs
        int nbClasses;
        vector<unordered_set<int>> Classes;
        int borne_inf;
        int borne_sup;
    
        // Constructeur
        Solution(Graph &G, int p);

        // Méthodes communes
        int nbAretesInter();
        double ValeurObj();
        
        void print_solution();

        bool is_realisable();

        void Tirage_Vois();

};