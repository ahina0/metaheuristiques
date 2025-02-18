#include "graphe.hpp"

graph read_graph(string filename){      // Renvoie un graphe à partir d'un fichier
    
    graph G;

    ifstream file(filename);

    if(!file){
        cerr << "Could not open " << filename << endl;
        abort();
    }

    // Taille du graphe
    int nbSommets, nbAretes;
    string lineComm;
    getline(file, lineComm);    // On passe la ligne commentée
    file >> nbSommets >> nbAretes;
    G.sommets.resize(nbSommets);

    // Degrés
    int dmin, dmax;
    getline(file, lineComm);    // On passe la ligne commentée
    file >> dmin >> dmax;
    G.dmin = dmin;
    G.dmax = dmax;

    // Arêtes
    int source, but;
    double weight;
    getline(file, lineComm);    // On passe la ligne commentée
    string line;
    for (int i=0; i<nbAretes; i++){
        file >> source >> but >> weight;
        G.sommets[source].aretes[but] = weight;
    }

    // Degré par sommet
    int sommet, degre;
    getline(file, lineComm);    // On passe la ligne commentée
    for (int i=0; i<nbSommets; i++){ 
        file >> sommet >> degre;
        G.sommets[i].degre = degre;
    }

    return G;
}

void print_graph(graph G){      // Affiche le graphe
    
    int n = G.sommets.size();
    cout << "n = " << n << endl;
    for (int i=0; i<n; i++){
        cout << "Sommet " << i << " (d = " << G.sommets[i].degre << "):" << endl;
        for (auto &[voisin, poids] : G.sommets[i].aretes){
            cout << "Voisin " << voisin << " : poids " << poids << endl;
        }
        cout << endl;
    }

}