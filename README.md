# IR-2022-Projet

## Sommaires
- [IR-2022-Projet](#ir-2022-projet)
  - [Sommaires](#sommaires)
  - [Descriptions du projets](#descriptions-du-projets)
    - [Utilité de l'application](#utilité-de-lapplication)
    - [Structure de projet](#structure-de-projet)
    - [Fichiers binaire](#fichiers-binaire)
      - [Parsage du CSV](#parsage-du-csv)
  - [Pourquoi Delaunay ?](#pourquoi-delaunay-)
  - [Exécution](#exécution)
    - [Utilisation de l'exécutable](#utilisation-de-lexécutable)
    - [Lancement des tests unitaires](#lancement-des-tests-unitaires)
    - [Génération de la documentation](#génération-de-la-documentation)
  - [Aperçu](#aperçu)
  - [Notes](#notes)
  - [Auteurs](#auteurs)

## Descriptions du projets

### Utilité de l'application

Le but du projet est d'utiliser [la base de données des arbres de Paris](https://opendata.paris.fr/explore/dataset/les-arbres/information/?disjunctive.typeemplacement&disjunctive.arrondissement&disjunctive.libellefrancais&disjunctive.genre&disjunctive.espece&disjunctive.varieteoucultivar&disjunctive.stadedeveloppement&disjunctive.remarquable) stockée au format CSV, afin d'en
extraire un arbre de poids minimal le poids d'un entre arbres étant ça distance ici (plan euclidien et sur une sphere). 

Cela à étais réalisé en utilisant l'algorithme de [Bowyer-Watson](https://fr.wikipedia.org/wiki/Algorithme_de_Bowyer-Watson?wprov=srpw1_0) pour réaliser la [Triangulation de Delaunay](https://fr.wikipedia.org/wiki/Triangulation_de_Delaunay), et l'algorithme de [Prim](https://fr.wikipedia.org/wiki/Triangulation_de_Delaunay) pour construitre l'arbre de poids minimal.

### Structure de projet

La structure du projet est décomposé de cette façon :
```
├── include
├── src
├── tests
├── Makefile
├── Doxyfile
└── README.md
```
Où chaque ``.c`` est présent dans ``src`` et chaque ``.h`` dans ``include/``. Les tests unitaires sont présent dans ``tests/``.

### Fichiers binaire
Deux fichiers binaire peuvent être généré lors de l’exécution
#### Parsage du CSV
Lorsque le csv est traité, un fichier binaire et créé contenant des données sous forma 
```
double  latitude;
double  longitude;
int  id;
```
## Pourquoi Delaunay ?
La triangulation de Delaunay porte un propriété intéressante tel que : [l'arbre euclidien couvrant de poids minimal est un sous graphe de la triangulation](https://fr.wikipedia.org/wiki/Triangulation_de_Delaunay#Applications). Sachant que cette construction peut ce faire en O(log n) en utilisant une approche divisé pour régné et les structures de donnée adapté (Quad Edge), cela est alors très intéressant dans notre cas ([exemple d'implémentation](https://github.com/alexbaryzhikov/triangulation)). Malheureusement nous avons du nous limiter à une proche itératif en passant par l'algorithme de [Bowyer-Watson](https://fr.wikipedia.org/wiki/Algorithme_de_Bowyer-Watson) qui ce fait en O(n²).

## Exécution

### Utilisation de l'exécutable
Vous devez d'abord être à la racine pour compiler le projet avec la commande ``make``. Deux dossier seront créé : ``obj/`` et ``bin/``. L'exécutable ce trouve dans ``bin/``.

Pour lancer l'application: ``./bin/main [OPTIONS]``
L'ensembles des options est disponible avec la commande ``./bin/main --help``

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

- ``./bin/main -i ../les-arbres.csv -o coor.bin -g -v -l tests/files/delaunay/to_load/delaunay_256K
`` Lis le fichier présent dans ``../les-arbres`` sauvegarde le fichier binaire contenant les informations nécessaire dans ``coor.bin``, active les messages pour débugger et affiche le résultat dans une fênetre et lis le fichier binaire ``delaunay_256K`` où ce trouve la triangulation pré enregistré.

### Lancement des tests unitaires

Pour lancer les testes unitaires, vous pouvais les exécutées à partir de :
 - la racine du répertoire avec ``make test`` 
 - dans le dossier ``tests/`` avec ``make tests`` 

Vous pouvez aussi obtenir un code coverage avec ``make cov`` à la racine ou dans ``tests/``, un rapport sera alors généré dans ``tests/rapport`` ou ce trouvera un ``index.html`` contenant le code coverage.

*Note: il est nécessaire d'avoir lcov d'installé sur votre machine.* 

### Génération de la documentation
Il suffit d'exécuter ``make doc`` à la racine du projet, un dossier ``docs`` sera alors généré et la documentation serra accessible via ``docs/html/index.html``.


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

## Notes
- La librairie ``argp`` est utilisé dans ce projet, il ce peut que sur certaine platforme (macOS) le projet ne compile pas dû au faite que cette librairie est peut être manquante.
- Des cercles rouges peuvent apparaitres dans l'affichage du résultats de Prim, en effet lorsque des doublons seront présent dans le CSV fournis, un des deux sera traité et l'autre non traité.


## Auteurs
* Loïc Herlin
* [Alexander Yanovskyy](https://www.yanovskyy.com)
