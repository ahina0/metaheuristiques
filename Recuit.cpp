#include "Recuit.hpp"


double Recuit::Calcul_T0(double tau){
    double delta = 0;
    for (int i=0; i<100; i++){
        Solution S(*this);
        S.Tirage_Vois();
        delta += abs(S.ValeurObj() - ValeurObj())/100;
    }
    return -delta / log(tau);
}


Recuit::Recuit(Graph &G, int k, double tau) : Solution(G, k), T(Calcul_T0(tau)) {}


void Recuit::Metropolis(Solution &X_best, Recuit &X, double T, int t, int &nb1, int &nb2) {

    double delta_f = X.ValeurObj() - ValeurObj();

    if (delta_f < -1e-6) {
        *this = X;
        if (ValeurObj() - X_best.ValeurObj() < -1e-6) {
            X_best = *this;
            iter_best = t;
            T_best = T;
        }
    }
    else {
        nb1++;
        uniform_real_distribution<> x(0.0, 1.0);
        double p = x(gen);
        if (p - exp(-1 * delta_f / T) < -1e-6) {
            *this = X;
            nb2++;
        }
    }
}

void Recuit::g(){
    double mu = 0.9;
    T = mu*T;
}

double Recuit::Recuit_Simule(){

    auto t1=steady_clock::now();
    double time_lim = 900000;
    
    // Solution / T° initiales dans le constructeur

    Solution X_best (*this);
    double opt = X_best.ValeurObj();
    int t = 0;
    int nbSansAmelioration = 0;
    while (T > 5e-2 && nbSansAmelioration < 3 && duration_cast<milliseconds>(steady_clock::now() - t1).count() < time_lim){     // T° min + adaptatif : 3 sans amélioration de optimal
        int iter = 0;
        int cond2 = 50*nbSommets;
        int nbPire = 0;
        int condAccept = 5*nbSommets;
        int nbAccept = 0;
        while (nbPire < cond2 && nbAccept < condAccept && duration_cast<milliseconds>(steady_clock::now() - t1).count() < time_lim){

            iter++;
            // Choisir X dans le voisinage
            Recuit X(*this);
            X.Tirage_Vois();

            Metropolis(X_best, X, T, t, nbPire, nbAccept);
            
            t++;
        }
        g();    // suite geom de raison mu = 0.95
        
        nbSansAmelioration++;
        double nb_opt = X_best.ValeurObj();
        if (nb_opt < opt-1e-3){
            opt = nb_opt;
            //cout << "reset" << endl;
            nbSansAmelioration = 0;
        }
        //cout << T << " : " << nb_opt << endl;
        
    }

    return X_best.ValeurObj();
}
