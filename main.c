#include "campo.h"


int main () {
  int escolha = 0;
  limpaTela ();
  printf ("====================================\n");
  printf ("=           CAMPO MINADO           =\n");
  printf ("====================================\n");
  printf ("Escolha a dificuldade:\n");
  printf ("1. Fácil\n");
  printf ("2. Normal\n");
  printf ("3. Difícil\n");
  printf ("4. Sair\n");
  scanf (" %d", &escolha);
  switch (escolha) {
  case 1:
    campo_minado (8, 8, 10);
    break;
  case 2:
    campo_minado (16, 16, 40);  
    break;
  case 3:
    campo_minado (26, 20, 99);
    break;
  default:
    printf ("Tchau!");
    break;
  }
  return EXIT_SUCCESS;
}
