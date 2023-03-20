#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// bool
#define TRUE 1
#define FALSE 0
// cores dos caracteres
#define BRANCO 0
#define VERDE 1
#define VERMELHO 2
#define AMARELO 3
#define MAGENTA 7
// cores de fundo
#define PRETO 4
#define AZUL 5
#define RESET 6

void muda_cor(int cor);
int campo_minado (int linhas, int colunas, int minas);
int gera_campo ();
void imprime_campo ();
void preenche_campo ();
int verifica_em_volta (int x, int y);
void mostra_todo_campo ();
void limpaTela ();
void sair ();
void pisa (char letra, int numero);
void coloca_bandeira (char letra, int numero);
int converte_coordenada (char letra);
int pisou_na_bomba ();
void imprime_campo_final ();
void abre_campo (int i, int j);
int ganhou ();
void muda_cor_fundo(int cor);


int **campo;
int campo_linhas = 4;
int campo_colunas = 4;
int campo_minas = 1;
int **pisadas;
int **bandeiras;

int campo_minado (int linhas, int colunas, int minas) {
  campo_linhas = linhas;
  campo_colunas = colunas;
  campo_minas = minas;
  muda_cor_fundo (PRETO);
  char jogada;
    char letra;
    int numero;

    limpaTela ();
    int ok = gera_campo ();
    if (ok)
      preenche_campo ();
      imprime_campo ();
      //imprime_campo_final ();

    while (TRUE) {
      
      printf ("Digite P para pisada, B para bandeira e S para sair: ");
      scanf (" %c", &jogada);
      if (jogada != 'P' && jogada != 'p' && jogada != 'B' && jogada != 'b') {
          printf ("Destistiu! Que pena!\n");
          sair ();
      };
      int condicao_ok;
      do {
        fflush (stdin);
        printf ("Digite uma letra e um número separados com espaço: ");
        scanf (" %c", &letra);
        scanf (" %d", &numero);
        condicao_ok = ((letra >= 'a' && letra < 'a' + campo_linhas) || (letra >= 'A' && letra < 'A' + campo_linhas)) && (numero >= 0 && numero < campo_colunas);
        //printf ("condição = %d\n", condicao_ok);
        if (!condicao_ok)
          printf ("Digite uma coordenada válida do campo.\n");
      } while (condicao_ok == FALSE);
      //printf ("Jogada %c: coordenada %c %d\n", jogada, letra, numero);
      if (jogada == 'b' || jogada == 'B') {
        coloca_bandeira (letra, numero);
      }
      if (jogada == 'p' || jogada == 'P') {
        pisa (letra, numero);
      }
      limpaTela ();
      imprime_campo ();
      if (ganhou ()) {
        printf ("Parabéns! Você ganhou!\n");
        exit (EXIT_SUCCESS);
      }
    }

    sair ();
}

int gera_campo () {
    int i, j;
    campo = (int **)malloc(campo_linhas * sizeof (int *));
    pisadas = (int **)malloc(campo_linhas * sizeof (int *));
    bandeiras = (int **)malloc(campo_linhas * sizeof (int *));
    if (campo == NULL || pisadas == NULL || bandeiras == NULL)
      return 0;
    for (i = 0; i < campo_linhas; i++) {
        campo[i] = (int *)malloc(campo_colunas * sizeof (int));
        pisadas[i] = (int *)malloc(campo_colunas * sizeof (int));
        bandeiras[i] = (int *)malloc(campo_colunas * sizeof (int));
        if (campo[i] == NULL || pisadas[i] == NULL || bandeiras[i] == NULL)
          return 0;
        for (j = 0; j < campo_colunas; j++) {
          campo[i][j] = 0;
          pisadas[i][j] = FALSE;
          bandeiras[i][j] = FALSE;
        }
    }
    return 1;
}

void imprime_campo () {
    int i, j;
    muda_cor (BRANCO);
    printf ("___|");
    for (j = 0; j < campo_colunas; j++) {
        if (j < 10)
          printf ("_%d_|", j);
        else
          printf ("%2d_|", j);
    }
    printf ("\n");
    for (i = 0; i < campo_linhas; i++) {
        muda_cor (BRANCO);
        printf ("%2c |", 65+i);
        muda_cor (VERDE);
        for (j = 0; j < campo_colunas; j++) {
            if (bandeiras[i][j] == TRUE) {
              muda_cor (AMARELO);
              printf ("_P_");
              muda_cor (VERDE);
              printf ("|");
            } else {
              if (pisadas[i][j] == TRUE) {
                  if (campo[i][j] == 0)
                    printf ("%2c |", '.');
                  if (campo[i][j] == -1)
                    printf ("%2c |", '*');
                  if (campo[i][j] > 0) {
                    muda_cor (MAGENTA);
                    printf ("%2d ", campo[i][j]);
                    muda_cor (VERDE);
                    printf ("|");
                  }
              } else {
                  printf ("%2c |", '#');
              }
            }
        }
        printf ("\n");
    }
}

