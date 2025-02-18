#pragma once

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

struct node{
    int degre;
    unordered_map<int, double> aretes;
};

struct graph{
    int dmin;
    int dmax;
    vector <node> sommets;
};


graph read_graph(string filename);      // Renvoie un graphe à partir d'un fichier

void print_graph(graph G);      // Affiche le graphe