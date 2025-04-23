#pragma once

#include "Solution.hpp"

class Recuit : public Solution {

    public :

        // Attributs
        double T;
        int iter_best;
        double T_best;

        double Calcul_T0(double tau);
        Recuit(Graph &G, int k, double tau);
    
        void Metropolis(Solution &X_best, Recuit &X, double T, int t, int &nb1, int &nb2);

        void g();

        double Recuit_Simule();
};