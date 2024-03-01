#ifndef __JOBS_H__ 
#define __JOBS_H__

#include "jobs_utils.h"
#include "csapp.h"

#define MAX_JOBS 10

struct Job {
    int job_number;
    pid_t pid;
    char command[100];
    int status;
};

extern struct Job job_list[MAX_JOBS];
extern int num_jobs;
extern int job_index;



/**
 * \brief Affiche la liste des travaux en cours.
 * 
 * Cette fonction met à jour le statut des travaux pour refléter les changements récents,
 * supprime les travaux terminés, puis parcourt la liste des travaux et affiche chaque
 * travail avec son numéro, son statut et sa commande associée.
 */
void jobs();


/**
 * \brief Ajoute un nouveau travail à la liste des travaux en cours.
 * 
 * Cette fonction ajoute un nouveau travail à la liste des travaux en cours en spécifiant
 * le PID du processus, la commande associée et le statut du travail. Si le PID existe déjà
 * dans la liste des travaux, le statut du travail existant est mis à jour.
 * 
 * @param pid Le PID du processus associé au travail.
 * @param command La commande associée au travail.
 * @param status_symbol Le symbole représentant le statut du travail.
 */
void add_job(pid_t pid, char *command, int status_symbol);



/**
 * \brief Supprime un travail de la liste des travaux en cours.
 * 
 * Cette fonction recherche le travail correspondant au numéro de travail spécifié
 * et le supprime de la liste des travaux en cours. Les travaux suivants sont déplacés
 * vers la gauche pour combler l'espace libéré. Le nombre total de travaux est décrémenté.
 * 
 * @param job_number Le numéro du travail à supprimer.
 */
void remove_job(int job_number);


/**
 * \brief Met un travail en premier plan.
 * 
 * Cette fonction met en premier plan le travail spécifié par son numéro. Si aucun numéro de travail n'est fourni
 * en argument, elle affiche un message d'utilisation. Si le numéro de travail fourni n'est pas valide, elle affiche
 * un message d'erreur.
 * 
 * @param args Un tableau de chaînes de caractères contenant les arguments de la commande.
 */
void handle_fg(char **args);


/**
 * \brief Met un travail en arrière-plan.
 * 
 * Cette fonction met en arrière-plan le travail spécifié par son numéro. Si aucun numéro de travail n'est fourni
 * en argument, elle affiche un message d'utilisation. Si le numéro de travail fourni n'est pas valide, elle affiche
 * un message d'erreur.
 * 
 * @param args Un tableau de chaînes de caractères contenant les arguments de la commande.
 */
void handle_bg(char **args);


/**
 * \brief Arrête un travail en cours d'exécution.
 * 
 * Cette fonction arrête le travail spécifié par son numéro s'il est en cours d'exécution.
 * Si aucun numéro de travail n'est fourni en argument, elle affiche un message d'utilisation.
 * Si le numéro de travail fourni n'est pas valide, elle affiche un message d'erreur.
 * 
 * @param args Un tableau de chaînes de caractères contenant les arguments de la commande.
 */
void handle_stop(char **args);

#endif
