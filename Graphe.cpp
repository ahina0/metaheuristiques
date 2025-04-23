#include "Graphe.hpp"

Graph::Graph(string filename){      // Construit un graphe à partir d'un fichier

    ifstream file(filename);

    if(!file){
        cerr << "Could not open " << filename << endl;
        abort();
    }

    // Taille du graphe

    string indic;
    string line;

    file >> indic;
    while (indic[0] == '#'){
        getline(file, line);
        file >> indic;
    }
    nbSommets = stoi(indic);

    file >> indic;
    while (indic[0] == '#'){
        getline(file, line);
        file >> indic;
    }
    nbAretes = stoi(indic);

    file >> indic;
    while (indic[0] == '#'){
        getline(file, line);
        file >> indic;
    }
    dmin = stoi(indic);

    file >> indic;
    while (indic[0] == '#'){
        getline(file, line);
        file >> indic;
    }
    dmax = stoi(indic);

    // Arêtes + degrés + sommets

    Degres.resize(nbSommets + 1, 0);

    double nombre;
    int i=0;

    while (i<nbAretes){

        vector<double> vec(0);

        // On passe les lignes commentées
        file >> indic;
        if (indic[0] == '#'){   
            getline(file, line);
        } else {
            i++;
            vec.push_back(stoi(indic));
            getline(file,line);
            istringstream iss(line);

            while (iss >> nombre){
                vec.push_back(nombre);
            }

            if (vec.size() == 2){
                vec.push_back(1);
            }


            Aretes[vec[0]][vec[1]] = vec[2];
            Degres[vec[0]] += 1;
            Degres[vec[1]] += 1;

            if (! Sommets.count(vec[0])) {
                Sommets.insert(vec[0]);
            }
            if (! Sommets.count(vec[1])) {
                Sommets.insert(vec[1]);
            }

        }
    }
}


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

// Renvoie le poids de l'arête entre les sommets si elle existe, 0 sinon
double Graph::w_xy(int x, int y) {
    double w = 0;
    if (Aretes.contains(x)) {
        for (auto & [arrivee, poids] : Aretes[x]) {
            if (arrivee == y) {
                w = poids;
            }
        }
    }
    if (Aretes.contains(y)) {
        for (auto & [arrivee, poids] : Aretes[y]) {
            if (arrivee == x) {
                w = poids;
            }
        }
    }
    return w;
}


bool find(const vector<int> &v, int a){
    for (auto &x : v){
        if (x==a){
            return true;
        }
    }
    return false;
}