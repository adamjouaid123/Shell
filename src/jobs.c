#include "jobs.h"


void jobs() {
    update_job_status(); // Met à jour le statut des jobs pour refléter les changements récents

    // Supprime les travaux terminés 
    remove_finished_jobs(); 

    // Parcours de la liste des jobs et affichage de chaque job avec son numéro, son statut et sa commande associée
    for (int i = 0; i < num_jobs; i++) {
        char status_symbol;
        const char* status_text = get_status_text(job_list[i].status); // Obtient le texte représentant l'état du job

        // Détermine le symbole à afficher en fonction du statut du job
        if (job_list[i].status == 0) {
            status_symbol = '-'; // Job done
        } else {
            status_symbol = '+'; // Job running ou stopped
        }

        // Affiche les détails du job avec son numéro, son symbole de statut, le texte de statut et la commande associée
        printf("[%d]%c  %s %s\n", 
               job_list[i].job_number, 
               status_symbol,
               status_text,
               job_list[i].command);
    }
}




void add_job(pid_t pid, char *command, int status_symbol) {
    // Vérifier si le PID existe déjà dans la liste des jobs
    for (int i = 0; i < num_jobs; i++) {
        if (job_list[i].pid == pid) {
            // Mettre à jour le statut du job existant et retourner
            job_list[i].status = status_symbol;
            return;
        }
    }
       
    if (num_jobs < MAX_JOBS) {
        job_list[num_jobs].job_number = job_index + 1; // Numéro du job
        job_list[num_jobs].pid = pid; // PID du processus
        strcpy(job_list[num_jobs].command, command); // Commande associée
        job_list[num_jobs].status = status_symbol; // Statut du job
        num_jobs++; // Incrémente le nombre total de jobs
        job_index++;  // pour savoir le numero du prochain
    } else {
        fprintf(stderr, "Impossible d'ajouter plus de travaux. Limite maximale atteinte.\n");
    }
}



void remove_job(int job_number) {
    int found = 0; // Variable pour indiquer si le job a été trouvé
    for (int i = 0; i < num_jobs; i++) {
        if (job_list[i].job_number == job_number) { // Vérifie si le numéro de job correspond
            found = 1; // Marque que le job a été trouvé
            for (int j = i; j < num_jobs - 1; j++) {
                job_list[j] = job_list[j + 1]; // Déplace les jobs suivants vers la gauche
            }
            num_jobs--; // Décrémente le nombre total de jobs
            break;
        }
    }
    if (!found) { // Si le job n'a pas été trouvé
        fprintf(stderr, "Le travail %d n'a pas été trouvé.\n", job_number);
    }
}




void handle_fg(char **args) {
    if (args[1] == NULL) { // Vérifie si un numéro de job est fourni
        fprintf(stderr, "Utilisation : fg <numéro du travail>\n"); // Affiche un message d'utilisation
        return;
    }

    int job_number = atoi(args[1]); // Convertit l'argument en entier

    if (job_number <= 0) { // Vérifie si le numéro de job est valide
        fprintf(stderr, "Numéro de travail invalide.\n"); // Affiche un message d'erreur
        return;
    }

    foreground(job_number); // Met le job en premier plan
}


void handle_bg(char **args) {
    if (args[1] == NULL) { // Vérifie si un numéro de job est fourni
        fprintf(stderr, "Utilisation : bg <numéro du travail>\n"); // Affiche un message d'utilisation
        return;
    }

    int job_number = atoi(args[1]); // Convertit l'argument en entier

    if (job_number <= 0) { // Vérifie si le numéro de job est valide
        fprintf(stderr, "Numéro de travail invalide.\n"); // Affiche un message d'erreur
        return;
    }

    background(job_number); // Met le job en arrière-plan
}



void handle_stop(char **args) {
    if (args[1] == NULL) { // Vérifie si un numéro de job est fourni
        fprintf(stderr, "Utilisation : stop <numéro du travail>\n"); // Affiche un message d'utilisation
        return;
    }

    int job_number = atoi(args[1]); // Convertit l'argument en entier

    if (job_number <= 0) { // Vérifie si le numéro de job est valide
        fprintf(stderr, "Numéro de travail invalide.\n"); // Affiche un message d'erreur
        return;
    }

    stop_job(job_number); // Arrête le job
}
