#include "command_utils.h"

void setup_signal_handlers() {
    Signal(SIGINT, SIG_DFL);
    Signal(SIGTSTP, sigtstp_handler);
}


int open_redirection_file(char *filename, int flags) {
    // Ouvre le fichier avec les options spécifiées

    int fd = open(filename, flags, 0644);

    // Vérifie si l'ouverture du fichier a échoué
    if (fd == -1) {
        // Utilisation de errno pour déterminer la cause de l'échec
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s : %s\n", filename, strerror(errno));
        return -1;                                              // renvoie -1 indiquant un échec d'ouverture
    }
    return fd; // Retourne le descripteur de fichier du fichier ouvert
}



void create_pipes(int num_commands, int ***pipes) {
    // Alloue de la mémoire pour le tableau de tubes (pipes)
    *pipes = malloc((num_commands - 1 ) * sizeof(int *));
    if (*pipes == NULL) {
        // En cas d'échec d'allocation de mémoire, affiche un message d'erreur et quitte le programme
        fprintf(stderr, "Erreur d'allocation de memoire : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Boucle pour créer chaque tube (pipe) et l'associer à l'indice correspondant dans le tableau
    for (int i = 0; i < num_commands - 1; i++) {
        (*pipes)[i] = malloc(2 * sizeof(int)); // Alloue de la mémoire pour le tube (pipe)
        if ((*pipes)[i] == NULL) {
            // En cas d'échec d'allocation de mémoire, affiche un message d'erreur et quitte le programme
            fprintf(stderr, "Erreur d'allocation de memoire : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        if (pipe((*pipes)[i]) == -1) {
            // En cas d'échec de création du tube (pipe), affiche un message d'erreur et quitte le programme
            fprintf(stderr, "Erreur de creation de pipe : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}

void close_all_pipes(int num_commands, int **pipes) {
    // Parcourt tous les tubes (pipes) sauf le dernier
    for (int i = 0; i < num_commands - 1; i++) {
        // Ferme le descripteur de fichier de lecture du tube (pipe)
        Close(pipes[i][0]);
        // Ferme le descripteur de fichier d'écriture du tube (pipe)
        Close(pipes[i][1]);
    }
}

void execute_commands(struct cmdline *cmd, int num_commands, int **pipes) {
    // Alloue de la mémoire pour les PID des processus enfants
    pid_t *pids = malloc(num_commands * sizeof(pid_t));
    if (pids == NULL) {
        fprintf(stderr, "Erreur d'allocation de memoire : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    

    // Boucle pour créer et exécuter chaque commande dans un processus enfant
    for (int i = 0; i < num_commands; i++) {
        pids[i] = Fork();
        if (pids[i] == -1) {
            fprintf(stderr, "Erreur fork du processus : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        } else if (pids[i] == 0) {
            // Processus enfant
            if (i != 0) {
                // Redirection de l'entrée standard vers le tube (pipe) précédent
                if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1) {
                    fprintf(stderr, "Error de duplication du fd : %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
            }
            if (i != num_commands - 1) {
                // Redirection de la sortie standard vers le tube (pipe) actuel
                if (dup2(pipes[i][1], STDOUT_FILENO) == -1) {
                    fprintf(stderr, "Error de duplication du fd : %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
            }
            // Ferme tous les tubes (pipes) dans le processus enfant
            close_all_pipes(num_commands, pipes);
            // Exécute la commande correspondante
            commandes_simple(cmd, i);
            exit(EXIT_SUCCESS);
        }
    }

    // Ferme tous les tubes (pipes) dans le processus parent
    close_all_pipes(num_commands, pipes);

    //Attend que les processus enfants se terminent
    for (int i = 0; i < num_commands - 1; i++) {
        int status;
        if (pids[i] > 0) { // Vérifie s'il s'agit d'un processus enfant valide
            if (waitpid(pids[i], &status, 0) == -1) {
                // Appel de la fonction handle_waitpid_error pour gérer l'erreur
                handle_waitpid_error();
                exit(EXIT_FAILURE);
            }
        }
    }


    // Libère la mémoire allouée pour les PID des processus enfants
    free(pids);
}

void free_pipes(int num_commands, int **pipes) {
    // Parcourt tous les pipes sauf le dernier
    for (int i = 0; i < num_commands-1 ; i++) {
        // Libère la mémoire allouée pour chaque pipe
        free(pipes[i]);
    }
    // Libère la mémoire allouée pour le tableau de pipes
    free(pipes);
}


int expand_and_replace_env(char **args, wordexp_t *p) {
    // Déclaration d'une chaîne de caractères pour stocker la commande étendue
    char expanded_command[128] = ""; 

    // Concaténation des arguments pour former une commande étendue
    for (int i = 0; args[i] != NULL; i++) {
        strcat(expanded_command, args[i]);
        strcat(expanded_command, " ");
    }

    // Expansion de la commande étendue en utilisant wordexp
    int ret = wordexp(expanded_command, p, WRDE_NOCMD | WRDE_SHOWERR);
    if (ret != 0) {
        if (ret == WRDE_NOSPACE) {
            fprintf(stderr, "Erreur lors de l'allocation de mémoire pour l'expansion de la commande\n");
        } else {
            fprintf(stderr, "Erreur lors de l'expansion de la commande\n");
        }
        return 1; // Retourne 1 en cas d'erreur
    }

    // Parcours des mots résultants de l'expansion pour identifier et remplacer les variables d'environnement
    for (size_t i = 0; i < p->we_wordc; i++) {
        char *env_expansion = NULL;
        char *env_var = strchr(p->we_wordv[i], '$');
        if (env_var != NULL) {
            // Extrait du nom de la variable
            char *var_name = env_var + 1; // Skip '$'
            char *closing_brace = strchr(var_name, '}');
            if (closing_brace != NULL) {
                // Remplacement de la variable par sa valeur correspondante de l'environnement
                *closing_brace = '\0';
                char *env_value = getenv(var_name);
                if (env_value != NULL) {
                    env_expansion = strdup(env_value);
                }
                *closing_brace = '}';
            }
        }
        // Remplacement du mot par l'expansion de la variable d'environnement
        if (env_expansion != NULL) {
            free(p->we_wordv[i]); 
            p->we_wordv[i] = env_expansion; 
        }
    }

    return 0; // Retourne 0 en cas de succès
}





void execute_child_process(char **args) {
    // Configuration des gestionnaires de signaux en fonction du mode d'exécution (arrière-plan ou non)
    setup_signal_handlers();
    // Exécution de la commande spécifiée par les arguments en utilisant execvp
    execute_with_error_handling(args);
}


void handle_child_process(pid_t pid, int *background, char **args) {
    int status;
    if (!(*background)) {
        // Si la commande n'est pas exécutée en arrière-plan, attendre la terminaison du processus enfant
        if (waitpid(pid, &status, WUNTRACED) == -1) {
            // Gestion des erreurs lors de l'attente du processus enfant
            handle_waitpid_error();
            exit(EXIT_FAILURE);
        }
        // Réinitialiser la variable child_pid car le processus enfant a terminé
        child_pid = -1;
    } else {
        // Si la commande est exécutée en arrière-plan, ajouter le processus enfant à la liste des emplois
        add_job(pid, args[0], 1);
        
    }
}



void expand_env(char *source, char **destination) {
    wordexp_t p;
    int ret = wordexp(source, &p, 0);
    
    if (ret != 0) {
        // Gestion des erreurs lors de l'expansion des variables d'environnement
        fprintf(stderr, "Erreur lors de l'expansion de la commande : %s\n", strerror(errno));
        return;
    }

    // Copie du résultat de l'expansion dans la variable destination
    *destination = strdup(p.we_wordv[0]);
    
    // Libération de la mémoire allouée par wordexp
    wordfree(&p);
}


char* create_expanded_path(char *home, char *destination) {
    // Alloue dynamiquement de la mémoire pour le chemin étendu
    char *expanded_path = malloc(strlen(home) + strlen(destination) + 1);
    
    if (expanded_path == NULL) {
        // Gestion des erreurs d'allocation mémoire
        perror("malloc");
        return NULL;
    }

    // Copie le répertoire principal (home) dans le chemin étendu
    strcpy(expanded_path, home);
    // Concatène le reste du chemin (destination) à partir de la deuxième position (ignorer '~')
    strcat(expanded_path, destination + 1);
    
    // Retourne le chemin étendu
    return expanded_path;
}



void perform_redirections(int input_fd, int output_fd) {
    // Rediriger l'entrée standard (stdin) if input_fd is valid (-1 means it's not valid)
    if (input_fd != -1 && dup2(input_fd, STDIN_FILENO) == -1) {
        perror("dup2");
        exit(EXIT_FAILURE);
    }

    // Rediriger la sortie standard (stdout) if output_fd is valid (-1 means it's not valid)
    if (output_fd != -1 && dup2(output_fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        exit(EXIT_FAILURE);
    }

}

void close_file_descriptors(int input_fd, int output_fd) {
    if (input_fd != STDIN_FILENO) {
        Close(input_fd);
    }
    if (output_fd != STDOUT_FILENO) {
        Close(output_fd);
    }
}

