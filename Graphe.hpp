#pragma once

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>

using namespace std;


class Graph{

    public:

    // Attributs
        int nbSommets;
        int nbAretes;
        int dmin;
        int dmax;
        unordered_map <int, unordered_map<int, double> > aretes;
        vector<int> degres;

    // Méthodes
        Graph(string filename);     // Construit un graphe à partir d'un fichier
        void print_graph();     // Affiche le graphe
};

