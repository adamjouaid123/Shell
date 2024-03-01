# Shell

## Description

Ce programme implémente un shell simple en langage C. Il fournit à l'utilisateur une interface pour saisir et exécuter des commandes système. Le shell prend en charge 
à la fois les commandes simples et les commandes avec des pipes pour permettre une manipulation avancée des flux de données.

En plus des fonctionnalités de base d'un shell, ce programme intègre la gestion des jobs. Les utilisateurs peuvent démarrer des processus en arrière-plan, les mettre 
en pause et les reprendre à tout moment, offrant ainsi une grande flexibilité dans la gestion des tâches système.

Le shell offre également une gestion complète des variables d'environnement. Les utilisateurs peuvent définir, afficher et modifier les variables d'environnement 
en fonction de leurs besoins, ce qui leur permet de personnaliser leur expérience d'utilisation du shell.

De plus, une fois que l'utilisateur quitte le shell, il prend en charge la fermeture de tous les processus fils qui ont été lancés pendant son exécution, garantissant ainsi une gestion propre des ressources système.

En résumé, les principales fonctionnalités de ce shell incluent :
- Exécution de commandes simples et de commandes avec des pipes.
- Gestion des jobs pour démarrer, mettre en pause et reprendre les processus.
- Manipulation avancée des variables d'environnement pour une personnalisation accrue.
- Fermeture propre de tous les processus fils une fois le shell quitté.

## Bugs Découverts

Parmi les bugs découverts, un problème non résolu concerne l'exécution de commandes en arrière-plan. Si une commande est lancée en arrière-plan sans espace entre le nom 
de la commande et le `&`, le shell peut rencontrer des erreurs.

Par exemple, exécuter une commande de cette manière peut entraîner des problèmes :
  - `ls&`

Le shell peut afficher un message d'erreur qui ne reflète pas correctement l'erreur réelle, tel que :
  - `Erreur lors de l'expansion de la commande` 

Pour éviter ce problème, assurez-vous d'ajouter un espace entre le nom de la commande et le `&` lors de l'exécution en arrière-plan.


Un autre bug identifié mais non corrigé concerne la redirection avec des pipes. Actuellement, lorsqu'une commande impliquant des pipes est utilisée avec une redirection 
de sortie, comme dans l'exemple suivant :
  - `ls | grep .txt > test.txt`

la redirection `<` ou `>` s'applique à la sortie de la première commande (`ls`), plutôt qu'au résultat filtré de la seconde commande (`grep .txt`). Cela peut entraîner 
des résultats inattendus lors de l'utilisation de redirections avec des pipes.


## Gestion des Erreurs

Une gestion efficace des erreurs est essentielle pour garantir un fonctionnement fiable du shell. En cas d'erreur, le shell fournit des messages d'erreur clairs pour 
aider les utilisateurs à diagnostiquer et à résoudre les problèmes rencontrés.

### Exemples de Messages d'Erreur

- **Commande Introuvable :**
    - a : commande introuvable

Conseil : Vérifiez l'orthographe de la commande et assurez-vous qu'elle est installée sur votre système.

- **Permissions Insuffisantes :**
    - permission refusée: fichier.txt

Conseil : Vérifiez les autorisations du fichier ou du répertoire et assurez-vous d'avoir les droits nécessaires pour effectuer l'opération.

- **Erreur d'Entrée/Sortie :**
    - Erreur lors de l'ouverture du fichier a : No such file or directory

Conseil : Vérifiez l'existence du fichier et assurez-vous qu'il est accessible en lecture ou en écriture.


## Fonctionnalités

- **Exécution de commandes simples :** Le shell peut exécuter des commandes système simples telles que 
  `ls`, `cat`, `mkdir`, etc.
- **Exécution de commandes avec des pipes :** Le shell peut exécuter des commandes avec des pipes, permettant 
  la redirection de la sortie standard d'une commande vers l'entrée d'une autre.
- **Ignorer les signaux SIGINT et SIGTSTP :** Le shell ignore les signaux SIGINT (Ctrl+C) et SIGTSTP (Ctrl+Z)
  pour empêcher l'interruption involontaire des processus en cours d'exécution.
- **Gestion des signaux SIGCHLD :** Le shell gère les signaux SIGCHLD pour traiter les processus fils terminés
  et nettoyer les ressources associées.
