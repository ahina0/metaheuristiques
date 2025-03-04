#include "Solution.hpp"

Solution::Solution(Graph &G, int p) : Graph(G) {
    nbClasses = p;
    Classes.resize(nbClasses);
    borne_inf = nbSommets / p - 1;
    borne_sup = nbSommets / p + 1;

    // On convertit les sommets en vecteur pour leur donner un ordre aléatoire
    vector<int> vec(Sommets.begin(), Sommets.end()); 
    for (int i=0; i<nbSommets; i++){
        int j = rand() % (nbSommets - i) + i;  // Nombre aléatoire entre 0 et nbSommets
        int temp = vec[i];
        vec[i] = vec[j];
        vec[j] = temp;
    }

    // On remplit les classes un sommet a la fois (un dans la 0, puis un dans la 1, ..., puis un dans la p-1, puis un dans la 0, ...)
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
    // on parcourt chaque voisin de chaque sommet de chaque classe
    for (int i=0 ; i<nbClasses ; i++) {
        for (auto &sommet: Classes[i]) {
            for (auto &[arrivee, poids]: Aretes[sommet]) {
                // si le sommet d'arrivee n'est pas dans la meme classe que le sommet de depart
                if (! Classes[i].count(arrivee)) {
                    // le poids de l'arete est pris en compte
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



// Renvoie true si la solution est realisable
// On suppose que dans la solution, tous les sommets sont repartis dans nbClasses
// Le test ne porte que sur le nombre de sommets par classe
// "a peu pres egal" est ici egal ou +1 (dans le cas ou le nb de sommets n'est pas un multiple du nb de classes voulu)
bool Solution::is_realisable() {
    for (int i=0 ; i<nbClasses ; i++) {
        if (Classes[i].size() < floor(nbSommets/nbClasses) or Classes[i].size() > floor(nbSommets/nbClasses)+1) {
            return false;
        }
    }
    return true;
}