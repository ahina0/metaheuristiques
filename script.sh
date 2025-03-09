#Vérifie qu'on a bien le nombre de classes en argument
if [ -z "$1" ]; then
  echo "Erreur : vous devez fournir un argument (nombre de classes)."
  exit 1
fi

fres=../results/${1}.txt
data="../data/"
# "cinqCent" "mille" "dixMille" 
Exemples=("quatre" "cinq" "dix" "quinze" "dixSept" "vingt" "vingtEtun" "vingtDeux" "vingtTrois" "vingtQuatre" "vingtCinq" "trente" "cinquante" "cent")
Autres=("RG_50_5" "RG_23_5" "connex_10" "huitSommets_12" "deuxTriangles" "kl_graph" "huitSommets_11" "centSommets_3" "RG_13_3" "dixSommets_2" "CG_25_4" "cG_25_4" "random_10" "huitSommets_weight" "test_cluster" "huitSommets" "test_gno" "28_3_clusters_ClusterGraph" "10_3_graph" "28_3_clusters" "28_3_clusters_SparseCluster" "28_3_clusters_SomeCluster" "28_3_clusters_EquiCluster")


echo "Partitionnement en ${1} classes" > $fres
echo >> $fres


#exemples de graphes:
for ex in ${Exemples[@]}; do
    echo ""
    echo "${ex}Sommets.txt"
    echo >> $fres
    echo >> $fres
    echo "${ex}Sommets.txt" >> $fres
    echo >> $fres
    ./exec ${data}Exemples\ de\ graphes/${ex}Sommets.txt $1  #Avec le nombre de classes en paramètre
done
echo "Dossier 'Exemples de graphes' fini"
echo ""
echo ""


# autres exemples:
for ex in ${Autres[@]}; do
    echo ""
    echo "${ex}.txt"
    echo >> $fres
    echo >> $fres
    echo "${ex}.txt" >> $fres
    echo >> $fres
    ./exec ${data}Autres\ Exemples/${ex}.txt $1
done
echo "Dossier 'Autres Exemples' fini"

#Football
echo ""
echo "football.txt"
echo >> $fres
echo >> $fres
echo "football.txt" >> $fres
./exec ${data}Football/football.txt $1

