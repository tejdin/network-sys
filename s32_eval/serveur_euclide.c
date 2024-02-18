#include "eval.h"

int main(int argc, char *argv[]) {
  // test des arguments de la commande
  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  serveur_euclide_udp(argv[1]);
}
