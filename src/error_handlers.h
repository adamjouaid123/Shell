#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"

/**
 * Gère les erreurs lors de la création d'un processus avec fork().
 * Affiche un message d'erreur approprié en fonction de errno.
 */
void handle_fork_error();


/**
 * Gère les erreurs lors du changement de répertoire avec chdir().
 * Affiche un message d'erreur approprié en fonction de errno.
 *
 * @param directory Le chemin du répertoire qui a provoqué l'erreur.
 */
void handle_chdir_error(const char *directory);


/**
 * Gère les erreurs lors de l'appel à waitpid() pour attendre un processus enfant.
 * Affiche un message d'erreur approprié en fonction de errno.
 */
void handle_waitpid_error();


/**
 * Gère les erreurs lors de l'envoi d'un signal à un processus avec kill().
 * Affiche un message d'erreur approprié en fonction de errno.
 *
 * @param pid Le PID du processus auquel le signal a été envoyé.
 */
void handle_kill_error(int pid);


/**
 * Exécute une commande avec gestion des erreurs.
 * 
 * \param args Un tableau de chaînes de caractères représentant la commande à exécuter.
 *        Le premier élément de args (args[0]) doit être le chemin de la commande à exécuter,
 *        et les éléments suivants (args[1], args[2], ...) sont les arguments de la commande.
 */
void execute_with_error_handling(char **args);

#endif /* ERROR_HANDLER_H */
