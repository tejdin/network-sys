#include "server-encoder.h"


// code à découvrir
char code[CODE_LENGTH];


int main (int argc, char *argv[]) {
    int backlog = atoi(argv[1]);   
    int sock_clients[backlog];
    // test des paramètres
    if (argc < 3) {
        printf("Usage: %s <port> <code to discover>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strlen(argv[3]) < CODE_LENGTH) {
        printf("The given code is too short. Please provide %zu digits\n", CODE_LENGTH);
        exit(EXIT_FAILURE);
    }
    memcpy(code, argv[3], CODE_LENGTH);

    // création de la structure d'adresse pour la création du socket
    struct addrinfo hints , *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;
    if(getaddrinfo("localhost",argv[2],&hints,&res) ==-1){
        perror("hi");
        return EXIT_FAILURE;
    }

    // création d'un socket pour se mettre en attente de requêtes
    int sock = socket(res->ai_family,res->ai_socktype,res->ai_protocol);

    // attachement du socket au port passé en paramètre
    if(bind(sock,res->ai_addr,res->ai_addrlen)){
            perror("hi");
        return EXIT_FAILURE;
    }

    // préparation du socket pour les connexions des clients
    if(listen(sock,backlog)){
            perror("hi");
        return EXIT_FAILURE;
    }




    // en attente d'une connexion
    for (int i = 0; i< backlog; i++)
    {
        sock_clients[i] = accept(sock,NULL,NULL);
        printf("> connection received %d\n",i);
        send(sock_clients[i],"If you strike me down, I shall become more powerful than you can possibly imagine.\n",100,0);
        if(send(sock_clients[i],"Waiting for others ...\n",100,0) == -1){
            perror("send");
            return EXIT_FAILURE;
        }
    }

    for (int i = 1; i <= backlog; i++)
    {
        char msg[100];
        sprintf(msg,"Game Started You are the %dth player, FIGHT \n",i);
        if(send(sock_clients[i-1],msg,100,0) == -1){
            perror("send");
            return EXIT_FAILURE;
        }
    }
    

    printf("> Game started, FIGHT\n");


    // la connexion est établie et on lance la boucle de jeu
    char proposition[backlog][CODE_LENGTH+1];
    uint8_t good_place=0;
    uint8_t good_number=0;
    uint8_t code=0;
    int winner = 0;

    while (1)
    {
        if(winner>0){
            printf("You win\n");
            break;
        }

        for (int i = 0; i < backlog; i++)
        {
            if(recv(sock_clients[i],proposition[i],CODE_LENGTH+1,0) ==-1){
                perror("send");
                return EXIT_FAILURE;
            }
        }
        for (int i = 0; i < backlog; i++)
        {
            code = test_code(proposition[i],&good_place,&good_number);
            if(send(sock_clients[i],&good_place,sizeof(uint8_t),0) ==-1){
                perror("send");
                return EXIT_FAILURE;
            }
            if(send(sock_clients[i],&good_number,sizeof(uint8_t),0) ==-1){
                perror("send");
                return EXIT_FAILURE;
            }
            if(send(sock_clients[i],&code,sizeof(uint8_t),0) ==-1){
                perror("send");
                return EXIT_FAILURE;
            }
            if(code == CODE_FOUND){
                winner = i+1;
            }
        }
        for (int i = 0; i < backlog; i++)
        {
            if(send(sock_clients[i],&winner,sizeof(int),0) ==-1){
                perror("send");
                return EXIT_FAILURE;
            }
        }     
    }

    for (int i = 0; i < backlog; i++)
    {
        if(winner == i+1){
            if(send(sock_clients[i],"You win\n",100,0) ==-1){
                perror("send");
                return EXIT_FAILURE;
            }
        }
        else{
            if(send(sock_clients[i],"You lose\n",100,0) ==-1){
                perror("send");
                return EXIT_FAILURE;
            }
        }
    }

   

    // fermeture de la connexion
    for (size_t i = 0; i < atoi(argv[1]); i++)
    {
        close(sock_clients[i]);
    }
    
    close(sock);
}


/*
 * Fonction permettant de tester la combinaison proposée par un décodeur
 *  - proposition : la proposition à tester
 *  - good_place  : nombre de chiffres bien placés (paramètre de sortie)
 *  - good_number : nombre de chiffres présents mal placés (paramètre de sortie)
 *
 * Valeurs renvoyées : CODE_FOUND si le code a été trouvé, CODE_NOT_FOUND sinon
 */
uint8_t test_code(const char* proposition, uint8_t* good_place, uint8_t* good_number) {
  char *test = malloc(CODE_LENGTH+1);
  *good_place  = 0;
  *good_number = 0;
  memcpy(test, code, CODE_LENGTH);
  test[CODE_LENGTH] = '\0';

    // test des chiffres bien placés
    for (int i=0 ; i<CODE_LENGTH ; i++) {
        printf("Testing: %s - %s\n", proposition, test);
        if (proposition[i] == test[i]) {
            (*good_place)++;
            test[i]='x';
        }
    }

    // test des chiffres présents mal placés
    for (int i=0 ; i<CODE_LENGTH ; i++) {
        if (strchr(test, proposition[i]) != NULL) {
            (*good_number)++;
            str_remove(test, proposition[i]);
        }
    }

    if (*good_place == CODE_LENGTH)
        return CODE_FOUND;

    free(test);

    return CODE_NOT_FOUND;
}

/*
 * Fonction remplaçant un caractère dans une chaîne par le caractère 'x'
 * utilisée pour l'évaluation des propositions
 *  - string    : la chaîne à modifier
 *  - to_remove : le caractère à remplacer par 'x'
 */
void str_remove(char * string, const char to_remove) {
    for(int i=0 ; i<strlen(string) ; i++)
        if (string[i]==to_remove) {
            string[i] = 'x';
        }
}
