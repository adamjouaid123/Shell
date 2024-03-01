#include "jobs_utils.h"


void update_job_status() {
    sigchld_handler(SIGCHLD); // Appel du gestionnaire de signaux SIGCHLD pour mettre à jour le statut des jobs
}



const char* get_status_text(int status) {
    switch (status) {
        case 0:
            return "Done";
        case 1:
            return "Running";
        case 2:
            return "Stopped";
        default:
            return "Unknown";
    }
}


void remove_finished_jobs() {
    for (int i = 0; i < num_jobs; i++) {
        if (job_list[i].status == 0) {
            const char *status = get_status_text(job_list[i].status);
            // un message que le job a etait supprimer car il est fini (done)
            printf("Travail [%d] supprimé : %s : %s \n", job_list[i].job_number, job_list[i].command, status);
            remove_job(job_list[i].job_number); // Retirer le travail si terminé 
        } else if (job_list[i].status == -1){
            // Retirer le travail si suspendu (CTRL-C) sans message
            remove_job(job_list[i].job_number);
        }
    }
}



void foreground(int job_number) {
    int found = 0; // Variable pour indiquer si le job a été trouvé
    for (int i = 0; i < num_jobs; i++) {
        if (job_list[i].job_number == job_number) { // Vérifie si le numéro de job correspond
            found = 1; // Marque que le job a été trouvé
            pid_t pid = job_list[i].pid; // Récupère le PID du job
            global_command = job_list[i].command; // Définit la commande globale en cours d'exécution
            child_pid = pid; // Définit le PID du processus enfant actif
            if (kill(pid, SIGCONT) < 0) { // Envoie un signal SIGCONT pour reprendre le processus
                handle_kill_error(pid);
            }
            printf("%s\n", global_command);
            int status;
            do {
                waitpid(pid, &status, WUNTRACED); // Attend que le processus se termine ou soit arrêté
            } while (!WIFEXITED(status) && !WIFSIGNALED(status) && !WIFSTOPPED(status));
            
            // Update job status
            if (WIFEXITED(status)) {
                job_list[i].status = 0;   // c'est terminé normalement
            } else if (WIFSIGNALED(status)) {
                job_list[i].status = -1;  // terminé avec SIGINT (CTRL-C) ou autres signals comme SIGTERM ou SIGKILL
            }            
            break;
        }
    }
    if (!found) { // Si le job n'a pas été trouvé
        fprintf(stderr, "Le travail %d n'a pas été trouvé.\n", job_number);
    }
}




void background(int job_number) {
    int found = 0; // Variable pour indiquer si le job a été trouvé
    for (int i = 0; i < num_jobs; i++) {
        if (job_list[i].job_number == job_number) { // Vérifie si le numéro de job correspond
            found = 1; // Marque que le job a été trouvé
            pid_t pid = job_list[i].pid; // Récupère le PID du job
            if (job_list[i].status != 2) { // Vérifie si le job est arrêté
                fprintf(stderr,"Le travail %d n'est pas arrêté.\n", job_number); // Affiche un message si le travail n'est pas arrêté
                return;
            }
            if (kill(pid, SIGCONT) < 0) { // Envoie un signal SIGCONT pour reprendre le processus
                handle_kill_error(pid);
            }
            job_list[i].status = 1; // Met à jour le statut du job à "Running"
            printf("Le travail %d a été repris : %s\n", job_number, job_list[i].command); // Affiche un message indiquant que le travail a été repris
            break;
        }
    }
    if (!found) { // Si le job n'a pas été trouvé
        fprintf(stderr, "Le travail %d n'a pas été trouvé.\n", job_number);
    }
}




void stop_job(int job_number) {
    int found = 0; // Variable pour indiquer si le job a été trouvé
    for (int i = 0; i < num_jobs; i++) {
        if (job_list[i].job_number == job_number) { // Vérifie si le numéro de job correspond
            found = 1; // Marque que le job a été trouvé
            pid_t pid = job_list[i].pid; // Récupère le PID du job
            if (job_list[i].status == 1) { // Vérifie si le job est en cours d'exécution
                if (kill(pid, SIGSTOP) < 0) { // Envoie un signal SIGSTOP pour mettre en pause le processus
                    handle_kill_error(pid);
                } else {
                    printf("Le travail %d a été arrêté.\n", job_number); // Affiche un message indiquant que le travail a été arrêté
                    job_list[i].status = 2; // Met à jour le statut du job à "Stopped"
                }
            } else {
                fprintf(stderr, "Le travail %d n'est pas en cours d'exécution.\n", job_number); // Affiche un message si le travail n'est pas en cours d'exécution
            }
            break;
        }
    }
    if (!found) { // Si le job n'a pas été trouvé
        fprintf(stderr, "Le travail %d n'a pas été trouvé.\n", job_number);
    }
}