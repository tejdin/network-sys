#include "eval.h"

int main(int argc, char *argv[]) {
  // test des arguments de la commande
  if (argc != 5) {
    printf("Usage : %s <nomserveur> <port> <a> <b>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  client_euclide_udp(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]));
}
