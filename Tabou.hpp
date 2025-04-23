#pragma once

#include "Solution.hpp"
#include <deque>


struct mvt_Inverse{
    int Sommet;
    int Classe;
    double valeur;
    int temps;
};

class Tabou: public Solution{

    public:

        Tabou(Graph &G, int k);

        double delta_obj(int sommet, int classe_depart, int classe_arrivee); 

        double Basique(int taille_tabou, int K);

        double Aspiration(int taille_tabou, int K);

};
