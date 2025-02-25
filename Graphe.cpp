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

    
    



    // Arêtes + degrés

    degres.resize(nbSommets + 1, 0);

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


            aretes[vec[0]][vec[1]] = vec[2];
            degres[vec[0]] += 1;
            degres[vec[1]] += 1;

        }
    }
}

void Graph::print_graph(){      // Affiche le graphe
    
    cout << "n = " << nbSommets << ", " << nbAretes << " arêtes" << endl << "dmin = " << dmin <<", dmax = " << dmax << endl;
    for (auto &[sommet, arete] : aretes){
        cout << "Sommet " << sommet << " (degré = " << degres[sommet] << ") :" << endl;
        for (auto &[voisin, poids] : arete){
            cout << "Voisin " << voisin << " : poids " << poids << endl;
        }
        cout << endl;
    }

}


