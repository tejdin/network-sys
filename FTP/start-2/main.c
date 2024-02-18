#include "ftp.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void launch_user_command(char *cli_input);

int main(int argc, char **argv)
{
    char cmd[1024];
    while (1)
    {
        printf("ftp> ");
        fgets(cmd, 1024, stdin);
        printf("\n");
        launch_user_command(cmd);
    }
    return 0;
}

/*
 * Fonction de lancement d'une commande entrée par l'utilisateur
 *  - cli_input : chaine de caractères lue dans la ligne de commande
 */
void launch_user_command(char *cli_input)
{
    // deux variables static qui garderont donc leur valeur
    // au fil des appels successifs à launch_user_command
    static int socket_control = 0;
    static char server[1024];

    char cli_command[1024] = "";
    char cli_params[1024] = "";
    sscanf(cli_input, "%s %s", cli_command, cli_params);

    char ftp_command[1024];
    char ftp_reply[BUFFER_SIZE];
    memset(ftp_reply, 0, BUFFER_SIZE);

    // OPEN
    if (!strcmp(cli_command, "open") || !strcmp(cli_command, "o"))
    {
        if (strlen(cli_params) < 1)
            strcpy(cli_params, DEFAULT_SERVER);
        socket_control = open_connection(cli_params, SERVER_PORT);
        get_ftp_command_reply(socket_control, ftp_reply);
        printf("\033[31;01m%s\033[00m\n", ftp_reply);
        // on sauve le nom du serveur utilisé pour utilisation future dans
        // l'établissement de la connexion de data
        strcpy(server, cli_params);
    }

    // USER
    else if (!strcmp(cli_command, "user") || !strcmp(cli_command, "u"))
    {
        if (strlen(cli_params) < 1)
            strcpy(cli_params, "anonymous");
        sprintf(ftp_command, "USER %s\r\n", cli_params);
        send_ftp_command(socket_control, ftp_command);
        get_ftp_command_reply(socket_control, ftp_reply);
        printf("\033[31;01m%s\033[00m\n", ftp_reply);
    }

    // PASSWD
    else if (!strcmp(cli_command, "pass") || !strcmp(cli_command, "passwd"))
    {
        if (strlen(cli_params) < 1)
            strcpy(cli_params, "anonymous@anonymous");
        sprintf(ftp_command, "PASS %s\r\n", cli_params);
        send_ftp_command(socket_control, ftp_command);
        get_ftp_command_reply(socket_control, ftp_reply);
        printf("\033[31;01m%s\033[00m\n", ftp_reply);
    }

    // STATUS
    else if (!strcmp(cli_command, "status"))
    {
        sprintf(ftp_command, "STAT\r\n");
        send_ftp_command(socket_control, ftp_command);
        get_ftp_command_reply(socket_control, ftp_reply);
        printf("\033[31;01m%s\033[00m\n", ftp_reply);
    }

    // PWD
    else if (!strcmp(cli_command, "pwd"))
    {
        sprintf(ftp_command, "PWD\r\n");
        send_ftp_command(socket_control, ftp_command);
        get_ftp_command_reply(socket_control, ftp_reply);
        printf("\033[31;01m%s\033[00m\n", ftp_reply);
    }

    // CWD
    else if (!strcmp(cli_command, "cd"))
    {
        sprintf(ftp_command, "CWD %s\r\n", cli_params);
        send_ftp_command(socket_control, ftp_command);
        get_ftp_command_reply(socket_control, ftp_reply);
        printf("\033[31;01m%s\033[00m\n", ftp_reply);
    }

    // LIST
    else if (!strcmp(cli_command, "ls") || !strcmp(cli_command, "dir"))
    {
        printf("# Getting passive port\n");
        int p = get_passive_port(socket_control);
        printf("%d\n", p);
        if (p < 0)
        {
            printf("# Could not get passive port\n");
            return;
        }

        printf("# Opening data connection on port %d\n", p);

        char port[8];
        sprintf(port, "%d", p);
        int socket_data = open_connection(server, port);
        printf("\n");

        if (socket_data > 0)
        {
            sprintf(ftp_command, "LIST\r\n");
            send_ftp_command(socket_control, ftp_command);
            int code = get_ftp_command_reply(socket_control, ftp_reply);
            printf("\033[31;01m%s\033[00m\n", ftp_reply);
            printf("%s\n", ftp_reply);   



            if (code == 150)
            {
                get_data(socket_data, ftp_reply, false);
                printf("%s\n", ftp_reply);
                close(socket_data);

                // pour récupérer le message de transfert terminé
                get_ftp_command_reply(socket_control, ftp_reply);
                printf("\033[31;01m%s\033[00m\n", ftp_reply);
            }
        }
    }

    // RETR
    else if (!strcmp(cli_command, "get"))
    {
        if (strlen(cli_params) < 1)
        {
            printf("# Please provide a filename\n");
            return;
        }

        int p = get_passive_port(socket_control);
        if (p < 0)
        {
            printf("# Could not get passive port\n");
            return;
        }

        char port[8];
        sprintf(port, "%d", p);
        int socket_data = open_connection(server, port);

        if (socket_data > 0)
        {
            sprintf(ftp_command, "RETR %s\r\n", cli_params);
            send_ftp_command(socket_control, ftp_command);
            int code = get_ftp_command_reply(socket_control, ftp_reply);
            printf("\033[31;01m%s\033[00m\n", ftp_reply);

            if (code == 150)
            {
                get_data(socket_data, cli_params, true);
                close(socket_data);

                // pour récupérer le message de transfert terminé
                get_ftp_command_reply(socket_control, ftp_reply);
                printf("\033[31;01m%s\033[00m\n", ftp_reply);
            }
        }
    }

    // CLOSE
    else if (!strcmp(cli_command, "close"))
    {
        sprintf(ftp_command, "QUIT\r\n");
        send_ftp_command(socket_control, ftp_command);
        get_ftp_command_reply(socket_control, ftp_reply);
        printf("\033[31;01m%s\033[00m\n", ftp_reply);
        socket_control = 0;
    }

    // EXIT
    else if (!strcmp(cli_command, "bye") || !strcmp(cli_command, "quit"))
    {
        exit(0);
    }
    

    // ERROR
    else
    {
        printf("Commande inconnue. Liste des commandes disponibles :\n");
        printf(" - open | o <server> : connexion au serveur\n");
        printf(" - user | u <username> : envoi du nom d'utilisateur\n");
        printf(" - passwd | pass <password> : envoi du mot de passe\n");
        printf(" - status : affiche des informations sur la connexion\n");
        printf(" - pwd : récupération du répertoire courant distant\n");
        printf(" - cd <directory> : change le répertoire courant sur le serveur\n");
        printf(" - ls | dir : liste des fichiers dans le répertoire courant\n");
        printf(" - get <filename> : téléchargement du fichier filename\n");
        printf(" - close : fermeture de la connexion avec le serveur\n");
        printf(" - bye | quit : quitter le programme\n");
    }
}
