#include "client-decoder.h"

int main (int argc, char *argv[]) {
    // test des paramètres
    char msgAccueil[100];
    
    if (argc < 3) {
        printf("Usage: %s <encoder host> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // création de la structure d'adresse pour la création du socket
    struct addrinfo hints , *res;
    // création d'un socket pour communiquer avec l'encoder (serveur)
    memset(&hints, 0, sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;
    if(getaddrinfo(argv[1],argv[2],&hints,&res) ==-1){
        perror("hi");
        return EXIT_FAILURE;
    }
    int sock = socket(res->ai_family,res->ai_socktype,res->ai_protocol);

    // connexion au serveu
    if(connect(sock,res->ai_addr,res->ai_addrlen)){
            perror("hi");
        return EXIT_FAILURE;

    }
    // boucle d'envoi des propositions
    for(int i=0;i<3;i++){
        if(recv(sock,msgAccueil,100,0) ==-1){
            perror("send");
            return EXIT_FAILURE;
        }
        printf("%s\n",msgAccueil);
    }
   

    char proposition[CODE_LENGTH+1];
    uint8_t good_place = 0;
    uint8_t good_number=0;
    uint8_t code = 0;
    int winner=0;

    while(1){

        if(winner>0){
            printf("You win\n");
            break;
        }   
        printf(">");
        get_proposition(proposition);

        if(send(sock,proposition,CODE_LENGTH+1,0) ==-1){
            perror("send");
            return EXIT_FAILURE;
        }
        if(recv(sock,&good_place,sizeof(uint8_t),0)==-1){
            perror("send");
            return EXIT_FAILURE;
        }

        if(recv(sock,&good_number,sizeof(uint8_t),0) ==-1){
            perror("send");
            return EXIT_FAILURE;
        }
        if(recv(sock,&code,sizeof(uint8_t),0) ==-1){
            perror("send");
            return EXIT_FAILURE;
        }
        if(recv(sock,&winner,sizeof(int),0) ==-1){
            perror("send");
            return EXIT_FAILURE;
        }
        print_result(good_place,good_number);
    
    }

    if(recv(sock,msgAccueil,100,0) ==-1){
            perror("send");
            return EXIT_FAILURE;
        }
    printf("%s\n",msgAccueil);
   
}

/*
 * Fonction d'affichage du résultat de l'analyse de la combinaison proposée.
 * Chaque * correspond à un chiffre bien placé dans la proposition
 * Chaque - correspond à un chiffre présent mais mal placé
 */
void print_result(uint8_t good_place, uint8_t good_number) {
    for (int i=0 ; i < good_place ; i++)
        printf("* ");
    for (int i=0 ; i < good_number ; i++)
        printf("- ");
    printf("\n");
}

/*
 * Fonction demandant la saisie au clavier d'une proposition de combinaison
 *  - proposition : la suite de CODE_LENGTH caractères saisie au clavier
 *
 * Attention ! proposition se composera de CODE_LENGTH caractères et doit avoir été allouée avant l'appel
 *
 * Valeurs renvoyées : EXIT_FAILURE en cas d'erreur de lecture de l'entrée standard, EXIT_SUCCESS sinon
 */
 int get_proposition(char* proposition) {
     char input[CODE_LENGTH+1];
     if (fgets(input, CODE_LENGTH+1, stdin) == NULL) {
         perror("fgets stdin");
         return(EXIT_FAILURE);
     }
     memcpy(proposition, input, CODE_LENGTH);
     fflush(stdin);

     return EXIT_SUCCESS;
 }

 /*
  * Fonction d'affichage d'une proposition
  */
 void print_proposition(char* proposition) {
   for (int i = 0 ; i < CODE_LENGTH ; i++)
     printf("%c", proposition[i]);
   printf("\n");
 }
