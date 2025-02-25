// Solution.cpp

int Solution::nbAretesInter(Graph &G){
    int compteur = 0;
    for (int i=0; i<nbClasses; i++){
        for (auto &sommet: Classes[i]){
            for (auto &[arrivee, poids]: G.Aretes[sommet]){
                if (! Classes[i].count(arrivee)){
                    compteur++;
                }
            }
        }
    }
    return compteur;
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



// Graphe.cpp

void Graph::print_graph(){      // Affiche le graphe
    
    cout << "n = " << nbSommets << ", " << nbAretes << " arêtes" << endl << "dmin = " << dmin <<", dmax = " << dmax << endl;
    for (auto &sommet: Sommets){
        cout << "Sommet " << sommet << " (degré = " << Degres[sommet] << ") :" << endl;
        for (auto &[voisin, poids] : Aretes[sommet]){
            cout << "Voisin " << voisin << " : poids " << poids << endl;
        }
        cout << endl;
    }

}