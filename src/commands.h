#ifndef __COMMANDS_H__ 
#define __COMMANDS_H__

#include "readcmd.h"
#include "jobs.h"

#define STDIN_FILENO 0
#define STDOUT_FILENO 1

#define ANSI_COLOR_RED     "\x1b[31m" 
#define ANSI_COLOR_RESET   "\x1b[0m" 

extern pid_t shell_pid;
extern int num_jobs;
extern volatile char *global_command ;
extern pid_t child_pid;


/**
 * Gère l'exécution de commandes simples.
 * 
 * @param cmd La structure cmdline contenant les commandes à exécuter.
 * @param option L'indice de la commande à exécuter dans la structure cmdline.
 */
void commandes_simple(struct cmdline *cmd,int option);



/**
 * Gère l'exécution de commandes avec des pipes.
 *
 * @param cmd La structure cmdline contenant les commandes à exécuter.
 */
void commandes_double(struct cmdline *cmd);

#endif