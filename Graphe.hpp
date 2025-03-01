#pragma once

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
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
        // Un graphe : <sommet de depart : (sommet d'arrivee : (poids de l'arete))>
        unordered_map <int, unordered_map<int, double> > Aretes;
        vector<int> Degres;
        unordered_set<int> Sommets;

    // Méthodes
        Graph(string filename);     // Construit un graphe à partir d'un fichier
        void print_graph();     // Affiche le graphe
};

