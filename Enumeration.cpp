#include "Enumeration.hpp"


Enumeration::Enumeration(Graph &G, int p): Solution(G,p){}


double Enumeration::explicite(){

    double valeurObj, min = ValeurObj();

    for (int x=0; x<pow(nbClasses, nbSommets); x++){        // Pour tout x dans ER :
        int i = x;
        vector< unordered_set<int> > classes(nbClasses);
        int iter=1;
        for (auto &sommet: Sommets){
            classes[i % nbClasses].insert(sommet);
            i /= int(nbClasses);
            iter++;
        }

        Classes = classes;
        
        if (is_realisable()) {
            valeurObj = ValeurObj();
            if (min > valeurObj) {
                min = valeurObj;
            }
        }
    }
    return min;
}
