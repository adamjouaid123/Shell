/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include "commands.h"

#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"



struct Job job_list[MAX_JOBS];
int num_jobs = 0;
pid_t shell_pid = 0;
int job_index = 0;



int main() {
    // Gestion des signaux SIGINT, SIGTSTP, SIGCHLD
    if (Signal(SIGINT, SIG_IGN) == SIG_ERR || 
        Signal(SIGTSTP, sigtstp_handler) == SIG_ERR ||
        Signal(SIGCHLD, sigchld_handler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }


    while (1) {
        // Attribution d'un nouveau groupe de processus
        if (setpgid(0, 0) == -1) {
            perror("setpgid");
            exit(EXIT_FAILURE);
        }
        
        // Récupération du PID du shell
        shell_pid = getpid();

        // Affichage du répertoire courant
        char *current_dir = handle_pwd();
        printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET " > ", current_dir);

        // Lecture de la commande
        struct cmdline *l = readcmd();

        if (!l) {
            printf("exit\n");
            exit(0);
        }

        // Exécution de la commande simple ou avec des pipes
        if (l != NULL && l->seq[0] != NULL && l->seq[1] != NULL && l->err == NULL) {
            commandes_double(l);
        } else if (l != NULL && l->err == NULL && l->seq[0] != NULL && l->seq[1] == NULL) {
            commandes_simple(l, 0);
        }


        // Libération de la mémoire allouée pour la commande
       // free(l);
    }

    return 0;
}



