#include "error_handlers.h"


void handle_fork_error() {
    // Vérifie le code d'erreur errno et affiche un message approprié
    if (errno == EAGAIN) {
        // Erreur lorsque les ressources sont insuffisantes pour créer un nouveau processus
        fprintf(stderr, "Impossible de créer le processus : Ressources insuffisantes\n");
    } else if (errno == ENOMEM) {
        // Erreur lorsque la mémoire est insuffisante pour allouer des structures de processus
        fprintf(stderr, "Impossible de créer le processus : Mémoire insuffisante\n");
    } else {
        // Erreur générale avec le code errno, affiche le message d'erreur associé
        fprintf(stderr, "Impossible de créer le processus : %s\n", strerror(errno));
    }
}

void handle_chdir_error(const char *directory) {
    // Vérifie le code d'erreur errno et affiche un message approprié
    if (errno == EACCES) {
        // Erreur d'accès, l'utilisateur n'a pas les permissions nécessaires
        fprintf(stderr, "Accès refusé : %s\n", directory);
    } else if (errno == ENOENT) {
        // Répertoire non trouvé, le répertoire spécifié n'existe pas
        fprintf(stderr, "Répertoire non trouvé : %s\n", directory);
    } else {
        // Erreur générale lors du changement de répertoire, affiche le message d'erreur associé
        fprintf(stderr, "Erreur lors du changement de répertoire : %s\n", strerror(errno));
    }
}
void handle_waitpid_error() {
    // Vérifie le code d'erreur errno et affiche un message approprié
    if (errno == ECHILD) {
        // Aucun processus enfant à attendre
        fprintf(stderr, "Aucun processus enfant à attendre\n");
    } else if (errno == EINTR) {
        // waitpid interrompu par un signal
        fprintf(stderr, "waitpid interrompu par un signal\n");
    } else {
        // Erreur générale avec le code errno, affiche le message d'erreur associé
        fprintf(stderr, "waitpid a échoué : %s\n", strerror(errno));
    }
}

void handle_kill_error(int pid) {
    // Vérifie le code d'erreur errno et affiche un message approprié
    if (errno == ESRCH) {
        // Processus avec le PID spécifié non trouvé
        fprintf(stderr, "Processus avec le PID %d introuvable\n", pid);
    } else if (errno == EPERM) {
        // Accès refusé pour envoyer un signal au processus avec le PID spécifié
        fprintf(stderr, "Accès refusé pour envoyer un signal au processus avec le PID %d\n", pid);
    } else {
        // Erreur générale avec le code errno, affiche le message d'erreur associé
        fprintf(stderr, "Erreur lors de l'envoi du signal au processus avec le PID %d : %s\n", pid, strerror(errno));
    }
}

void execute_with_error_handling(char **args) {
    if (execvp(args[0], args) == -1) {
        // Gestion des erreurs d'exécution
        if (errno == ENOENT) {
            // Commande non trouvée
            fprintf(stderr, "%s: commande introuvable\n", args[0]);
        } else if (errno == EACCES) {
            // Permission refusée
            fprintf(stderr, "%s:  permission refusée\n", args[0]);
        } else {
            // Autres erreurs
            perror(args[0]);
        }
        // Sortie du processus enfant avec le code d'échec
        exit(EXIT_FAILURE);
    }
}