void preenche_campo () {
    srand (time (NULL));
    int i, j, cont = 0;

    while (cont < campo_minas) {
        i = rand() % campo_linhas;
        j = rand() % campo_colunas;
        if (campo[i][j] == 0) {
            campo[i][j] = -1;
            cont++;
        }
    }

    for (i = 0; i < campo_linhas; i++)
      for (j = 0; j < campo_colunas; j++)
        if (campo[i][j] == 0)
          campo[i][j] = verifica_em_volta (i, j);

}

int verifica_em_volta (int x, int y) {
  int i, j, cont = 0;
  i = x - 1;
  j = y - 1;
  if (i >= 0 && j >= 0 && i < campo_linhas && j < campo_colunas && campo[i][j] == -1)
    cont++;
  i = x - 1;
  j = y;
  if (i >= 0 && j >= 0 && i < campo_linhas && j < campo_colunas && campo[i][j] == -1)
    cont++;
  i = x - 1;
  j = y + 1;
  if (i >= 0 && j >= 0 && i < campo_linhas && j < campo_colunas && campo[i][j] == -1)
    cont++;
  i = x;
  j = y + 1;
  if (i >= 0 && j >= 0 && i < campo_linhas && j < campo_colunas && campo[i][j] == -1)
    cont++;
  i = x + 1;
  j = y + 1;
  if (i >= 0 && j >= 0 && i < campo_linhas && j < campo_colunas && campo[i][j] == -1)
    cont++;
  i = x + 1;
  j = y;
  if (i >= 0 && j >= 0 && i < campo_linhas && j < campo_colunas && campo[i][j] == -1)
    cont++;
  i = x + 1;
  j = y - 1;
  if (i >= 0 && j >= 0 && i < campo_linhas && j < campo_colunas && campo[i][j] == -1)
    cont++;
  i = x;
  j = y - 1;
  if (i >= 0 && j >= 0 && i < campo_linhas && j < campo_colunas && campo[i][j] == -1)
    cont++;
  return cont;
}

void sair () {
  int i, j;
  for (i = 0; i < campo_linhas; i++) {
    free (campo[i]);
    free (pisadas[i]);
    free (bandeiras[i]);
  }
  free (campo);
  free (pisadas);
  free (bandeiras);

  muda_cor_fundo (RESET);
  exit (EXIT_SUCCESS);
}

void limpaTela () {
  printf ("limpaTela ()\n");
  system ("clear");
}

void muda_cor(int cor) {
  switch (cor) {
    case 0:
      printf ("\x1b[37m");
      break;
    case 1:
      printf ("\x1b[32m");
      break;
    case 2:
      printf ("\x1b[31m");
      break;
    case 3:
      printf ("\x1b[33m");
      break;
    case 7:
      printf ("\x1b[35m");
      break;
    default: 
      printf ("\x1B[0m"); //reset
      break;
  };
}

void muda_cor_fundo(int cor) {
  switch (cor) {
    case 4:
      printf ("\033[40m");
      break;
    case 5:
      printf ("\033[44m");
      break;
    default:
      printf ("\033[0m"); //reset
      break;
  }
}

void pisa (char letra, int numero) {
  int i = converte_coordenada (letra);
  int j = numero;

  pisadas[i][j] = TRUE;

  if (pisou_na_bomba ()) {
    limpaTela ();
    imprime_campo_final ();
    printf ("Boom! Você perdeu!\n");
    sair ();
  } else {
    printf ("antes do primeiro abre_campo (%d, %d)\n", i, j);
    pisadas[i][j] = FALSE;
    abre_campo (i, j);
    pisadas[i][j] = TRUE;
  }
}

void coloca_bandeira (char letra, int numero) {
  int i = converte_coordenada (letra);

  int j = numero;
  if (bandeiras[i][j] == FALSE)
    bandeiras[i][j] = TRUE;
  else
    bandeiras[i][j] = FALSE;
}

int converte_coordenada (char letra) {
  if ('A' <= letra && letra < 'A' + campo_linhas)
    return letra - 'A';
  return letra - 'a';
}

