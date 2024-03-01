#include "signal_handlers.h"

void sigtstp_handler(int signum) {
    // Obtenir la commande globale et le PID du processus en premier plan
    char* command = (char*)global_command;
    pid_t foreground_pid = child_pid;

    // Si le shell est en premier plan, s'il n'y a pas de processus en premier plan ou si la commande est vide, retourner
    if (shell_pid == foreground_pid || foreground_pid == -1 || strcmp(command, "") == 0) {
        return;
    }

    // Ajouter le processus du premier plan à la liste des tâches et le mettre en pause en arriere plan
    if (foreground_pid != -1) {
        add_job(foreground_pid, command, 2);
        kill(foreground_pid, SIGSTOP);
    }

    // Réinitialiser les variables child_pid et global_commande
    child_pid = -1;
    global_command = "";
    return;
}


void sigchld_handler(int signum) {
    int status;
    pid_t pid;

    // Parcourir tous les processus fils
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
        // Mettre à jour le statut de la tâche concernée
        for (int i = 0; i < num_jobs; i++) {
            if (job_list[i].pid == pid) {
                if (WIFEXITED(status)) {
                    // Si le processus s'est terminé normalement, marquer la tâche comme terminée
                    job_list[i].status = 0;
                } else if (WIFSIGNALED(status)) {
                    job_list[i].status = -1;
                } else if (WIFSTOPPED(status)) {
                    // Si le processus a été mis en pause, marquer la tâche comme mise en pause
                    job_list[i].status = 2; 
                } else if (WIFCONTINUED(status)) {
                    // Si le processus a repris après avoir été mis en pause, marquer la tâche comme en cours d'exécution
                    job_list[i].status = 1; 
                } else {
                    // TODO : Autres cas?
                }
                break; 
            }
        }
    }
}