#include "ftp.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>


/*
 * Ouvre une connexion TCP vers un serveur et retourne le socket créé
 *  - server : nom de la machine cible
 *  - port : port sur lequel se connecter
 */
int open_connection(char *server, char *port)
{
    printf("# Open connection on server %s on port %s\n", server, port);
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // AF_INET means IPv4 only addresses
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    int status = getaddrinfo(server, port, &hints, &res);
    if (status != 0)
    {
        perror("getaddrinfo");
        return -1;
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (sockfd == -1)
    {
        perror("socket");
        return -1;
    }


    status = connect(sockfd, res->ai_addr, res->ai_addrlen);

    if (status == -1)
    {
        perror("connect");
    }
    return sockfd;
}

/*
 * Envoie une commande FTP vers le serveur sur la connexion de contrôle
 *  - socket_control : socket de la connexion de contrôle
 *  - ftp_command : commande à envoyer au serveur FTP (PORT, USER, PASSWD, etc.)
 */
void send_ftp_command(int socket_control, char *ftp_command)
{
  send(socket_control, ftp_command, strlen(ftp_command), 0);


}

/*
 * Récupère une réponse sur une connexion
 *  - socket : socket sur lequel lire une réponse
 *  - reply : buffer dans lequel écrire la réponse lue
 */
int get_ftp_command_reply(int socket, char *reply) {
  //vider reply
  memset(reply, 0, BUFFER_SIZE);
    char buffer[BUFFER_SIZE];
    int total_bytes_read = 0;
    char status_code[4] = {0}; // Pour stocker le code de statut

    reply[0] = '\0';

    while (1) {
        int bytes_read = recv(socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_read <= 0) {
            break;
        }

        buffer[bytes_read] = '\0';
        strcat(reply, buffer);
        total_bytes_read += bytes_read;

        if (status_code[0] == '\0' && total_bytes_read >= 3) {
            strncpy(status_code, reply, 3);
            status_code[3] = '\0';
        }
        if (strstr(buffer, "\r\n") != NULL &&
          strncmp(buffer, status_code, 3) == 0 && 
            buffer[3] == ' ') {
            break;
        }
    }
    status_code[3] = '\0';  
    int code = atoi(status_code);
    return code;
}


/*
 * Demande et retourne le port sur lequel se connecter pour la connexion de data
 *  - socket_control : socket de la connexion de contrôle
 */

// Assurez-vous que les fonctions send_ftp_command et get_ftp_command_reply sont définies correctement

int get_passive_port(int socket_control) {
    int port = 0;
    char ftp_command[1024];
    char ftp_reply[BUFFER_SIZE];
    // Envoyer la commande PASV
    sprintf(ftp_command, "PASV\r\n");
    send_ftp_command(socket_control, ftp_command);

    // Recevoir la réponse du serveur
    get_ftp_command_reply(socket_control, ftp_reply);

    // Chercher la réponse "227"
    if (strstr(ftp_reply, "227") != NULL) {
        char* debut= strstr(ftp_reply, "(");
        char* fin= strstr(ftp_reply, ")");
        char* ip= malloc(sizeof(char)*20);
        strncpy(ip, debut+1, fin-debut-1);
        printf("%s\n", ip); 
        strtok(ip, ",");
        strtok(NULL, ",");
        strtok(NULL, ",");
        strtok(NULL, ",");
        char* port1= strtok(NULL, ",");
        char* port2= strtok(NULL, ",");
        port= atoi(port1)*256+atoi(port2);
        printf("%d\n", port); 
    }

    return port;
}
 

/*
 * Récupère un fichier sur une connexion
 *  - socket : socket sur lequel lire des données
 *  - buffer : buffer de caractères
 *  - file : si ce booléen vaut vrai, le buffer contient le nom du fichier dans lequel écrire les données obtenues
 *           sinon, c'est le buffer dans lequel enregistrer les octets reçus sous la forme d'une chaîne
 */
void get_data(int socket, char *buffer, bool file)
{
  
  char data[BUFFER_SIZE];
  int bytes_read = 0;

  if(!file){
    while ((bytes_read=recv(socket, data, BUFFER_SIZE, 0)) > 0) {
      data[bytes_read] = '\0';
      strcat(buffer, data);
    }    
  }
  else{
    int fd = open(buffer, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    while ((bytes_read=recv(socket, data, BUFFER_SIZE, 0)) > 0) {
      write(fd, data, bytes_read);
    }
    close(fd);
  }

  printf("Data: %s\n", buffer);

}
