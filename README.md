# IR-2022-Projet

Loïc Herlin
Alexander yanovskyy

## Description : utilité de l'application

Le but du projet est d'utiliser la base de données des arbres de Paris, stockée au format CSV, afin d'en
extraire un sous-graphe sans cycle (un arbre) de poids minimum (le poids d'un lien étant ça longueur ici).
Imaginez que vous vouliez relier tous les arbres de Paris avec des cordes, de telle sorte que les écureils
puissent se balader partout. Cependant vous voulez utiliser le moins de corde possible.
Votre programme doit avoir plusieurs actions possible. 

La première est la conversion de la base de données pour la stocker dans un fichier binaire dont vous définirez le format. Ce fichier contiendra uniquement les informations nécessaire à la résolution du problème. 

La deuxième action consiste à prendre la base de données stockée en binaire et d'effectuer le calcul de l'arbre de poids minimum. Chaque action acceptera plusieurs arguments de ligne de commande afin de modifier le fonctionnement (comme choisir le chemin de sortie, customiser l'apparence, filtrer, etc ...)

## Description : structure de projet

La structure du projet est décomposé de cette façon :
- les fichiers d'entête sont dans le dossier include. Ces fichiers comprennent : la documentation pour chaque fonction, l'entête de chaque fonction et les structures de données utilisé.
- les fichiers c sont dans le dossier src. Ces fichiers comprennent : les différents include utilisé donc les différente structure de données, le code des fonctions décrite dans les fichiers d'entêtes. Le fichier C main est particulier puisqu'il va permettre, après compilation de lancer le programme.
- les tests sont dans le dossier tests. Le dossier file regroupe les fichiers important pour les testes comme un échantillon de 10 000 arbre de paris, un fichier make permettant de compiler les testes, des fichiers d'entête dans les quels les fonctions de testes ont été codé et teste.c qui une fois compilé permet de lancer les testes.

A la racine du projet, nous avons un fichier ``Doxyfile`` qui nous permet de générer la doc et un fichier make qui permet de compiler le projet.

## Comment doit être utilisé notre programme

### Utilisation de notre application
Lorsque vous avez les différents fichier sur votre ordinateurs, vous devez vous placer à la racine du projet et lancer la commande **Make** qui permet de compiler l'ensemble du projet. 
Pour lancer l'application, taper **./bin/main *[options]***
Voici l'ensemble des options : 
```mixed
|arg     |large arg  |comment|
|--------|--------|-----------|
|  -d | --delimiter=DELIMITER | Delimiter for parsing input file. Default is ';'. |
|  -g | --debug               | Produce debugging output |
|  -h | --height=HEIGHT       | Height of the visualisation window. (default 900)|
|  -i | --input=CSV_FILE      ||
|  -l | --load delaunay=BINARY_FILE | Load delaunay triangles from binary file.|
|  -m | --save mst=PATH       | Save mst to plain txt file with "org - dest" convention.|
|  -o | --output=BINARY_FILE  ||
|  -s | --save delaunay=PATH  | Save delaunay triangles to binary file.|
|  -v | --visualise           | Visualise the result.|
|  -w | --width=WIDTH         | Width of the visualisation window. (default 1400)|
|  -? | --help                | Give this help list|
|     | --usage               | Give a short usage message|
|  -V | --version             | Print program version|
```
Exemple ( Pour obtenir l'arbre couvrant minimal et delauney a partir du fichier csv les arbres de paris avec l'interface graphique) :
``./bin/main -i *les-arbres.csv* -o *monfichierbinaire* -v``

### Lancement des testes

Afin de lancer les testes, il suffit lancer **Make testes** a la source du projet. Afin d'avoir un meilleur visuelle, vous pouvez lancer la commande ``firefox  tests/rapport/index.html``.

### Génération de la documentation