#pragma once

#include "Solution.hpp"

class KL : public Solution {

    public :

        KL(Graph &G);  
        
    
        // Initialise le vecteur de gains
        vector<double> gains_x();

        // Retourne le gain_xy max et modifie en référence les valeurs de x et y
        double gain_max(const vector<double> &gains, const unordered_set<int> &C1, const unordered_set<int> &C2, int &x, int &y);

        void maj_gains(vector<double> &gains, int x, int y, const vector<int> &L_C1, const vector<int> &L_C2);

        void maj_Classes(const vector<int> &L_C1, const vector<int> &L_C2, int k);

        double Kernighan_Lin(int iter_max);
        
};