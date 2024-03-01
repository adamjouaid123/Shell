#include "commands.h"

volatile char *global_command = "";
pid_t child_pid = -1;

void commandes_simple(struct cmdline *cmd, int option) {
    // Vérifie si la commande est "quit" ou "q" pour quitter le shell
    if (strcmp(cmd->seq[0][0], "quit") == 0 || strcmp(cmd->seq[0][0], "q") == 0 ) {
        printf(ANSI_COLOR_RED "Exiting shell...\n" ANSI_COLOR_RESET);
        kill(-getpid(), SIGTERM); // Envoie le signal SIGTERM à tous les processus du groupe
        exit(0); // Quitte le shell
    } 
    // Vérifie si la commande est "export" avec deux arguments valides pour ajouter une variable d'environnement
    else if (strcmp(cmd->seq[0][0], "export") == 0) {
        if (*(cmd->set_var) == 1 && cmd->seq[0][1] != NULL && cmd->seq[0][2] != NULL && cmd->seq[0][3] == NULL) {
            handle_add_envar(cmd->seq[0][1], cmd->seq[0][2]); // Ajoute la variable d'environnement
        } else {
            fprintf(stderr, "Erreur de syntaxe : %s\n", cmd->seq[0][0]);  // message d'erreur approprié
        }
    } 
    // Vérifie si la commande est "unset" avec un argument valide pour supprimer une variable d'environnement
    else if (strcmp(cmd->seq[0][0], "unset") == 0   ) {
        if (cmd->seq[0][1] != NULL && cmd->seq[0][2] == NULL) {
            handle_delete_envar(cmd->seq[0][1]); // Supprime la variable d'environnement
        }else {
            fprintf(stderr, "Erreur de syntaxe : %s\n", cmd->seq[0][0]); // message d'erreur approprié
        }
    } 
    // Vérifie si la commande est "jobs" pour afficher les processus en arrière-plan
    else if (strcmp(cmd->seq[0][0], "jobs") == 0 ) {
        jobs(); // Affiche les processus en arrière-plan
    } 
    // Vérifie si la commande est "fg" pour ramener un processus en arrière-plan en premier plan
    else if (strcmp(cmd->seq[0][0], "fg") == 0 ) {
        handle_fg(cmd->seq[0]); // Ramène un processus en arrière-plan en premier plan
    } 
    // Vérifie si la commande est "bg" pour exécuter un processus en arrière-plan
    else if (strcmp(cmd->seq[0][0], "bg") == 0 ) {
        handle_bg(cmd->seq[0]); // Exécute un processus en arrière-plan
    } 
    // Vérifie si la commande est "stop" pour arrêter un processus en arrière-plan
    else if (strcmp(cmd->seq[0][0], "stop") == 0 ) {
        handle_stop(cmd->seq[0]); // Arrête un processus en arrière-plan
    } 
    // Vérifie si aucun fichier d'entrée ou de sortie n'est spécifié
    else if (cmd->out == NULL && cmd->in == NULL) {
        handle_cmd_simple(cmd->seq[option], cmd->background); // Exécute la commande normalement
    } 
    // Si des redirections d'entrée ou de sortie sont spécifiées, les gère
    else if (cmd->out != NULL || cmd->in != NULL){
        handle_redirections(cmd->seq[option], cmd->in, cmd->out); 
    } else {
        fprintf(stderr, "%s: command introuvable \n", cmd->seq[0][0]);
    }
}



void commandes_double(struct cmdline *cmd) {
    if ( cmd->seq[0]!= NULL && cmd->seq[1]!= NULL ){
        handle_pipe(cmd); // Gère les commandes interconnectées par un pipe
    }
}
