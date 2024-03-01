#ifndef COMMAND_UTILS_H
#define COMMAND_UTILS_H

#include "commands.h"
#include <wordexp.h>

/**
 * Configuration des gestionnaires de signaux.
 */
void setup_signal_handlers();


/**
 * Ouvre un fichier pour la redirection.
 * 
 * @param filename Nom du fichier à ouvrir.
 * @param flags Options pour l'ouverture du fichier (par exemple, O_RDONLY, O_WRONLY | O_CREAT | O_TRUNC, etc.).
 * @return Le descripteur de fichier du fichier ouvert.
 *         En cas d'échec, la fonction retourne -1.
 */
int open_redirection_file(char *filename, int flags);


/**
 * Crée les tubes nécessaires pour exécuter les commandes en pipeline.
 * 
 * @param num_commands Le nombre de commandes dans le pipeline.
 * @param pipes Un pointeur vers le tableau de tubes à créer.
 */
void create_pipes(int num_commands, int ***pipes);


/**
 * Ferme tous les descripteurs de fichiers des tubes ouverts.
 * 
 * @param num_commands Le nombre de commandes dans le pipeline.
 * @param pipes Le tableau de tubes à fermer.
 */
void close_all_pipes(int num_commands, int **pipes) ;


/**
 * Exécute les commandes en pipeline.
 * 
 * @param cmd La structure cmdline contenant les commandes à exécuter.
 * @param num_commands Le nombre de commandes dans le pipeline.
 * @param pipes Le tableau de tubes pour la communication entre les commandes.
 */
void execute_commands(struct cmdline *cmd, int num_commands, int **pipes) ;


/**
 * Libère la mémoire allouée pour les tubes (pipes).
 * 
 * @param num_commands Le nombre de commandes dans le pipeline.
 * @param pipes Le tableau de tubes à libérer.
 */
void free_pipes(int num_commands, int **pipes) ;


/**
 * \brief Étend et remplace les variables d'environnement dans les arguments de la commande.
 * 
 * Cette fonction étend et remplace les variables d'environnement dans les arguments de la commande
 * spécifiés par `args`. Les arguments sont étendus en une seule chaîne de caractères, puis analysés
 * pour identifier les variables d'environnement à remplacer. Les variables d'environnement sont 
 * identifiées par le préfixe "$" et remplacées par leur valeur correspondante dans l'environnement.
 * 
 * @param args Un tableau de chaînes de caractères contenant les arguments de la commande.
 * @param p Un pointeur vers une structure wordexp_t où seront stockés les résultats de l'expansion.
 * @return 0 en cas de succès, 1 en cas d'erreur.
 */
int expand_and_replace_env(char **args, wordexp_t *p);


/**
 * \brief Exécute un processus enfant avec la commande spécifiée par les arguments.
 * 
 * Cette fonction configure les gestionnaires de signaux selon le mode d'exécution
 * spécifié par `background`. Ensuite, elle exécute la commande spécifiée par `args`
 * en utilisant la fonction execvp. Si l'exécution de la commande échoue, un message
 * d'erreur approprié est affiché sur la sortie d'erreur standard.
 * 
 * @param args Un tableau de chaînes de caractères contenant les arguments de la commande.
 *        Le premier argument (args[0]) est le nom de la commande.
 *        Les autres arguments (args[1], args[2], ...) sont les arguments de la commande.
 */
void execute_child_process(char **args);


/**
 * \brief Gère le processus enfant après son exécution. (le pere)
 * 
 * Cette fonction prend en charge le processus enfant après son exécution.
 * Si la commande n'est pas exécutée en arrière-plan, elle attend la terminaison
 * du processus enfant en utilisant waitpid. Si l'attente échoue, elle affiche un
 * message d'erreur approprié sur la sortie d'erreur standard.
 * Si la commande est exécutée en arrière-plan, elle ajoute le processus enfant à la
 * liste des jobs en utilisant la fonction add_job.
 * 
 * @param pid Le PID du processus enfant.
 * @param background Un pointeur vers un entier indiquant si la commande a été exécutée en arrière-plan.
 *        Si background est égal à 1, la commande a été exécutée en arrière-plan.
 * @param args Un tableau de chaînes de caractères contenant les arguments de la commande.
 *        Le premier argument (args[0]) est le nom de la commande.
 */
void handle_child_process(pid_t pid, int *background, char **args);


/**
 * \brief Expansion des variables d'environnement dans une chaîne de caractères.
 * 
 * Cette fonction prend une chaîne de caractères en entrée et remplace toute variable
 * d'environnement qu'elle contient par sa valeur correspondante. La chaîne de caractères
 * source est analysée et les variables d'environnement sont remplies par leurs valeurs.
 * Le résultat de l'expansion est stocké dans la variable destination.
 * 
 * @param source La chaîne de caractères à analyser et à étendre.
 * @param destination Un pointeur vers une chaîne de caractères qui recevra le résultat de l'expansion.
 *        La mémoire pour cette chaîne de caractères sera allouée dynamiquement à l'intérieur de la fonction.
 */
void expand_env(char *source, char **destination) ;

/**
 * \brief Création d'un chemin de destination étendu à partir du répertoire principal (home).
 * 
 * Cette fonction crée un chemin de destination étendu en concaténant le répertoire principal (home)
 * avec le reste du chemin spécifié. Le chemin de destination peut commencer par '~' pour représenter
 * le répertoire principal de l'utilisateur.
 * 
 * @param home Le répertoire principal de l'utilisateur.
 * @param destination Le reste du chemin à concaténer avec le répertoire principal.
 * 
 * @return Un pointeur vers une nouvelle chaîne de caractères contenant le chemin étendu.
 *         Cette mémoire doit être libérée par l'appelant après utilisation.
 *         En cas d'erreur d'allocation mémoire, la fonction renvoie NULL.
 */
char* create_expanded_path(char *home, char *destination) ;



/**
 * Effectue les redirections des descripteurs de fichier standard.
 *
 * 
 * @param input_fd Le descripteur de fichier vers lequel rediriger l'entrée standard.
 * @param output_fd Le descripteur de fichier vers lequel rediriger la sortie standard.
 */
void perform_redirections(int input_fd, int output_fd) ;


/**
 * Ferme les descripteurs de fichiers spécifiés.
 * 
 * @param input_fd Le descripteur de fichier d'entrée à fermer.
 * @param output_fd Le descripteur de fichier de sortie à fermer.
 */
void close_file_descriptors(int input_fd, int output_fd) ;


#endif 
