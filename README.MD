#Requirements
 - sudo apt-get install libssl


#Compilation du projet
 - make compilation

#Exemples d'utilisation
 - ./main -g liste_français.txt -out secret -algo SHA512
    * Permet de chiffrer toutes les entrées présentes dans liste_français.txt et sauvegarde le résultat dans un fichier "secret"

- ./main -l secret
    * Sauvegarde les entrées présentes dans le fichier "secret" et permet de faire une recherche rapide d'un mot en clair à partir d'un hash

#/!\ Le logiciel est très sensible à la casse au niveau des fichiers d'entrée. Il faut s'assurer qu'il n'y ait pas de saut de ligne en fin de fichier. Il faut également que l'écart entre le mot en clair et son hash soit une tabulation.