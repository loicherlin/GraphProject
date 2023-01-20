# IR-2022-Projet

## Sommaires
- [IR-2022-Projet](#ir-2022-projet)
  - [Sommaires](#sommaires)
  - [Descriptions du projets](#descriptions-du-projets)
    - [Utilité de l'application](#utilité-de-lapplication)
    - [Structure de projet](#structure-de-projet)
    - [Fichiers binaires](#fichiers-binaires)
      - [Parsage du CSV](#parsage-du-csv)
      - [Sauvegarde de la triangulation de Delaunay](#sauvegarde-de-la-triangulation-de-delaunay)
  - [Pourquoi Delaunay ?](#pourquoi-delaunay-)
  - [Aperçu](#aperçu)
  - [Exécution](#exécution)
    - [Utilisation de l'exécutable](#utilisation-de-lexécutable)
    - [Utilisation de l'interface graphique](#utilisation-de-linterface-graphique)
    - [Lancement des tests unitaires](#lancement-des-tests-unitaires)
    - [Génération de la documentation](#génération-de-la-documentation)
  - [Notes](#notes)
  - [Auteurs](#auteurs)

## Descriptions du projets

### Utilité de l'application

Le but du projet est d'utiliser [la base de données des arbres de Paris](https://opendata.paris.fr/explore/dataset/les-arbres/information/?disjunctive.typeemplacement&disjunctive.arrondissement&disjunctive.libellefrancais&disjunctive.genre&disjunctive.espece&disjunctive.varieteoucultivar&disjunctive.stadedeveloppement&disjunctive.remarquable) stockée au format CSV, afin d'en extraire un arbre couvrant de poids minimal où le poids entre deux arbres étant sa distance (plan euclidien ou sur une sphère).

Cela a été réalisé en utilisant l'algorithme de [Bowyer-Watson](https://fr.wikipedia.org/wiki/Algorithme_de_Bowyer-Watson?wprov=srpw1_0) pour réaliser la [Triangulation de Delaunay](https://fr.wikipedia.org/wiki/Triangulation_de_Delaunay), et l'algorithme de [Prim](https://fr.wikipedia.org/wiki/Triangulation_de_Delaunay) pour construitre l'arbre de poids minimal.

### Structure de projet

La structure du projet est décomposé de cette façon :
```
├── include
│   └── ...
├── src
│   └── ...
├── tests
│   └── ...
├── Makefile
├── Doxyfile
└── README.md
```
Où chaque ``.c`` est présent dans ``src`` et chaque ``.h`` dans ``include/``. Les tests unitaires sont présents dans ``tests/``.

### Fichiers binaires
Deux fichiers binaires peuvent être généré lors de l’exécution :
#### Parsage du CSV
Lorsque le csv est traité, un fichier binaire est créé contenant n-1 éléments sous la forme suivante :
```
8 octets (type double) + 8 octets (type double) + 4 octets (type int)
```
Nous avons par exemple en hexadécimal : 
```
....
87 1E FC B7 3E 6B 48 40 -> lattiude : 48.8378515225459
FF D2 48 D4 AE 34 02 40 -> : longitude : 2.275724085304659
3F 23 03 00 00 00 00 00 -> id relatif : 205 631
....
```
#### Sauvegarde de la triangulation de Delaunay
Le nombre d'arbres et le total de triangles sont écrits au début du fichier binaire sous la forme d'un ``size_t`` en 8 octets. Nous avons ensuite, pour chaque triangle, 3 ``int`` en 4 octets, représentant les id relatif formant le triangle.

Par exemple :  
```
40 23 03 00 00 00 00 00 -> total d'arbre : 205 632
59 46 06 00 00 00 00 00 -> total de triangle : 411 225
# premier triangle
29 09 00 00 00 00 00 00 -> id_0 : 2345
7D 02 00 00 12 0C 00 00 -> id_1 : 637
2A 07 00 00 9C 01 00 00 -> id_2 : 1834
.....
```

## Pourquoi Delaunay ?
La triangulation de Delaunay possède une propriété intéressante tel que : [l'arbre euclidien couvrant de poids minimal est un sous graphe de la triangulation de Delaunay](https://fr.wikipedia.org/wiki/Triangulation_de_Delaunay#Applications). Sachant que cette construction peut ce faire en O(log n) en utilisant une approche 'diviser pour mieux régner' et les structures de données adaptées (Quad Edge). C'est dans cette perspective que nous avons décidé d'implémenter Delaunay ([exemple d'implémentation](https://github.com/alexbaryzhikov/triangulation)). Malheureusement nous avons dû nous limiter à une proche itératif en passant par l'algorithme de [Bowyer-Watson](https://fr.wikipedia.org/wiki/Algorithme_de_Bowyer-Watson) qui se fait en O(n²).


## Aperçu 
<table>
<thead> 
	<tr> 
		<th colspan="4">Pour 205 632 arbres</th>
	</tr> 
</thead>
  <tr>
    <td>Prim</td>
    <td><img target="_blank" src="https://i.imgur.com/m2X20Lm.png" alt="Prim"></td>
  </tr>
  <tr>
    <td>Delaunay</td>
    <td><img target="_blank" src="https://i.imgur.com/WtplU3Y.png" alt="Delaunay"></td>
  </tr>
 <thead> 
	<tr> 
		<th colspan="4">Pour 10 000 arbres</th>
	</tr> 
</thead>
  <tr>
    <td>Prim</td>
    <td><img target="_blank" src="https://i.imgur.com/1VyjdHV.png" alt="Prim"></td>
  </tr>
  <tr>
    <td>Delaunay</td>
    <td><img target="_blank" src="https://i.imgur.com/tCEmxk9.png" alt="Delaunay"></td>
  </tr>
   <thead> 
	<tr> 
		<th colspan="4">Pour 100 arbres</th>
	</tr> 
</thead>
  <tr>
    <td>Prim</td>
    <td><img target="_blank" src="https://i.imgur.com/JI8nKby.png" alt="Prim"></td>
  </tr>
  <tr>
    <td>Delaunay</td>
    <td><img target="_blank" src="https://i.imgur.com/vtEu24N.png" alt="Delaunay"></td>
  </tr>
</table>

## Exécution

### Utilisation de l'exécutable
Vous devez d'abord être à la racine pour compiler le projet avec la commande ``make``. Deux dossiers seront créés : ``obj/`` et ``bin/``. L'exécutable se trouve dans ``bin/``.

Pour lancer l'application: ``./bin/main [OPTIONS]``
L'ensemble des options est disponible avec la commande ``./bin/main --help``

Qui renvoie ceci :

```
Usage: main [OPTION...]
Give the Minimum Spanning Tree based on input file (normaly Tree's of Paris)
using Delaunay Triangulation and Prim algorithm.

  -d, --delimiter=DELIMITER  Delimiter for parsing input file. Default is ';'.
  -g, --debug                Produce debugging output
  -h, --height=HEIGHT        Height of the visualisation window. (default 900)
  -i, --input=CSV_FILE
  -l, --load delaunay=BINARY_FILE
                             Load delaunay triangles from binary file.
  -m, --save mst=PATH        Save mst to plain txt file with "org - dest"
                             convention.
  -o, --output=BINARY_FILE
  -s, --save delaunay=PATH   Save delaunay triangles to binary file.
  -v, --visualise            Visualise the result.
  -w, --width=WIDTH          Width of the visualisation window. (default 1400)
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version
```
Exemple pour utiliser l'exécutable :

- ```./bin/main -i tests/files/csv/les-arbres.csv -o coor.bin -g -v -l tests/files/delaunay/to_load/delaunay_256K ``` 

La commande ci-dessus lis le fichier présent dans ``tests/files/csv/les-arbres.csv``, sauvegarde le fichier binaire contenant les informations nécessaire dans ``coor.bin``, active les messages pour débugger, affiche le résultat dans une fênetre et lis le fichier binaire ``delaunay_256K`` où se trouve la triangulation pré-enregistré.

### Utilisation de l'interface graphique
Il est possible d'utiliser une interface graphique pour visualiser les résultats grâce
à l'option ``-v``. Vous pouvez utiliser les flèches directionnelles pour vous déplacer ou clique gauche et bouger à la souris, pour zoomer ou dézoomer vous pouvez utiliser la molette de la souris ou les touches ``SPACE`` et ``A``.
### Lancement des tests unitaires

Pour lancer les testes unitaires, vous pouvez les exécuters à partir de :
 - la racine du répertoire avec ``make test`` 
 - dans le dossier ``tests/`` avec ``make tests`` 

Vous pouvez aussi obtenir un code coverage avec la commande ``make cov`` à la racine ou dans ``tests/``. Un rapport sera alors généré dans ``tests/rapport/`` où se trouvera un ``index.html`` contenant le code coverage.

*Note: il est nécessaire d'avoir lcov d'installé sur votre machine.* 

### Génération de la documentation
Il suffit d'exécuter ``make doc`` à la racine du projet, un dossier ``docs/`` sera alors généré et la documentation sera accessible via ``docs/html/index.html``.

## Notes
- La librairie ``tps`` est nécessaire pour compiler le projet, pour l'installer rendez-vous sur [ce lien](https://gitlab.com/bramas/libtps.h/).
- La librairie ``argp`` est utilisée dans ce projet, il se peut que sur certaine platforme (macOS) le projet ne compile pas dû au faite que cette librairie est peut être manquante.
- Des cercles rouges peuvent apparaitres dans l'affichage du résultats de Prim, en effet lorsque des doublons seront présents dans le CSV fourni, un des deux sera traité et l'autre non.


## Auteurs
* Loïc Herlin
* [Alexander Yanovskyy](https://www.yanovskyy.com)
