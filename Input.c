#include <stdlib.h>
#include <stdio.h>
#include "Input.h"


input* criarTeclado(){

  input *teclado = (input*) malloc (sizeof(input));
  
  if (teclado == NULL){
    printf("Erro ao alocar memória para input");
    return NULL;
  }

  teclado->right = 0;
  teclado->left = 0;
  teclado->up = 0;
  teclado->down = 0;
  teclado->shot = 0;

  return teclado;
}

void destruirTeclado(input *teclado){
  free(teclado);
}

void tecladoUp(input *teclado){
  teclado->up = teclado->up ^ 1;
}

void tecladoDown(input *teclado){
  teclado->down = teclado->down ^ 1;
}
void tecladoRight(input *teclado){
  teclado->right = teclado->right ^ 1;
}
void tecladoLeft(input *teclado){
  teclado->left = teclado->left ^ 1;
}
void tecladoShot(input *teclado, char modo){
  teclado->shot = teclado->shot ^ 1;
}





