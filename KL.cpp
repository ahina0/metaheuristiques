#include "KL.hpp"

KL::KL(Graph &G) : Solution(G,2){}


vector<double> KL::gains_x() {
    vector<double> gains(nbSommets+1, 0);
    for (auto &[depart, arete] : Aretes) {
        for (auto &[arrivee, poids] : arete) {
            // Si les deux sommets sont dans la même classe, interne
            // Il n'y a que deux classes
            if(Classes[0].contains(depart) == Classes[0].contains(arrivee)) {
                gains[depart] -= poids;
                gains[arrivee] -= poids;
            }
            // Si les deux sommets ne sont pas dans la même classe, externe
            else {
                gains[depart] += poids;
                gains[arrivee] += poids;
            }
        }
    }
    return gains;
}

double KL::gain_max(const vector<double> &gains, const unordered_set<int> &C1, const unordered_set<int> &C2, int &x, int &y){
    double max = -1e6;
    for (int a : C1){
        for (int b : C2){
            if(gains[a] + gains[b] - 2*w_xy(a,b) > max){
                max = gains[a] + gains[b] - 2*w_xy(a,b);
                x = a;
                y = b;
            }
        }
    }
    return max;
}

void KL::maj_gains(vector<double> &gains, int x, int y, const vector<int> &L_C1, const vector<int> &L_C2){
    for (auto &a : Classes[0]){
        if (!find(L_C1, a)){
            if (Classes[0].contains(x)){
                gains[a] += 2*w_xy(a,x);
            } else {
                gains[a] -= 2*w_xy(a,x);
            }
            if (Classes[0].contains(y)){
                gains[a] += 2*w_xy(a,y);
            } else {
                gains[a] -= 2*w_xy(a,y);
            }
        } 
    }
    for (auto &a : Classes[1]){
        if (!find(L_C2, a)){
            if (Classes[1].contains(x)){
                gains[a] += 2*w_xy(a,x);
            } else {
                gains[a] -= 2*w_xy(a,x);
            }
            if (Classes[1].contains(y)){
                gains[a] += 2*w_xy(a,y);
            } else {
                gains[a] -= 2*w_xy(a,y);
            }
        } 
    }
}

void KL::maj_Classes(const vector<int> &L_C1, const vector<int> &L_C2, int k){
    for (int i=0; i<k+1; i++){
        Classes[0].insert(L_C2[i]);
        Classes[0].erase(L_C1[i]);
        Classes[1].insert(L_C1[i]);
        Classes[1].erase(L_C2[i]);
    }
}

double KL::Kernighan_Lin(int iter_max){

    auto t1=steady_clock::now();
    double time_lim = 900000;

    double max = 1;
    int iter = 0;

    // On prend n = min(|C1|, |C2|)
    int n;
    if (Classes[0].size() > Classes[1].size()){
        n = Classes[1].size();
    } else {
        n = Classes[0].size();
    }
    
    while (max > 1e-6 and iter < iter_max and duration_cast<milliseconds>(steady_clock::now() - t1).count() < time_lim){

        // Récupérer les gains
        vector<double> gains = gains_x();
        unordered_set<int> set_C1, set_C2;
        for (int sommet : Classes[0]){
            set_C1.insert(sommet);
        }
        for (int sommet : Classes[1]){
            set_C2.insert(sommet);
        }

        vector<int> L_C1(n,-1), L_C2(n,-1);
        vector<double> L_g(n,0.);

        for (int i=0; i<n; i++){
            int x=-1;
            int y=-1;
            // On cherche x dans C1 et y dans C2 tels que gain_xy est maximal
            double gain_xy = gain_max(gains, set_C1, set_C2, x, y);
            L_C1[i] = x;
            L_C2[i] = y;
            set_C1.erase(x);
            set_C2.erase(y);
            L_g[i] = gain_xy;
            maj_gains(gains, x, y, L_C1, L_C2);
        }

        // On cherche k tel que cumsum(L_g) est maximal
        double sommes_partielles = L_g[0];
        double k = 0;
        max = sommes_partielles;
        for (int i=1; i<n; i++){
            sommes_partielles += L_g[i];
            if (sommes_partielles > max){
                max = sommes_partielles;
                k = i;
            }
        }
        if (max > 1e-6){
            maj_Classes(L_C1, L_C2, k);
        }
        iter++;
    }
    

    return ValeurObj();
}