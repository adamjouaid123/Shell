#ifndef SIGNAL_HANDLERS_H
#define SIGNAL_HANDLERS_H

#include "csapp.h"
#include "command_handlers.h"

/**
 * Gère le signal SIGTSTP.
 * Cette fonction est appelée lors de la réception du signal SIGTSTP par le shell.
 * Elle place le processus en premier plan (s'il existe) en arrière-plan et le met en pause.
 * 
 * @param signum Le numéro du signal.
 */
void sigtstp_handler(int signum);

/**
 * Gère le signal SIGCHLD.
 * Cette fonction est appelée lorsqu'un changement d'état d'un processus fils survient, tel que terminé ou en pause.
 * Elle met à jour le statut de la tâche concernée dans la liste des tâches.
 * 
 * @param signum Le numéro du signal.
 */
void sigchld_handler(int signum);

#endif 
