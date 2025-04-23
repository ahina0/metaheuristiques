#include "Solution.hpp"

Solution::Solution(Graph &G, int p) : Graph(G) {
    nbClasses = p;
    Classes.resize(nbClasses);
    // A peu près équité, choix : 1%
    borne_inf = floor(double(nbSommets)/p*0.99);
    borne_sup = ceil(double(nbSommets)/p*1.01);

    // On convertit les sommets en vecteur pour leur donner un ordre aléatoire
    vector<int> vec(Sommets.begin(), Sommets.end()); 
    std::shuffle(vec.begin(), vec.end(), gen);

    // On remplit les classes un sommet à la fois (un dans la 0, puis un dans la 1, ..., puis un dans la p-1, puis un dans la 0, ...)
    int compteur=0;
    for (int i=0; i<nbSommets; i++) {
        Classes[compteur % nbClasses].insert(vec[i]);
        compteur++;
    }

    // Nombre aléatoire de PnD entre 0 et 1% de nbSommets pour modifier la structure de la solution
    uniform_int_distribution<int> x(0,nbSommets/100 + 1);
    int alea = x(gen);
    for (int i=0; i<alea; i++){
        Tirage_Vois();
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


double Solution::ValeurObj(){
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
        if (int(Classes[i].size()) < borne_inf or int(Classes[i].size()) > borne_sup) {
            return false;
        }
    }
    return true;
}


void Solution::Tirage_Vois(){

    // On prend une classe de départ au hasard
    uniform_int_distribution<int> x2(0,nbClasses-1);
    int cDep = x2(gen);
    while (int(Classes[cDep].size()) == borne_inf){
        uniform_int_distribution<int> x2(0,nbClasses-1);
        cDep = x2(gen);
    }

    // On prend un sommet au hasard dans la classe
    uniform_int_distribution<int> x3(0,Classes[cDep].size()-1);
    int iter = x3(gen);
    auto it = Classes[cDep].begin();
    advance(it, iter);
    int sommet = *it;

    // On prend une classe d'arrivée au hasard
    uniform_int_distribution<int> x4(0,nbClasses-2);
    int cArr = x4(gen);
    if (cArr >= cDep){cArr++;}
    while (int(Classes[cArr].size()) == borne_sup){
        uniform_int_distribution<int> x4(0,nbClasses-2);
        cArr = x4(gen);
        if (cArr >= cDep){cArr++;}
    }

    // On fait le PnD
    Classes[cDep].erase(sommet);
    Classes[cArr].insert(sommet);
}