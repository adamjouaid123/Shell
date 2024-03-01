#include "command_handlers.h"


void handle_cd(char **args) {
    // Récupérer la destination à partir des arguments
    char *destination = args[1];
    // Si aucun argument n'est fourni, utiliser le répertoire HOME de l'utilisateur
    if (destination == NULL) {
        destination = getenv("HOME");
    } 
    // Si l'argument commence par '~', le traiter comme un chemin absolu
    else if (destination[0] == '~') {
        char *home = getenv("HOME");
        if (home != NULL) {
            // Créer le chemin absolu étendu
            char *expanded_path = create_expanded_path(home, destination);
            if (expanded_path == NULL) {
                return; // Gestion de l'erreur d'allocation mémoire
            }
            // Utiliser le chemin étendu comme destination
            destination = expanded_path;
        }
    }

    // Variable pour stocker la destination étendue
    char *expanded_destination = NULL;
    // Étendre les variables d'environnement dans la destination
    expand_env(destination, &expanded_destination);

    // Changer le répertoire de travail courant
    if (chdir(expanded_destination) != 0) {
        // Gestion des erreurs de changement de répertoire
        handle_chdir_error(expanded_destination);
    }

    // Libérer la mémoire allouée pour la destination étendue
    free(expanded_destination);
}


void handle_cmd_simple(char **args, int *background) {
    // Vérifier si la commande est "cd"
    if (strcmp(args[0], "cd") == 0) {
        handle_cd(args); // Appeler la fonction handle_cd pour changer le répertoire de travail
        return;
    }

    pid_t pid;
    wordexp_t p;

    global_command = args[0]; // Définir la commande globale pour le processus

    int err = expand_and_replace_env(args, &p); // Étendre les variables d'environnement dans les arguments
    if (err != 0) {
        return;
    }
    // Créer un nouveau processus
    pid = Fork();
    if (pid == 0) { // Code du processus enfant
        execute_child_process(p.we_wordv); // Exécuter la commande dans le processus enfant
    } else if (pid < 0) { // Gestion des erreurs de création de processus
        // Affichage d'une erreur spécifique en fonction de errno
        handle_fork_error();
    } else { // Code du processus parent
        child_pid = pid; // Stocker l'ID du processus enfant
        handle_child_process(pid, background, args); // Gérer le processus enfant
    }

    wordfree(&p); // Libérer la mémoire allouée pour les mots expansés
}

 

/* Bug : Marche avec les tildes et * mais pas avec les pipes */
void handle_redirections(char **args, char *input_file, char *output_file) {
    pid_t pid; // Identifiant du processus
    int status; // Statut du processus enfant
    int input_fd = STDIN_FILENO; // Descripteur de fichier pour l'entrée standard
    int output_fd = STDOUT_FILENO; // Descripteur de fichier pour la sortie standard
    wordexp_t p;

    // Étendre les wildcards et remplacer les variables d'environnement dans les arguments
    int err = expand_and_replace_env(args, &p); // Étendre les variables d'environnement dans les arguments
    if (err != 0) {
        return;
    }
    // Vérifier s'il y a un fichier d'entrée spécifié pour la redirection
    if (input_file != NULL) {
        input_fd = open_redirection_file(input_file, O_RDONLY); // Ouvrir le fichier en lecture seule
    }

    // Vérifier s'il y a un fichier de sortie spécifié pour la redirection
    if (output_file != NULL) {
        // Ouvrir le fichier en écriture, le créer s'il n'existe pas, le tronquer s'il existe
        output_fd = open_redirection_file(output_file, O_WRONLY | O_CREAT | O_TRUNC);
    }

    if (output_fd == -1 || input_fd == -1) {
        return;
    }

    // Créer un nouveau processus
    pid = Fork();
    if (pid < 0) { // Erreur lors de la création du processus
        handle_fork_error();// Afficher un message d'erreur
        exit(EXIT_FAILURE); // Terminer le processus avec un code d'échec
    } else if (pid == 0) { // Code exécuté par le processus enfant
        perform_redirections(input_fd, output_fd); // Configurer les redirections
        execute_with_error_handling(p.we_wordv); // Exécuter la commande avec les arguments fournis
    } else { // Code exécuté par le processus parent
        // Attendre que le processus enfant se termine et récupérer son statut
        if (waitpid(pid, &status, 0) == -1) {
            handle_waitpid_error();
            exit(EXIT_FAILURE); // Terminer le processus avec un code d'échec
        }
        close_file_descriptors(input_fd, output_fd); // Fermer les descripteurs de fichier
    }

    wordfree(&p); // Libérer la mémoire allouée par wordexp
}




void handle_pipe(struct cmdline *cmd) {
    int num_commands = 0;
    int **pipes;

    // Compter le nombre de commandes
    while (cmd->seq[num_commands] != NULL) {
        num_commands++;
    }

    // Créer des pipes
    create_pipes(num_commands, &pipes);

    // Exécuter les commandes
    execute_commands(cmd, num_commands, pipes);

    // Libérer la mémoire
    free_pipes(num_commands, pipes);
}


char *handle_pwd() {
    static char cwd[100]; // Variable statique pour stocker le chemin du répertoire de travail
    
    // Obtient le chemin absolu du répertoire de travail actuel
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        return cwd; // Retourne le chemin absolu du répertoire de travail
    } else {
        fprintf(stderr, "Erreur getwcd : %s\n", strerror(errno)); // Affiche un message d'erreur en cas d'échec de la récupération du répertoire de travail
        exit(EXIT_FAILURE); // Quitte le programme avec un code d'erreur
    }
}

void handle_add_envar(const char *name, const char *value) {
    // Ajoute la variable d'environnement avec le nom et la valeur spécifiés
    if (setenv(name, value, 1) != 0) {
        fprintf(stderr, "Erreur setenv : %s\n", strerror(errno)); // Affiche un message d'erreur en cas d'échec de l'ajout de la variable d'environnement
    } 
}

void handle_delete_envar(const char *name) {
    // Supprime la variable d'environnement spécifiée par son nom
    if (unsetenv(name) != 0) {
        fprintf(stderr, "Erreur unsetenv : %s\n", strerror(errno));// Affiche un message d'erreur en cas d'échec de la suppression de la variable d'environnement
    }
}