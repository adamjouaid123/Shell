#ifndef JOBS_UTILS_H
#define JOBS_UTILS_H

#include "csapp.h"
#include "signal_handlers.h"

/**
 * Met à jour le statut des jobs en invoquant le gestionnaire de signaux SIGCHLD.
 * Cette fonction est utilisée pour s'assurer que le statut des jobs est correctement mis à jour
 * avant d'afficher la liste des jobs en cours d'exécution.
 */
void update_job_status();

/**
 * Retourne le texte représentant l'état d'un job.
 */
const char* get_status_text(int status);

/**
 * Supprime les travaux terminés
 */
void remove_finished_jobs();


/**
 * \brief Met un travail en premier plan.
 * 
 * Cette fonction met le travail correspondant au numéro de travail spécifié
 * en premier plan, ce qui signifie qu'il reprend l'exécution et que le shell
 * attend jusqu'à ce que le travail se termine ou soit arrêté.
 * 
 * @param job_number Le numéro du travail à mettre en premier plan.
 */

void foreground(int job_number);


/**
 * \brief Met un travail arrêté en arrière-plan en cours d'exécution.
 * 
 * Cette fonction recherche le travail correspondant au numéro de travail spécifié
 * et le met en arrière-plan en cours d'exécution, ce qui signifie qu'il reprend l'exécution
 * s'il est arrêté et que le shell met à jour son statut en "Running".
 * 
 * @param job_number Le numéro du travail à reprendre en arrière-plan.
 */

void background(int job_number);


/**
 * \brief Met un travail en cours d'exécution en pause.
 * 
 * Cette fonction recherche le travail correspondant au numéro de travail spécifié
 * et le met en pause s'il est en cours d'exécution, en envoyant un signal SIGSTOP au processus.
 * Le statut du travail est mis à jour en "Stopped".
 * 
 * @param job_number Le numéro du travail à mettre en pause.
 */

void stop_job(int job_number);

#endif 
