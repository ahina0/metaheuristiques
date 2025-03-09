#include "Graphe.hpp"
#include "Solution.hpp"
#include "Enumeration.hpp"
#include "Glouton.hpp"
#include <algorithm>
#include <chrono>
#include <fstream>



using namespace std::chrono;


int main(int argc, char ** argv){

    auto t1=steady_clock::now(), t2=steady_clock::now();
    milliseconds s;

    srand(time(nullptr));

    // Utilisation du script
    if (argc==3){

        int k = stoi(argv[2]);
        string filename = string(argv[1]);
        string file_results = "../results/";
        file_results += string(argv[2]) + ".txt";


        Graph G(filename);

        ofstream file(file_results, ios::app);

        // Enumeration
        file << "Enumeration :\n";
        if (G.nbSommets > 15){
            file << "Nombre de sommets trop grand pour l'énumération\n";
        } else {
            t1 = steady_clock::now();
            Enumeration E(G,k);
            double min = E.explicite();
            t2 = steady_clock::now();
            s = duration_cast<milliseconds>(t2 - t1);
            file << "Valeur optimale par énumération explicite pour " << E.nbClasses << " classes : " << min << "\n";
            file << "temps en ms : " << s.count() << "\n";
        }
        file << "\n";


        // Gradient
        file << "Gradient :\n";
        int iter_max = 100;
        int iter_algo = 100;
        Glouton Gl(G,k);

        vector<double> optimaux(iter_algo);
        double somme = 0;
        t1 = steady_clock::now();
        for (int i=0; i<iter_algo; i++){
            Glouton Gl(G,k);
            optimaux[i] = Gl.gradient(iter_max);
            somme += optimaux[i];
        }
        t2 = steady_clock::now();
        s = duration_cast<milliseconds>(t2 - t1);

        // Calculs statistiques
        sort(optimaux.begin(), optimaux.end());
        file << "Meilleur optimum local : " << optimaux[0] << "\n";
        file << "Pire optimum local : " << optimaux[iter_algo - 1] << "\n";
        file << "Moyenne : " << somme/iter_algo << "\n";

        // Médiane
        file << "Médiane : ";
        if (iter_algo % 2 == 0){
            file << (optimaux[iter_algo/2 - 1] + optimaux[iter_algo/2]) / 2. << "\n";
        } else {
            file << optimaux[iter_algo/2] << "\n";
        }

        // Nombre de meilleurs opt locaux
        int nbOpt = 1;
        while(abs(optimaux[0] - optimaux[nbOpt]) < 1e-6){
            nbOpt += 1;
        }
        file << "Meilleur optimum local trouvé " << nbOpt << " fois" << "\n";
        file << "temps en ms : " << s.count() << "\n";





    } else {

        string filename = "test.txt";

        if (argc == 2){
            filename = argv[1];
        }

        Graph G(filename);

        bool fin = false;
        int k = 2;

        while (!fin){

            cout << endl << "partitionnement en " << k << " classes" << endl << endl;
            cout << "1 pour afficher le graphe" << endl << "2 pour utiliser l'algolrithme d'énumération (pas plus de 25 sommets)" << endl;
            cout << "3 pour la méthode du gradient sans optimisation" << endl;
            cout << "4 pour changer de fichier"  << endl << "5 pour changer le nombre de classes" << endl << "0 pour quitter" << endl;
            int i;
            cin >> i;

            int iter_algo = 0;
            int iter_max = 0;
            cout << endl;

            switch(i){
                case 1:
                    G.print_graph();
                    break;

                case 2:
                    t1 = steady_clock::now();

                    {Enumeration E(G,k);
                    E.explicite();}

                    t2 = steady_clock::now();
                    s = duration_cast<milliseconds>(t2 - t1);
                    cout << "temps en ms : " << s.count() << endl;
                    break;

                case 3:

                    while (iter_algo < 1){
                        cout << "Nombre de lancements de l'algorithme : ";
                        cin >> iter_algo;
                    }
                    while (iter_max < 1){
                        cout << "Nombre max d'itérations : ";
                        cin >> iter_max;
                    }
                    if (iter_algo==1){
                        Glouton Gl(G,k);
                        double opti = Gl.gradient(iter_max);
                        cout << "Minimum local : " << opti;
                    } else {
                        vector<double> optimaux(iter_algo);
                        double somme = 0;
                        
                        t1 = steady_clock::now();
                        
                        for (int i=0; i<iter_algo; i++){
                            Glouton Gl(G,k);
                            optimaux[i] = Gl.gradient(iter_max);
                            somme += optimaux[i];
                        }
                        
                        t2 = steady_clock::now();
                        s = duration_cast<milliseconds>(t2 - t1);
                        cout << "temps en ms : " << s.count() << endl;
                        
                        // Calculs statistiques
                        sort(optimaux.begin(), optimaux.end());
                        cout << "Meilleur optimum local : " << optimaux[0] << endl;
                        cout << "Pire optimum local : " << optimaux[iter_algo - 1] << endl;
                        cout << "Moyenne : " << somme/iter_algo << endl;

                        // Médiane
                        cout << "Médiane : ";
                        if (iter_algo % 2 == 0){
                            cout << (optimaux[iter_algo/2 - 1] + optimaux[iter_algo/2]) / 2. << endl;
                        } else {
                            cout << optimaux[iter_algo/2] << endl;
                        }

                        // Nombre de meilleurs opt locaux
                        int nbOpt = 1;
                        while(abs(optimaux[0] - optimaux[nbOpt]) < 1e-6){
                            nbOpt += 1;
                        }
                        cout << "Meilleur optimum local trouvé " << nbOpt << " fois" << endl;
                    }
                    break;

                case 4:
                    cout << "Nom du fichier : ";
                    cin >> filename;
                    G = Graph(filename);
                    break;

                case 5:
                    cout << "k : ";
                    cin >> k;
                    break;

                default:
                    cout << "Fin du programme" << endl;
                    fin = true;
                    break;

            }
        }
    } 
    
    return 0;
}