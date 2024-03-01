#ifndef COMMAND_HANDLERS_H
#define COMMAND_HANDLERS_H

#include "command_utils.h"
#include "error_handlers.h"


/**
 * \brief Change le répertoire de travail courant.
 * 
 * Cette fonction change le répertoire de travail courant en fonction
 * de l'argument fourni. Si aucun argument n'est fourni, le répertoire
 * de l'utilisateur (HOME) est utilisé. Si l'argument commence par '~',
 * il est interprété comme un chemin absolu à partir du répertoire de 
 * l'utilisateur.
 * 
 * \param args Un tableau de chaînes de caractères contenant les arguments.
 *        Le premier argument (args[0]) est le nom de la commande.
 *        Le deuxième argument (args[1]) est l'argument de la commande.
 */
void handle_cd(char **args);

/**
 * \brief Gère l'exécution d'une commande simple.
 * 
 * Cette fonction gère l'exécution d'une commande simple spécifiée par
 * les arguments. Si la commande est "cd", elle appelle la fonction 
 * handle_cd pour changer le répertoire de travail courant. Sinon, elle
 * exécute la commande en créant un processus enfant.
 * 
 * \param args Un tableau de chaînes de caractères contenant les arguments.
 *        Le premier argument (args[0]) est le nom de la commande.
 *        Les autres arguments (args[1], args[2], ...) sont les arguments de la commande.
 * \param background Un pointeur vers un entier indiquant si la commande doit être exécutée en arrière-plan.
 *        Si background est égal à 1, la commande est exécutée en arrière-plan.
 */
void handle_cmd_simple(char **args, int *background);


/**
 * \brief Gère les redirections d'entrée et de sortie pour l'exécution d'une commande.
 * 
 * Cette fonction gère les redirections d'entrée et de sortie pour l'exécution d'une commande.
 * Elle prend en charge les fichiers d'entrée et de sortie spécifiés et crée un processus fils
 * pour exécuter la commande avec les redirections appropriées.
 * 
 * \param args Un tableau de chaînes de caractères contenant les arguments de la commande.
 * \param input_file Le nom du fichier à utiliser comme entrée pour la commande.
 * \param output_file Le nom du fichier à utiliser comme sortie pour la commande.
 */
void handle_redirections(char **args, char *input_file, char *output_file);


/**
 * \brief Gère les commandes séparées par des pipes (|).
 * 
 * Cette fonction prend en charge l'exécution de commandes séparées par des pipes.
 * Elle compte d'abord le nombre de commandes dans la structure cmdline.
 * Ensuite, elle crée des tubes (pipes) pour chaque paire de commandes à exécuter.
 * Après avoir créé les pipes, elle exécute les commandes à l'aide des pipes pour rediriger
 * la sortie de chaque commande vers l'entrée de la suivante.
 * Enfin, elle libère la mémoire allouée pour les pipes.
 * 
 * @param cmd La structure cmdline contenant les commandes à exécuter.
 */
void handle_pipe(struct cmdline *cmd);




/**
 * Gère la commande "pwd" (print working directory) qu'on utilise pour le shell.
 * Cette fonction retourne le chemin absolu du répertoire de travail actuel.
 * 
 * @return Le chemin absolu du répertoire de travail actuel.
 */
char *handle_pwd() ;


/**
 * \brief Gère l'ajout d'une variable d'environnement.
 * Cette fonction ajoute une nouvelle variable d'environnement avec le nom et la valeur spécifiés.
 * 
 * @param name Le nom de la variable d'environnement à ajouter.
 * @param value La valeur associée à la variable d'environnement.
 */
void handle_add_envar(const char *name, const char *value);


/**
 * \brief Gère la suppression d'une variable d'environnement.
 * Cette fonction supprime la variable d'environnement spécifiée par son nom.
 * 
 * @param name Le nom de la variable d'environnement à supprimer.
 */
void handle_delete_envar(const char *name);



#endif 
