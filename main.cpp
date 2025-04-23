#include "Graphe.hpp"
#include "Solution.hpp"
#include "Enumeration.hpp"
#include "Glouton.hpp"
#include "KL.hpp"
#include "Recuit.hpp"
#include "Tabou.hpp"
#include <algorithm>
#include <fstream>




int main(int argc, char ** argv){

    auto t1=steady_clock::now(), t2=steady_clock::now();
    milliseconds s;

    srand(time(nullptr));

    // Utilisation du script
    if (argc==3){

        int k = stoi(argv[2]);
        
        string filenameC = string(argv[1]);
        string file_results = "../results/";
        file_results += string(argv[2]) + "_classes.txt";
        string file_enum = "../results/Enum_";
        file_enum += string(argv[2]) + ".txt";
        string file_grad = "../results/Grad_";
        file_grad += string(argv[2]) + ".txt";
        string file_grad_heur = "../results/Grad_Heur_";
        file_grad_heur += string(argv[2]) + ".txt";
        string file_kl = "../results/KL_";
        file_kl += string(argv[2]) + ".txt";
        string file_recuit = "../results/RS_";
        file_recuit += string(argv[2]) + ".txt";
        string file_tabou = "../results/Tabou_";
        file_tabou += string(argv[2]) + ".txt";
        string file_aspi = "../results/Aspi_";
        file_aspi += string(argv[2]) + ".txt";


        Graph G(filenameC);

        ofstream fileR(file_results, ios::app);
        ofstream fileE(file_enum, ios::app);
        ofstream fileG(file_grad, ios::app);
        ofstream fileH(file_grad_heur, ios::app);
        ofstream fileK(file_kl, ios::app);
        ofstream fileRS(file_recuit, ios::app);
        ofstream fileT(file_tabou, ios::app);
        ofstream fileA(file_aspi, ios::app);


        // Pour enlever le chemin du nom du fichier
        size_t pos = filenameC.find_last_of("/\\");
        string filename = (pos ==  string::npos) ? filenameC : filenameC.substr(pos + 1);
        
        
        double time_lim = 3600000;

        if (G.nbSommets > k){    
            int i=0;

            /*// Enumeration
            cout << "   Enumeration" << endl;
            fileR << "Enumeration :\n";
            if (pow(k,G.nbSommets) > 1.5e8 || G.nbSommets > 25){    // Condition sur le nombre de classes  
                fileR << "Nombre de sommets trop grand pour l'énumération pour " << k << " classes\n";
            } else {
                fileE << filename << " " << G.nbSommets << " ";
                t1 = steady_clock::now();
                Enumeration E(G,k);
                double min = E.explicite();
                t2 = steady_clock::now();
                s = duration_cast<milliseconds>(t2 - t1);
                fileR << "Valeur optimale par énumération explicite pour " << E.nbClasses << " classes : " << min << "\n";
                fileE << min << " ";
                fileR << "temps en ms : " << s.count() << "\n";
                fileE << s.count() << "\n";
            }
            fileR << "\n";
*/


            // Gradient
            cout << "   Gradient" << endl;
            fileR << "Gradient :\n";
            int iter_max = 500;
            int iter_algo = 100;

            vector<double> optimaux(0);
            double somme = 0;
            fileG << filename << " " << G.nbSommets << " ";
            t1 = steady_clock::now();
            
            i = 0;
            optimaux.resize(0);
            while (i<iter_algo && duration_cast<milliseconds>(steady_clock::now() - t1).count() < time_lim){
                Glouton Gl(G,k);
                optimaux.push_back(Gl.gradient(iter_max));
                somme += optimaux[i];
                i++;
            }
            t2 = steady_clock::now();
            s = duration_cast<milliseconds>(t2 - t1);

            // Calculs statistiques
            sort(optimaux.begin(), optimaux.end());
            fileR << "Meilleur optimum local : " << optimaux[0] << "\n";
            fileG << optimaux[0] << " ";
            fileR << "Pire optimum local : " << optimaux[optimaux.size() - 1] << "\n";
            //fileG << optimaux[iter_algo - 1] << " ";
            fileR << "Moyenne : " << somme/i << "\n";
            //fileG << somme/iter_algo << " ";

            // Médiane
            fileR << "Médiane : ";
            if (i % 2 == 0){
                fileR << (optimaux[i/2 - 1] + optimaux[i/2]) / 2. << "\n";
                //fileG << (optimaux[iter_algo/2 - 1] + optimaux[iter_algo/2]) / 2. << " ";
            } else {
                fileR << optimaux[i/2] << "\n";
                //fileG << optimaux[iter_algo/2] << " ";
            }

            // Nombre de meilleurs opt locaux
            int nbOpt = 1;
            while(abs(optimaux[0] - optimaux[nbOpt]) < 1e-6 && nbOpt < i){
                nbOpt += 1;
            }
            fileR << "Meilleur optimum local trouvé " << nbOpt << " fois" << "\n";
            //fileG << nbOpt << " ";
            fileR << "temps en ms : " << s.count() << "\n";
            fileG << s.count()/i << " " << i << "\n";



            // Gradient avec heuristique
            cout << "   Heuristique" << endl;
            fileR << "Gradient avec heuristique :\n";

            int K = 10;
            optimaux.resize(0);
            somme = 0;
            fileH << filename << " " << G.nbSommets << " ";
            t1 = steady_clock::now();
            
            i = 0;
            while (i<iter_algo && duration_cast<milliseconds>(steady_clock::now() - t1).count() < time_lim){
                Glouton Gl(G,k);
                optimaux.push_back(Gl.heuristique(iter_max,K));
                somme += optimaux[i];
                i++;
            }
            t2 = steady_clock::now();
            s = duration_cast<milliseconds>(t2 - t1);

            // Calculs statistiques
            sort(optimaux.begin(), optimaux.end());
            fileR << "Meilleur optimum local : " << optimaux[0] << "\n";
            fileH << optimaux[0] << " ";
            fileR << "Pire optimum local : " << optimaux[optimaux.size() - 1] << "\n";
            //fileG << optimaux[iter_algo - 1] << " ";
            fileR << "Moyenne : " << somme/i << "\n";
            //fileG << somme/iter_algo << " ";

            // Médiane
            fileR << "Médiane : ";
            if (i % 2 == 0){
                fileR << (optimaux[i/2 - 1] + optimaux[i/2]) / 2. << "\n";
                //fileG << (optimaux[iter_algo/2 - 1] + optimaux[iter_algo/2]) / 2. << " ";
            } else {
                fileR << optimaux[i/2] << "\n";
                //fileG << optimaux[iter_algo/2] << " ";
            }

            // Nombre de meilleurs opt locaux
            nbOpt = 1;
            while(abs(optimaux[0] - optimaux[nbOpt]) < 1e-6 && nbOpt < i){
                nbOpt += 1;
            }
            fileR << "Meilleur optimum local trouvé " << nbOpt << " fois" << "\n";
            //fileG << nbOpt << " ";
            fileR << "temps en ms : " << s.count() << "\n";
            fileH << s.count()/i << " " << i << "\n";


            if (k < 3){
            // Kernighan Lin
            cout << "   Kernighan Lin" << endl;
            fileR << "KL :\n";

            optimaux.resize(0);
            somme = 0;
            fileK << filename << " " << G.nbSommets << " ";
            t1 = steady_clock::now();
            
            i = 0;
            while (i<iter_algo && duration_cast<milliseconds>(steady_clock::now() - t1).count() < time_lim){
                KL K(G);
                optimaux.push_back(K.Kernighan_Lin(iter_max));
                somme += optimaux[i];
                i++;
            }
            t2 = steady_clock::now();
            s = duration_cast<milliseconds>(t2 - t1);

            // Calculs statistiques
            sort(optimaux.begin(), optimaux.end());
            fileR << "Meilleur optimum local : " << optimaux[0] << "\n";
            fileK << optimaux[0] << " ";
            fileR << "Pire optimum local : " << optimaux[optimaux.size() - 1] << "\n";
            //fileG << optimaux[iter_algo - 1] << " ";
            fileR << "Moyenne : " << somme/i << "\n";
            //fileG << somme/iter_algo << " ";

            // Médiane
            fileR << "Médiane : ";
            if (i % 2 == 0){
                fileR << (optimaux[i/2 - 1] + optimaux[i/2]) / 2. << "\n";
                //fileG << (optimaux[iter_algo/2 - 1] + optimaux[iter_algo/2]) / 2. << " ";
            } else {
                fileR << optimaux[i/2] << "\n";
                //fileG << optimaux[iter_algo/2] << " ";
            }

            // Nombre de meilleurs opt locaux
            nbOpt = 1;
            while(abs(optimaux[0] - optimaux[nbOpt]) < 1e-6 && nbOpt < i){
                nbOpt += 1;
            }
            fileR << "Meilleur optimum local trouvé " << nbOpt << " fois" << "\n";
            //fileG << nbOpt << " ";
            fileR << "temps en ms : " << s.count() << "\n";
            fileK << s.count()/i << " " << i << "\n";
            }
            
            
            // Recuit Simule
            cout << "   Recuit Simule" << endl;
            fileR << "Recuit Simule :\n";

            double tau = 0.5;
            optimaux.resize(0);
            somme = 0;
            fileRS << filename << " " << G.nbSommets << " ";
            t1 = steady_clock::now();
            
            i = 0;
            while (i<iter_algo && duration_cast<milliseconds>(steady_clock::now() - t1).count() < time_lim){
                Recuit RS(G,k,tau);
                optimaux.push_back(RS.Recuit_Simule());
                somme += optimaux[i];
                i++;
            }
            t2 = steady_clock::now();
            s = duration_cast<milliseconds>(t2 - t1);

            // Calculs statistiques
            sort(optimaux.begin(), optimaux.end());
            fileR << "Meilleur optimum local : " << optimaux[0] << "\n";
            fileRS << optimaux[0] << " ";
            fileR << "Pire optimum local : " << optimaux[optimaux.size() - 1] << "\n";
            //fileG << optimaux[iter_algo - 1] << " ";
            fileR << "Moyenne : " << somme/i << "\n";
            //fileG << somme/iter_algo << " ";

            // Médiane
            fileR << "Médiane : ";
            if (i % 2 == 0){
                fileR << (optimaux[i/2 - 1] + optimaux[i/2]) / 2. << "\n";
                //fileG << (optimaux[iter_algo/2 - 1] + optimaux[iter_algo/2]) / 2. << " ";
            } else {
                fileR << optimaux[i/2] << "\n";
                //fileG << optimaux[iter_algo/2] << " ";
            }

            // Nombre de meilleurs opt locaux
            nbOpt = 1;
            while(abs(optimaux[0] - optimaux[nbOpt]) < 1e-6 && nbOpt < i){
                nbOpt += 1;
            }
            fileR << "Meilleur optimum local trouvé " << nbOpt << " fois" << "\n";
            //fileG << nbOpt << " ";
            fileR << "temps en ms : " << s.count() << "\n";
            fileRS << s.count()/i << " " << i << "\n";


            // Tabou sans aspiration
            cout << "   Tabou" << endl;
            fileR << "Tabou sans aspiration :\n";

            K = 10;
            optimaux.resize(0);
            somme = 0;
            fileT << filename << " " << G.nbSommets << " ";
            t1 = steady_clock::now();
            
            i = 0;
            while (i<iter_algo && duration_cast<milliseconds>(steady_clock::now() - t1).count() < time_lim){
                Tabou T(G,k);
                optimaux.push_back(T.Basique(5, K));
                somme += optimaux[i];
                i++;
            }
            t2 = steady_clock::now();
            s = duration_cast<milliseconds>(t2 - t1);

            // Calculs statistiques
            sort(optimaux.begin(), optimaux.end());
            fileR << "Meilleur optimum local : " << optimaux[0] << "\n";
            fileT << optimaux[0] << " ";
            fileR << "Pire optimum local : " << optimaux[optimaux.size() - 1] << "\n";
            //fileG << optimaux[iter_algo - 1] << " ";
            fileR << "Moyenne : " << somme/i << "\n";
            //fileG << somme/iter_algo << " ";

            // Médiane
            fileR << "Médiane : ";
            if (i % 2 == 0){
                fileR << (optimaux[i/2 - 1] + optimaux[i/2]) / 2. << "\n";
                //fileG << (optimaux[iter_algo/2 - 1] + optimaux[iter_algo/2]) / 2. << " ";
            } else {
                fileR << optimaux[i/2] << "\n";
                //fileG << optimaux[iter_algo/2] << " ";
            }

            // Nombre de meilleurs opt locaux
            nbOpt = 1;
            while(abs(optimaux[0] - optimaux[nbOpt]) < 1e-6 && nbOpt < i){
                nbOpt += 1;
            }
            fileR << "Meilleur optimum local trouvé " << nbOpt << " fois" << "\n";
            //fileG << nbOpt << " ";
            fileR << "temps en ms : " << s.count() << "\n";
            fileT << s.count()/i << " " << i << "\n";



            // Tabou avec aspiration
            cout << "   Aspiration" << endl;
            fileR << "Tabou avec aspiration :\n";

            optimaux.resize(0);
            somme = 0;
            fileA << filename << " " << G.nbSommets << " ";
            t1 = steady_clock::now();
            
            i = 0;
            while (i<iter_algo && duration_cast<milliseconds>(steady_clock::now() - t1).count() < time_lim){
                Tabou T(G,k);
                optimaux.push_back(T.Aspiration(5,K));
                somme += optimaux[i];
                i++;
            }
            t2 = steady_clock::now();
            s = duration_cast<milliseconds>(t2 - t1);

            // Calculs statistiques
            sort(optimaux.begin(), optimaux.end());
            fileR << "Meilleur optimum local : " << optimaux[0] << "\n";
            fileA << optimaux[0] << " ";
            fileR << "Pire optimum local : " << optimaux[optimaux.size() - 1] << "\n";
            //fileG << optimaux[iter_algo - 1] << " ";
            fileR << "Moyenne : " << somme/i << "\n";
            //fileG << somme/iter_algo << " ";

            // Médiane
            fileR << "Médiane : ";
            if (i % 2 == 0){
                fileR << (optimaux[i/2 - 1] + optimaux[i/2]) / 2. << "\n";
                //fileG << (optimaux[iter_algo/2 - 1] + optimaux[iter_algo/2]) / 2. << " ";
            } else {
                fileR << optimaux[i/2] << "\n";
                //fileG << optimaux[iter_algo/2] << " ";
            }

            // Nombre de meilleurs opt locaux
            nbOpt = 1;
            while(abs(optimaux[0] - optimaux[nbOpt]) < 1e-6 && nbOpt < i){
                nbOpt += 1;
            }
            fileR << "Meilleur optimum local trouvé " << nbOpt << " fois" << "\n";
            //fileG << nbOpt << " ";
            fileR << "temps en ms : " << s.count() << "\n";
            fileA << s.count()/i << " " << i << "\n";

        } else {
        fileR << "On ne traite pas les graphes en dessous de " << k+1 << " sommets pour " << k << " classes\n";
        }



    } else if (argc==2){

        string filename;

        //if (argc == 2){
            filename = argv[1];
        /*} else {
            cout << "Erreur : un argument requis (nom du fichier)." << endl;
            abort();
        } */

        Graph G(filename);

        bool fin = false;
        int k = 2;

        while (!fin){

            cout << endl << "partitionnement en " << k << " classes" << endl << endl;
            cout << "0 pour quitter" << endl << "1 pour afficher le graphe" << endl;
            cout << "2 pour changer de fichier"  << endl << "3 pour changer le nombre de classes" << endl;
            cout << "4 pour utiliser l'algorithme d'énumération (pas plus de 25 sommets)" << endl;
            cout << "5 pour la méthode du gradient sans optimisation" << endl;
            cout << "6 pour la méthode du gradient avec optimisation" << endl;
            cout << "7 pour utiliser l'algorithme de Kernighan et Lin (seulement pour k=2)" << endl;
            cout << "8 pour utiliser l'algorithme du recuit simulé" << endl;
            cout << "9 pour utiliser la méthode tabou sans aspiration" << endl;
            cout << "10 pour utiliser la méthode tabou avec aspiration" << endl;
            int i;
            cin >> i;

            int iter_algo = 0;
            int iter_max = 0;
            int K = 0;
            int taille_tabou = 0;
            double tau = -1;
            cout << endl;

            switch(i){
                case 1:
                    G.print_graph();
                    break;

                case 2:
                    cout << "Nom du fichier : ";
                    cin >> filename;
                    G = Graph(filename);
                    break;

                case 3:
                    cout << "k : ";
                    cin >> k;
                    break;

                case 4:
                    t1 = steady_clock::now();

                    {Enumeration E(G,k);
                    double opti = E.explicite();

                    t2 = steady_clock::now();
                    s = duration_cast<milliseconds>(t2 - t1);
                    cout << "Minimum local : " << opti << endl;
                    cout << "Temps en ms : " << s.count() << endl;}
                    break;

                case 5:

                    while (iter_algo < 1){
                        cout << "Nombre de lancements de l'algorithme : ";
                        cin >> iter_algo;
                    }
                    while (iter_max < 1){
                        cout << "Nombre max d'itérations : ";
                        cin >> iter_max;
                    }
                    if (iter_algo==1){
                        t1 = steady_clock::now();
                        Glouton Gl(G,k);
                        double opti = Gl.gradient(iter_max);
                        t2 = steady_clock::now();
                        s = duration_cast<milliseconds>(t2 - t1);
                        cout << "Minimum local : " << opti << endl;
                        cout << "Temps en ms : " << s.count() << endl;
                    } else {
                        vector<double> optimaux(iter_algo);
                        double somme = 0;
                        
                        t1 = steady_clock::now();
                        
                        for (int i=0; i<iter_algo; i++){
                            Glouton Gl(G,k);
                            optimaux[i] = Gl.gradient(iter_max);
                            somme += optimaux[i];
                            if ((i%100)==0){
                                cout << "Itération " << i << endl;
                            }
                        }

                        t2 = steady_clock::now();
                        s = duration_cast<milliseconds>(t2 - t1);
                        cout << "Temps en ms : " << s.count() << endl;

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
                        while(abs(optimaux[0] - optimaux[nbOpt]) < 1e-6 && nbOpt<iter_algo){
                            nbOpt += 1;
                        }
                        cout << "Meilleur optimum local trouvé " << nbOpt << " fois" << endl;
                    }
                    break;

                case 6:
                    while (iter_algo < 1){
                        cout << "Nombre de lancements de l'algorithme : ";
                        cin >> iter_algo;
                    }
                    while (iter_max < 1){
                        cout << "Nombre max d'itérations : ";
                        cin >> iter_max;
                    }
                    while (K < 1) {
                        cout << "Nombre de solutions améliorantes voulues avant d'arrêter la recherche : ";
                        cin >> K;
                    }
                    if (iter_algo==1){
                        t1 = steady_clock::now();
                        Glouton Gl(G,k);
                        double opti = Gl.heuristique(iter_max, K);
                        t2 = steady_clock::now();
                        s = duration_cast<milliseconds>(t2 - t1);
                        cout << "Minimum local : " << opti << endl;
                        cout << "Temps en ms : " << s.count() << endl;
                    } else {
                        vector<double> optimaux(iter_algo);
                        double somme = 0;
                        
                        t1 = steady_clock::now();
                        
                        for (int i=0; i<iter_algo; i++){
                            Glouton Gl(G,k);
                            optimaux[i] = Gl.heuristique(iter_max, K);
                            somme += optimaux[i];
                        }

                        t2 = steady_clock::now();
                        s = duration_cast<milliseconds>(t2 - t1);
                        cout << "Temps en ms : " << s.count() << endl;

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
                        while(abs(optimaux[0] - optimaux[nbOpt]) < 1e-6 && nbOpt<iter_algo){
                            nbOpt += 1;
                        }
                        cout << "Meilleur optimum local trouvé " << nbOpt << " fois" << endl;
                    }
                    break;

                case 7:

                    if (k==2) {

                        while (iter_algo < 1){
                            cout << "Nombre de lancements de l'algorithme : ";
                            cin >> iter_algo;
                        }
                        while (iter_max < 1){
                            cout << "Nombre max d'itérations : ";
                            cin >> iter_max;
                        }
                        if (iter_algo==1){
                            t1 = steady_clock::now();
                            KL kl(G);
                            double opti = kl.Kernighan_Lin(iter_max);
                            t2 = steady_clock::now();
                            s = duration_cast<milliseconds>(t2 - t1);
                            cout << "Minimum local : " << opti << endl;
                            cout << "Temps en ms : " << s.count() << endl;
                        } else {
                            vector<double> optimaux(iter_algo);
                            double somme = 0;
                            
                            t1 = steady_clock::now();
                            
                            for (int i=0; i<iter_algo; i++){
                                KL kl(G);
                                optimaux[i] = kl.Kernighan_Lin(iter_max);
                                somme += optimaux[i];
                            }
                            
                            t2 = steady_clock::now();
                            s = duration_cast<milliseconds>(t2 - t1);
                            cout << "Temps en ms : " << s.count() << endl;
                            
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
                            while(abs(optimaux[0] - optimaux[nbOpt]) < 1e-6 && nbOpt<iter_algo){
                                nbOpt += 1;
                            }
                            cout << "Meilleur optimum local trouvé " << nbOpt << " fois" << endl;
                        }
                    }
                    break;

                case 8:
                
                    while (iter_algo < 1){
                        cout << "Nombre de lancements de l'algorithme : ";
                        cin >> iter_algo;
                    }
                    while (tau < 0 or tau > 1){
                        cout << "Valeur de tau : ";
                        cin >> tau;
                    }
                    if (iter_algo==1){
                        t1 = steady_clock::now();
                        Recuit R(G,k,tau);
                        double opti = R.Recuit_Simule();
                        t2 = steady_clock::now();
                        s = duration_cast<milliseconds>(t2 - t1);
                        cout << "Minimum local : " << opti << endl;
                        cout << "Temps en ms : " << s.count() << endl;
                    } else {
                        vector<double> optimaux(iter_algo);
                        double somme = 0;
                        
                        t1 = steady_clock::now();
                        
                        for (int i=0; i<iter_algo; i++){
                            cout << "Iteration " << i << endl;
                            Recuit R(G,k,tau);
                            optimaux[i] = R.Recuit_Simule();
                            somme += optimaux[i];
                        }

                        t2 = steady_clock::now();
                        s = duration_cast<milliseconds>(t2 - t1);
                        cout << "Temps en ms : " << s.count() << endl;

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
                        while(abs(optimaux[0] - optimaux[nbOpt]) < 1e-6 && nbOpt<iter_algo){
                            nbOpt += 1;
                        }
                        cout << "Meilleur optimum local trouvé " << nbOpt << " fois" << endl;
                    }
                    break;

                case 9:
                
                    while (iter_algo < 1){
                        cout << "Nombre de lancements de l'algorithme : ";
                        cin >> iter_algo;
                    }
                    while (K < 1) {
                        cout << "Nombre de solutions améliorantes voulues avant d'arrêter la recherche : ";
                        cin >> K;
                    }
                    while(taille_tabou < 1) {
                        cout << "Taille de la liste tabou : ";
                        cin >> taille_tabou;
                    }
                    if (iter_algo==1){
                        t1 = steady_clock::now();
                        Tabou T(G,k);
                        double opti = T.Basique(taille_tabou, K);
                        t2 = steady_clock::now();
                        s = duration_cast<milliseconds>(t2 - t1);
                        cout << "Minimum local : " << opti << endl;
                        cout << "Temps en ms : " << s.count() << endl;
                    } else {
                        vector<double> optimaux(iter_algo);
                        double somme = 0;
                        
                        t1 = steady_clock::now();
                        
                        for (int i=0; i<iter_algo; i++){
                            Tabou T(G,k);
                            optimaux[i] = T.Basique(taille_tabou, K);
                            somme += optimaux[i];
                        }

                        t2 = steady_clock::now();
                        s = duration_cast<milliseconds>(t2 - t1);
                        cout << "Temps en ms : " << s.count() << endl;

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
                        while(abs(optimaux[0] - optimaux[nbOpt]) < 1e-6 && nbOpt<iter_algo){
                            nbOpt += 1;
                        }
                        cout << "Meilleur optimum local trouvé " << nbOpt << " fois" << endl;
                    }
                    break;

                case 10:
                
                    while (iter_algo < 1){
                        cout << "Nombre de lancements de l'algorithme : ";
                        cin >> iter_algo;
                    }
                    while (K < 1) {
                        cout << "Nombre de solutions améliorantes voulues avant d'arrêter la recherche : ";
                        cin >> K;
                    }
                    while(taille_tabou < 1) {
                        cout << "Taille de la liste tabou : ";
                        cin >> taille_tabou;
                    }
                    if (iter_algo==1){
                        t1 = steady_clock::now();
                        Tabou T(G,k);
                        double opti = T.Aspiration(taille_tabou, K);
                        t2 = steady_clock::now();
                        s = duration_cast<milliseconds>(t2 - t1);
                        cout << "Minimum local : " << opti << endl;
                        cout << "Temps en ms : " << s.count() << endl;
                    } else {
                        vector<double> optimaux(iter_algo);
                        double somme = 0;
                        
                        t1 = steady_clock::now();
                        
                        for (int i=0; i<iter_algo; i++){
                            Tabou T(G,k);
                            optimaux[i] = T.Aspiration(taille_tabou, K);
                            somme += optimaux[i];
                        }

                        t2 = steady_clock::now();
                        s = duration_cast<milliseconds>(t2 - t1);
                        cout << "Temps en ms : " << s.count() << endl;

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
                        while(abs(optimaux[0] - optimaux[nbOpt]) < 1e-6 && nbOpt<iter_algo){
                            nbOpt += 1;
                        }
                        cout << "Meilleur optimum local trouvé " << nbOpt << " fois" << endl;
                    }
                    break;

                default:
                    cout << "Fin du programme" << endl;
                    fin = true;
                    break;

            }
        }
    } else {

    }
    
    return 0;
}