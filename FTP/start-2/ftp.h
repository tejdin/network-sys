#ifndef __FTP_H__
#define __FTP_H__

#include <stdbool.h>

#define BUFFER_SIZE 4060
#define DEFAULT_SERVER "iin-tp-s32.iutrs.unistra.fr"
#define SERVER_PORT "21"

/*
 * Ouvre une connexion TCP vers un serveur et retourne le socket créé
 *  - server : nom de la machine cible
 *  - port : port sur lequel se connecter
 */
int open_connection(char *server, char *port);

/*
 * Envoie une commande FTP vers le serveur sur la connexion de contrôle
 *  - socket_control : socket de la connexion de contrôle
 *  - ftp_command : commande à envoyer au serveur FTP (PORT, USER, PASSWD, etc.)
 */
void send_ftp_command(int socket_control, char *ftp_command);

/*
 * Récupère une réponse sur une connexion, et retourne le code de la réponse
 *  - socket : socket sur lequel lire une réponse
 *  - reply : buffer dans lequel écrire la réponse lue
 */
int get_ftp_command_reply(int socket, char *reply);

/*
 * Demande et retourne le port sur lequel se connecter pour la connexion de data
 *  - socket_control : socket de la connexion de contrôle
 */
int get_passive_port(int socket_control);

/*
 * Récupère un fichier sur une connexion
 *  - socket : socket sur lequel lire des données
 *  - buffer : buffer de caractères
 *  - file : si ce booléen vaut vrai, le buffer contient le nom du fichier dans lequel écrire les données obtenues
 *           sinon, c'est le buffer dans lequel enregistrer les octets reçus sous la forme d'une chaîne
 */
void get_data(int socket, char *buffer, bool file);

#endif