- **Gestion des processus en arrière-plan et en premier plan :** Le shell prend en charge l'exécution de commandes
  en arrière-plan et en premier plan, permettant à l'utilisateur de continuer à travailler dans le shell pendant
  que des commandes sont en cours d'exécution.
- **Affichage des processus en cours d'exécution :** Le shell fournit une fonctionnalité pour afficher les processus
  en cours d'exécution en arrière-plan à tout moment.
- **Mettre un job en premier plan avec `fg` :** Vous pouvez utiliser la commande `fg` suivie du numéro de job pour mettre un job en premier plan.
- **Mettre un job en arrière-plan avec `bg` :** La commande `bg` permet de mettre un job en arrière-plan en fournissant son numéro de job.
- **Arrêter un job avec `stop` :** Utilisez la commande `stop` suivie du numéro de job pour arrêter un job en cours.
- **Supprimer un job avec `remove_job` :** Cette fonctionnalité permet de supprimer un job en cours en spécifiant son numéro de job.
- **Gestion des signaux personnalisés :** Le shell implémente une gestion personnalisée des signaux SIGTSTP et SIGCHLD pour une meilleure expérience utilisateur.
- **Export et unset pour les variables d'environnement :** Les utilisateurs peuvent exporter de nouvelles variables d'environnement avec `export` et supprimer 
  des variables avec `unset`.
- **Redirections vers et depuis des fichiers :** Le shell prend en charge les redirections d'entrée et de sortie vers et depuis des fichiers à l'aide des opérateurs `<` 
  et `>`.
- **Gestion d'erreurs personnalisée :** Le shell fournit une gestion d'erreurs personnalisée avec des messages clairs et des suggestions pour aider les utilisateurs 
  à comprendre et à corriger les erreurs.
- **Sortie du shell :** Le shell peut uniquement être quitté en utilisant la commande `quit` ou `q`.


## Utilisation

1. **Compilation :** Pour compiler le programme, assurez-vous que le Makefile est placé dans le répertoire parent du dossier src, puis exécutez la commande suivante :
    make

2. **Exécution :** Exécutez le programme compilé.
    ./shell

3. **Utilisation du shell :**

Une fois le shell lancé, vous pouvez saisir des commandes système standard.

- Pour exécuter des commandes simples, entrez-les directement :
  - Exemple : `ls` - Affiche le contenu du répertoire actuel.
  - Exemple : `pwd` - Affiche le répertoire de travail actuel.
  - Exemple : `date` - Affiche la date et l'heure actuelles.

- Pour exécuter des commandes avec des pipes, utilisez la syntaxe des pipes (`|`) pour rediriger la sortie d'une commande vers l'entrée d'une autre :
  - Exemple : `ls -l | grep .txt` - Affiche uniquement les fichiers ayant l'extension .txt dans le répertoire actuel.

- Pour exécuter des commandes en arrière-plan, ajoutez `&` à la fin de la commande :
  - Exemple : `sleep 10 &` - Lance une commande sleep en arrière-plan pendant 10 secondes.

- Pour afficher les processus en cours d'exécution, utilisez la commande `jobs` :
  - Exemple : `jobs` - Affiche la liste des jobs en cours d'exécution.

- Pour mettre un job en premier plan, utilisez la commande `fg` suivie du numéro de job :
  - Exemple : `fg 1` - Met le job numéro 1 en premier plan.

- Pour mettre un job en arrière-plan, utilisez la commande `bg` suivie du numéro de job :
  - Exemple : `bg 2` - Met le job numéro 2 en arrière-plan.

- Pour arrêter un job en cours, utilisez la commande `stop` suivie du numéro de job :
  - Exemple : `stop 3` - Arrête le job numéro 3.

- Pour ajouter une variable d'environnement, utilisez la commande `export` suivie du nom de la variable et de sa valeur :
  - Exemple : `export VAR = VALEUR` - Crée une variable d'environnement nommée VAR avec la valeur "Valeur".

- Pour supprimer une variable d'environnement, utilisez la commande `unset` suivie du nom de la variable :
  - Exemple : `unset VAR` - Supprime la variable d'environnement nommée VAR.

- Pour quitter le shell, utilisez la commande `quit` ou `q`.
  - Exemple : `quit` - Quitte le shell et revient au terminal.


## Organisation du Projet 

