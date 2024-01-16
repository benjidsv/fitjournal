#!/bin/sh

# Define functions
print_help() {
    echo "Help function executed."
    echo "Usage: $0 data_path [options]"
    echo "Options:"
    echo "  -h    Show this help message and exit."
    echo "  -d    Execute the 'exec_d' function with argument '1' or '2'."
    echo "  -l    Execute the 'exec_l' function."
    echo "  -t    Execute the 'exec_t' function."
    echo "  -s    Execute the 'exec_s' function."
}

exec_d1() {
    echo "Conducteurs avec le plus trajets \n"

    cut -d ';' -f1,6 data.csv > extracted_columns.csv

    # Suppression des doublons de la colonne 1 et comptage des lignes uniques par personne (colonne 6)
    awk -F ';' '!seen[$1,$2]++ {count[$2]++} END {for (person in count) {print count[person], person}}' extracted_columns.csv > counted_unique_lines.csv

    # Tri par ordre décroissant en fonction du nombre de trajets et conservation des 10 premières lignes
    sort -rn counted_unique_lines.csv | head -n 10 > sorted_top10.csv

    # Affichage des 10 premières lignes du fichier résultant
    cat sorted_top10.csv

    echo "\n\n"
}

exec_d2() {
    echo "Conducteurs avec la plus grande distance\n"

    # Extraction des colonnes 5 et 6 (distances et noms)
    cut -d ';' -f5,6 data.csv > distances_and_names.csv

    # Somme des distances par personne
    awk -F ';' '{sum[$2]+=$1} END {for (person in sum) print sum[person], person}' distances_and_names.csv > summed_distances.csv

    # Trier par ordre décroissant et garder les 10 premiers résultats
    sort -rn summed_distances.csv | head -n 10 > top_10_distances.csv

    # Affichage des 10 distances les plus grandes par personne
    cat top_10_distances.csv

    echo "\n\n"
}

exec_l() {
    echo "10 trajets les plus longs\n"
    
    awk -F';' 'NR > 1 { distances[$1]+=$5 } END { for (route in distances) print route, distances[route] }' data.csv \
    | sort -t' ' -k2,2nr \
    | head -n 10 \
    | awk '{ print "Route ID: " $1, "Distance totale: " $2 }'

    echo "\n\n"
}

build_c() {
    cd "./progc"
    if [ ! -d "./build" ]; then
        mkdir "./build"
    fi
    cd "./build"

    rm -r ./*

    cmake ..
    cmake --build .

    chmod +x projetG

    cd "../../"
}

exec_t() {
    build_c
    ./progc/build/projetG $data_path T
}

exec_s() {
  build_c
  ./progc/build/projetG $data_path S
}

if [ $# -lt 1 ]; then
  echo "Error: Missing mandatory argument 'data_path'."
  echo "Usage: $0 data_path [options]"
  exit 1
fi

for arg in "$@"; do
  if [ "$arg" = "-h" ]; then
    print_help
    exit 0
  fi
done

data_path=$1
echo "Data path set to: $data_path"

if [ ! -d "./temp" ]; then
    mkdir "./temp"
elif [ -z "$(ls -A ./temp)" ]; then
    rm -r ./temp/*
fi

if [ ! -d "./images" ]; then
    mkdir "./images"
fi

if [ ! -d "./progc" ]; then
    exit 0
fi

shift

while getopts "hd:lts" opt; do
    case $opt in
        d)
            if [ "$OPTARG" = "1" ]; then
                exec_d1
            elif [ "$OPTARG" = "2" ]; then
                exec_d2
            else
                echo "Invalid argument for -d: $OPTARG"
            fi
            ;;
        l)
            exec_l
            ;;
        t)
            exec_t
            ;;
        s)
            exec_s
            ;;
        \?)
            echo "Invalid option: -$OPTARG" >&2
            ;;
    esac
done
