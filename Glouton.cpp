#include "Glouton.hpp"

Glouton::Glouton(Graph &G, int p): Solution(G,p){}


// Calcule la variation d'objectif d'un PnD
double Glouton::delta_obj(int sommet, int classe_depart, int classe_arrivee){

    double delta = 0;

    for (auto &s : Classes[classe_depart]){     // On ajoute le poids de chaque arête avec les sommets de la classe de départ
        if (Aretes[sommet].count(s)){
            delta += Aretes[sommet][s];
        } else if (Aretes[s].count(sommet)){
            delta += Aretes[s][sommet];
        }
    }

    for (auto &s : Classes[classe_arrivee]){     // On enlève le poids de chaque arête avec les sommets de la classe d'arrivée
        if (Aretes[sommet].count(s)){
            delta -= Aretes[sommet][s];
        } else if (Aretes[s].count(sommet)){
            delta -= Aretes[s][sommet];
        }
    }

    return delta;
}

double Glouton::gradient(int iter_max){
    auto t1=steady_clock::now();
    double time_lim = 900000;
    // cout << borne_inf << " " << borne_sup << endl;
    double valeurObj = ValeurObj();
    double min;

    bool is_optimal = false;
    int iter = 0;

    while(!is_optimal and iter<iter_max and duration_cast<milliseconds>(steady_clock::now() - t1).count() < time_lim){   // On stoppe si pas d'amélioration de l'objectif ou si nombre max d'itérations

        
        iter++;
        min = 0;    // On ne cherche que des solutions diminuant la fonction objectif
        int sommet;
        int classe_depart;
        int classe_arrivee;
    
        for (int i=0; i<nbClasses; i++){    // Pour chaque classe

            if(int(Classes[i].size()) > borne_inf){   // On teste si on peut enlever un sommet

                for (auto &s : Classes[i]){    // Pour chaque sommet de la classe
                    
                    for (int j=0; j<nbClasses; j++){
                        if (j!=i){
                            if (int(Classes[j].size()) < borne_sup){      // On teste pour chaque classe si elle peut recevoir le sommet

                                double delta = delta_obj(s,i,j);   // Calcul variation fobj
                                // cout << delta << endl;

                                if (delta < min){      // Si la solution est meilleure que min

                                    sommet = s;
                                    classe_depart = i;
                                    classe_arrivee = j;
                                    min = delta;

                                }
                            }
                        }
                    }
                }
            }
        }

        if (min < -0.00001){    // Si amélioration de l'objectif, réalise le mouvement élémentaire optimal
            valeurObj += min;
            Classes[classe_depart].erase(sommet);
            Classes[classe_arrivee].insert(sommet);
        } else {    // Sinon, arrêt de l'algorithme car minimum local
            is_optimal = true;
        }
    }
    return valeurObj;
}


double Glouton::heuristique(int iter_max, int K) {
    auto t1=steady_clock::now();
    double time_lim = 900000;
    double valeurObj = ValeurObj();
    double min;

    bool is_optimal = false;
    int iter = 0;
    int i=0;

    while(!is_optimal and iter<iter_max and duration_cast<milliseconds>(steady_clock::now() - t1).count() < time_lim){   // On stoppe si pas d'amélioration de l'objectif ou si nombre max d'itérations

        
        iter++;
        min = 0;    // On ne cherche que des solutions diminuant la fonction objectif
        int sommet;
        int classe_depart;
        int classe_arrivee;
        int k=0;
    
        //for (int i=0; i<nbClasses && k<K; i++){    // Pour chaque classe

        while (int(Classes[i].size()) <= borne_inf){
            i = (i+1)%nbClasses;
        }

            //if(int(Classes[i].size()) > borne_inf){   // On teste si on peut enlever un sommet

                for (auto it = Classes[i].begin(); it!=Classes[i].end() && k<=K; it++){    // Pour chaque sommet de la classe
                    
                    for (int j=0; j<nbClasses && k<=K; j++){
                        if (j!=i){
                            if (int(Classes[j].size()) < borne_sup){      // On teste pour chaque classe si elle peut recevoir le sommet

                                double delta = delta_obj(*it,i,j);   // Calcul variation fobj
                                // cout << delta << endl;
                                
                                if (delta < -1e-6){
                                    k++;
                                    if (delta < min){      // Si la solution est meilleure que min

                                        sommet = *it;
                                        classe_depart = i;
                                        classe_arrivee = j;
                                        min = delta;

                                    }
                                }
                                if (k>K){
                                    break;
                                }
                            }
                        }
                    }
                }
            //}
        //}

        if (min < -0.00001){    // Si amélioration de l'objectif, réalise le mouvement élémentaire optimal
            valeurObj += min;
            Classes[classe_depart].erase(sommet);
            Classes[classe_arrivee].insert(sommet);
        } else {    // Sinon, arrêt de l'algorithme car minimum local
            is_optimal = true;
        }

        i = (i+1)%nbClasses;
    }
    return valeurObj;
}