Dans ce projet de shell, les fichiers sont organisés de manière à favoriser la modularité et la lisibilité du code. Chaque fichier correspond à une fonctionnalité spécifique du shell et interagit avec les autres pour assurer le bon fonctionnement de l'ensemble.

- **commands.c** : Gère l'exécution des commandes simples et celles avec des pipes.
  
- **command_handlers.c** : Gère les commandes spécifiques, y compris les redirections et les changements de répertoire.
  
- **command_utils.c** : Fournit des fonctions utilitaires pour la manipulation des commandes et des opérations système.
  
- **jobs.c** : Responsable de la gestion des processus en cours d'exécution et de leur état.
  
- **jobs_utils.c** : Fournit des fonctions pour la manipulation des jobs, y compris la mise à jour de leur statut et la gestion des processus associés.
  
- **signal_handlers.c** : Gère les signaux système, notamment SIGCHLD pour la gestion des processus enfants et SIGTSTP pour la suspension des processus en arrière-plan.
  
- **error_handler.c** : Gère les erreurs courantes rencontrées pendant l'exécution du shell.
  
- **shell.c** : Le fichier principal du shell qui orchestre l'ensemble du processus.


## Interactions entre les Fichiers

Les interactions entre les fichiers sont essentielles pour assurer le bon fonctionnement et la modularité du shell. Voici comment les fichiers interagissent dans notre programme :

- **commands.c :**
  - Contient les fonctions appelées dans le programme principal `shell.c`.
  - Utilise les fonctions de `command_handlers` et `jobs`.
  - Définit deux variables globales :
    - `global_command` : Utilisée pour ajouter la commande à la liste des jobs.
    - `child_pid` : Utilisée pour mettre un job en arrière-plan.

- **command_handlers.c :**
  - Contient les fonctions d'exécution des commandes.
  - Utilise des fonctions diverses de `command_utils` pour factoriser le code.
  - Utilise une fonction `execvp`  avec une gestion personnalisée des erreurs à partir du fichier `error_handlers`.

- **command_utils.c :**
  - Contient des fonctions utilitaires pour simplifier le code de `command_handlers`.

- **error_handlers.c :**
  - Contient les vérifications et l'envoi des messages d'erreurs pour différentes opérations système.

- **jobs.c :**
  - Contient les fonctions d'ajout, de suppression, de mise en pause, et d'arrière-plan des jobs.
  - Utilise des fonctions de `jobs_utils` pour des fonctions utilitaires.

- **signal_handlers.c :**
  - Contient la définition des gestionnaires de signaux `sigtstp_handler` et `sigchild_handler`.

- **shell.c :**
  - Programme principal.
  - Définit les variables globales :
    - `job_list` : Liste des jobs.
    - `num_jobs` : Nombre actuel de jobs dans la liste.
    - `job_index` : Numéro du prochain job à ajouter.
    - `shell_pid` : Utilisé pour bloquer les signaux SIGINT et SIGTSTP du shell principal.

Ce schéma d'interaction garantit une séparation claire des préoccupations et favorise la modularité du code, ce qui facilite la maintenance et l'extension du shell.


## Remarque sur les Fichiers Fournis

Pour faciliter le développement du projet, les fichiers `csapp.c` et `readcmd.c`, ainsi qu'une version simplifiée de `shell.c`, ont été fournis au début du projet. Ces fichiers ont été utilisés comme base pour la mise en œuvre du shell et contiennent des fonctionnalités essentielles.

- **csapp.c :** Contient des fonctions utilitaires pour gérer les opérations système de manière robuste et portable.
- **readcmd.c :** Fournit des fonctions pour lire et analyser les commandes utilisateur saisies dans le shell. Deux pointeurs ont été ajoutés pour enrichir 
    cette fonctionnalité :
    - Un pointeur entier pour le mode de background :
      - 0 indique un mode foreground.
      - 1 indique un mode background si le caractère '&' est détecté, sinon 0.
    - Un pointeur entier pour le mode set_var :
      - 0 pour pas de variable, indiquant une syntaxe incorrecte au cas de la commande export ( absence de '=' ).
      - 1 pour séparer la variable et la valeur, indiquant une commande correcte avec une variable et une valeur associée.
- **shell.c (version simplifiée) :** Une version de départ du shell qui a été développée progressivement pour implémenter les fonctionnalités requises du projet.

Ces fichiers ont servi de fondation pour le développement du shell et ont été intégrés dans la structure du projet pour assurer sa cohérence et sa qualité globale.
