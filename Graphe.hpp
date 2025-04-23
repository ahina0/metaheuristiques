#pragma once

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <random>
#include <algorithm>
#include <cmath>

using namespace std;

static random_device rd;
static mt19937 gen(rd());

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
        double w_xy(int x, int y); // Renvoie le poids de l'arête entre les sommets si elle existe, 0 sinon
};



bool find(const vector<int> &v, int a);