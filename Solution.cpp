#include "Solution.hpp"

Solution::Solution(Graph &G, int p) : Graph(G) {
    nbClasses = p;
    Classes.resize(nbClasses);
    // A peu près équité, choix : 1%
    borne_inf = floor((nbSommets/p) * (99/100));
    borne_sup = ceil((nbSommets/p) * (101/100));

    // On convertit les sommets en vecteur pour leur donner un ordre aléatoire
    vector<int> vec(Sommets.begin(), Sommets.end()); 
    for (int i=0; i<nbSommets; i++){
        int j = rand() % (nbSommets - i) + i;  // Nombre aléatoire entre 0 et nbSommets
        int temp = vec[i];
        vec[i] = vec[j];
        vec[j] = temp;
    }

    // On remplit les classes un sommet à la fois (un dans la 0, puis un dans la 1, ..., puis un dans la p-1, puis un dans la 0, ...)
    int compteur=0;
    for (int i=0; i<nbSommets; i++) {
        Classes[compteur % nbClasses].insert(vec[i]);
        compteur++;
    }
}


int Solution::nbAretesInter(){
    int compteur = 0;
    for (int i=0; i<nbClasses; i++){
        for (auto &sommet: Classes[i]){
            for (auto &[arrivee, poids]: Aretes[sommet]){
                if (! Classes[i].count(arrivee)){
                    compteur++;
                }
            }
        }
    }
    return compteur;
}


double Solution::ValeurObj() {
    double obj=0;
    // On parcourt chaque voisin de chaque sommet de chaque classe
    for (int i=0 ; i<nbClasses ; i++) {
        for (auto &sommet: Classes[i]) {
            for (auto &[arrivee, poids]: Aretes[sommet]) {
                // Si le sommet d'arrivée n'est pas dans la même classe que le sommet de départ
                if (! Classes[i].count(arrivee)) {
                    // Le poids de l'arête est pris en compte
                    obj += poids;
                }
            }
        }
    }
    return obj;
}


void Solution::print_solution(){
    cout << nbSommets << " sommets" << endl << nbClasses << " classes" << endl;
    for (int i=0; i<nbClasses; i++){
        cout << "Classe " << i+1 << " :" << endl;
        for (auto &sommet: Classes[i]){
            cout << "  Sommet " << sommet << endl;
        }
        cout << endl;
    }
}



// Renvoie true si la solution est réalisable
// On suppose que dans la solution, tous les sommets sont répartis dans nbClasses classes
// Le test ne porte que sur le nombre de sommets par classe
bool Solution::is_realisable() {
    for (int i=0 ; i<nbClasses ; i++) {
        if (Classes[i].size() < borne_inf or Classes[i].size() > borne_sup) {
            return false;
        }
    }
    return true;
}