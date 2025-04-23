#include "Tabou.hpp"

Tabou::Tabou(Graph &G, int p): Solution(G,p){}

// Calcule la variation d'objectif d'un PnD
double Tabou::delta_obj(int sommet, int classe_depart, int classe_arrivee){

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

double Tabou::Basique(int taille_tabou, int K){

    auto t1=steady_clock::now();
    double time_lim = 900000;

    double optimal = ValeurObj();
    double courant = optimal;
    deque<mvt_Inverse> tabou;

    int nb_iter = 0;
    double _min;

    int iter_max = nbSommets*nbSommets;
    int i=0;

    while (nb_iter < iter_max && duration_cast<milliseconds>(steady_clock::now() - t1).count() < time_lim){
        nb_iter++;
        _min = 1e9;
        int sommet;
        int classe_depart;
        int classe_arrivee;
        int k=0;

        // Calcul arg_min f(V\tabou)
        //for (int i=0; i<nbClasses && k<K; i++){    // Pour chaque classe

        while (int(Classes[i].size()) <= borne_inf){
            i = (i+1)%nbClasses;
        }
            //if(int(Classes[i].size()) > borne_inf){   // On teste si on peut enlever un sommet

                for (auto it = Classes[i].begin(); it!=Classes[i].end() && k<=K; it++){    // Pour chaque sommet de la classe
                    
                    for (int j=0; j<nbClasses && k<=K; j++){

                        // On vérifie que le mvt n'est pas tabou
                        bool continuer = false;
                        for (auto mvt : tabou){
                            if (*it == mvt.Sommet && j == mvt.Classe){
                                continuer = true;
                                break;
                            }
                        }
                        if (continuer){
                            continue;
                        }

                        if (j!=i){
                            if (int(Classes[j].size()) < borne_sup){      // On teste pour chaque classe si elle peut recevoir le sommet

                                
                                double delta = delta_obj(*it,i,j);   // Calcul variation fobj
                                // cout << delta << endl;
                                
                                if (delta < -1e-6){
                                    k++;
                                }
                                if (delta < _min){      // Si la solution est meilleure que min

                                    sommet = *it;
                                    classe_depart = i;
                                    classe_arrivee = j;
                                    _min = delta;

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

        courant += _min;
        Classes[classe_depart].erase(sommet);
        Classes[classe_arrivee].insert(sommet);
        if (courant < optimal){
            optimal = courant;
        }


        for (auto &mvt : tabou){
            mvt.temps++;
        }

        if (!tabou.empty()){
            if (tabou.back().temps > taille_tabou){
                tabou.pop_back();
            }
        }

        if (_min > -1e-6){
            mvt_Inverse mvt{sommet, classe_depart, _min, 1};
            tabou.push_front(mvt);
        }

        // Afficher la liste Tabou
        /*for (auto &mvt : tabou){
            cout << "(" << mvt.Sommet << "," << mvt.Classe << "), " << mvt.valeur << ", " << mvt.temps << endl;
        }
        cout << endl;*/

        i = (i+1)%nbClasses;
    }

    return optimal;
}

double Tabou::Aspiration(int taille_tabou, int K){

    auto t1=steady_clock::now();
    double time_lim = 900000;
    
    double optimal = ValeurObj();
    double courant = optimal;
    deque<mvt_Inverse> tabou;

    int nb_iter = 0;
    double _min;

    int iter_max = nbSommets*nbSommets;
    int i=0;

    while (nb_iter < iter_max && duration_cast<milliseconds>(steady_clock::now() - t1).count() < time_lim){
        nb_iter++;
        _min = 1e9;
        int sommet;
        int classe_depart;
        int classe_arrivee;
        int k=0;

        // Calcul arg_min f(V\tabou)
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

                                // On vérifie que le mvt n'est pas tabou
                                bool continuer = false;
                                for (auto mvt : tabou){
                                    if (*it == mvt.Sommet && j == mvt.Classe && (delta > mvt.valeur-1e-6 || mvt.temps < 2)){
                                        //cout << "(" << mvt.Sommet << "," << mvt.Classe << "), " << mvt.valeur << ", " << mvt.temps << endl;
                                        continuer = true;
                                        break;
                                    }
                                }
                                if (continuer){
                                    continue;
                                }
                                
                                if (delta < -1e-6){
                                    k++;
                                }
                                if (delta < _min){      // Si la solution est meilleure que min

                                    sommet = *it;
                                    classe_depart = i;
                                    classe_arrivee = j;
                                    _min = delta;

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

        courant += _min;
        Classes[classe_depart].erase(sommet);
        Classes[classe_arrivee].insert(sommet);
        if (courant < optimal){
            optimal = courant;
        }


        for (auto &mvt : tabou){
            mvt.temps++;
        }

        if (!tabou.empty()){
            if (tabou.back().temps > taille_tabou){
                tabou.pop_back();
            }
        }

        if (_min > -1e-6){
            mvt_Inverse mvt{sommet, classe_depart, _min, 1};
            tabou.push_front(mvt);
        }

        // Afficher la liste Tabou
        /*for (auto &mvt : tabou){
            cout << "(" << mvt.Sommet << "," << mvt.Classe << "), " << mvt.valeur << ", " << mvt.temps << endl;
        }
        cout << endl;*/

        i = (i+1)%nbClasses;
    }

    return optimal;
}
