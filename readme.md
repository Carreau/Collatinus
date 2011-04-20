#Collatinus

###Présentation de Collatinus
Collatinus est un lemmatiseur de textes latins : il est capable, si on lui donne une forme déclinée ou conjuguée, de trouver quel mot il faudra chercher dans le dictionnaire pour avoir sa traduction dans une autre langue, ses différents sens, et toutes les autres données que fournit habituellement le dictionnaire.

Pour plus d'information, compilez  `collatinus.tex`

###Compilation de Collatinus

Si vous êtes intéressé dans le développement de Collatinus vous pouvez récupérer les sources à l'URL suivante   

    git://github.com/Carreau/Collatinus.git

ou en HTTP   

    https://github.com/Carreau/Collatinus.git

### Dépendences/Connaissances nécessaires

Collatinus utilise Qt, C++ et latin comme langage, mais on a toujours besoin de personne capable d'automatiser le processus de compilation pour certaines plateformes, vous êtes donc toujours le bienvenu pour *forker* le projet et y apporter les modifications nécessaires.

### Compilation sous Mac OS

Installez Qt à partir des source (à vérifier)

oneliner

```bash
qmake -spec macx-g++;  make deploy; open Collatinus.app
```

longer description

```bash
#générer le makefile
qmake -spec macx-g++

#compiler
make
    
#copier des ressources supplémentaire dans le bundle
make install_data
    
#si redistribution, il faut copier le Framwork qt localement dans le bundle de l'application 
make deploy
```