int pisou_na_bomba () {
  printf ("entrou no pisou_na_bomba\n");
  int pisou = FALSE;
  int i, j;
  for (i = 0; i < campo_linhas; i++)
    for (j = 0; j < campo_colunas; j++)
      if (campo[i][j] == -1 && pisadas[i][j] == TRUE)
        return TRUE;
  return FALSE;
}

void imprime_campo_final () {
    int i, j;
    muda_cor (BRANCO);
    printf ("___|");
    for (j = 0; j < campo_colunas; j++) {
        if (j < 10)
          printf ("_%d_|", j);
        else
          printf ("%2d_|", j);
    }
    printf ("\n");
    for (i = 0; i < campo_linhas; i++) {
      muda_cor (BRANCO);
      printf ("%2c |", 65+i);
      muda_cor (VERDE);
      for (j = 0; j < campo_colunas; j++) {
          if (bandeiras[i][j] == TRUE) {
            muda_cor (AMARELO);
            printf ("_P_");
            muda_cor (VERDE);
            printf ("|");
          } else {
            if (campo[i][j] == 0)
              printf ("%2c |", '.');
            if (campo[i][j] == -1) {
              muda_cor (VERMELHO);
              printf ("%2c ", '*');
              muda_cor (VERDE);
              printf ("|");
            }
            if (campo[i][j] > 0) {
                    muda_cor (MAGENTA);
                    printf ("%2d ", campo[i][j]);
                    muda_cor (VERDE);
                    printf ("|");
                  }
          }
      }
      printf ("\n");
    }
}

void abre_campo (int x, int y) {
  int i, j;
  i = x;
  j = y;
  //printf ("abre_campo (%d, %d)\n", i, j);

  if (pisadas[i][j] == FALSE && campo[i][j] >= 0) {
    if (campo[i][j] >= 0)
      pisadas[i][j] = TRUE;
    if (campo[i][j] == 0) {
      bandeiras[i][j] = FALSE;
      
      i = x - 1;
      j = y - 1;
      if (i >= 0 && j >= 0 && i < campo_linhas && j < campo_colunas) {
        //printf ("abre_campo (%d, %d)\n", i, j);
        abre_campo (i,j);
      }

      i = x - 1;
      j = y;
      if (i >= 0 && j >= 0 && i < campo_linhas && j < campo_colunas) {
        //printf ("abre_campo (%d, %d)\n", i, j);
        abre_campo (i,j);
      }

      i = x - 1;
      j = y + 1;
      if (i >= 0 && j >= 0 && i < campo_linhas && j < campo_colunas) {
        //printf ("abre_campo (%d, %d)\n", i, j);
        abre_campo (i,j);
      }

      i = x;
      j = y + 1;
      if (i >= 0 && j >= 0 && i < campo_linhas && j < campo_colunas) {
        //printf ("abre_campo (%d, %d)\n", i, j);
        abre_campo (i,j);
      }

      i = x + 1;
      j = y + 1;
      if (i >= 0 && j >= 0 && i < campo_linhas && j < campo_colunas) {
        //printf ("abre_campo (%d, %d)\n", i, j);
        abre_campo (i,j);
      }

      i = x + 1;
      j = y;
      if (i >= 0 && j >= 0 && i < campo_linhas && j < campo_colunas) {
        //printf ("abre_campo (%d, %d)\n", i, j);
        abre_campo (i,j);
      }

      i = x + 1;
      j = y - 1;
      if (i >= 0 && j >= 0 && i < campo_linhas && j < campo_colunas) {
        //printf ("abre_campo (%d, %d)\n", i, j);
        abre_campo (i,j);
      }

      i = x;
      j = y - 1;
      if (i >= 0 && j >= 0 && i < campo_linhas && j < campo_colunas) {
        //printf ("abre_campo (%d, %d)\n", i, j);
        abre_campo (i,j);
      }
    }
  }
}

int ganhou () {
  int cont_bandeiras = 0;
  int cont_pisadas = 0;
  int i,j;
  for (i = 0; i < campo_linhas; i++)
    for (j = 0; j < campo_colunas; j++) {
      if (bandeiras[i][j] == TRUE)
        cont_bandeiras++;
      if (pisadas[i][j] == TRUE)
        cont_pisadas++;
    }
  if (cont_bandeiras == campo_minas && cont_pisadas == (campo_linhas * campo_colunas - campo_minas))
    return TRUE;
  return FALSE;
}
