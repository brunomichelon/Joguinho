#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "Projetil.h"

Projetil* criarProjetil(Projetil **primeiro, unsigned short x, unsigned short y
          ,unsigned char velocidade, unsigned char dano, unsigned short largura, 
  unsigned short altura, bool face, ALLEGRO_BITMAP **imagem, char projetilTipo){

  Projetil *novoProjetil = (Projetil*)malloc(sizeof(Projetil));

  if (novoProjetil == NULL)
    return NULL;

  novoProjetil->x = x;
  novoProjetil->y = y;
  novoProjetil->velocidade = velocidade;
  novoProjetil->dano = dano;
  novoProjetil->largura = largura;
  novoProjetil->largura = altura;
  novoProjetil->proximo = *primeiro;
  novoProjetil->trajetoria = face; 
  novoProjetil->timer = 1;
  *primeiro = novoProjetil;
  novoProjetil->vivo = true;
  novoProjetil->tipo = projetilTipo;

  novoProjetil->imagem = *imagem;

  return novoProjetil;
}







void destruirDoisProjeteis(Projetil **primeiro, Projetil *projetil, 
                                                           Projetil *projetil2){

  if (projetil == NULL)
    return;


  if (*primeiro == projetil) {
    *primeiro = projetil->proximo;
  } else if (*primeiro == projetil2) {
    *primeiro = projetil2->proximo;
  }

  // Certifique-se de que projetil2->proximo não é NULL antes de acessá-lo
  if (projetil2->proximo != NULL) {
    *primeiro = projetil2->proximo->proximo;
  }

  free(projetil);
  free(projetil2);

}




void destruirProjetil(Projetil **primeiro, Projetil *projetil) {
  if (projetil == NULL) return;

  // Caso o projétil seja o primeiro da lista
  if (*primeiro == projetil) {
    *primeiro = projetil->proximo;  // Ajusta o primeiro para o próximo projétil
  } else {
    // Caso contrário, percorre a lista para encontrar o projétil
    Projetil *atual = *primeiro;
    while (atual != NULL && atual->proximo != projetil) {
      atual = atual->proximo;
    }
    if (atual != NULL) {
      // Remove o projétil da lista
      atual->proximo = projetil->proximo;
    }
  }

  // Libera a memória do projétil destruído
   free(projetil);

  // Definir o ponteiro do projétil para NULL (para evitar acessar memória já liberada)
  projetil = NULL;
}

void desenharProjeteis(Projetil **primeiroProjetil) {
  struct Projetil *atual = *primeiroProjetil;

  while (atual != NULL) {
    al_draw_filled_rectangle(
        atual->x - atual->largura / 2,
        atual->y - atual->altura / 2,
        atual->x + atual->largura / 2,
        atual->y + atual->altura / 2,
        al_map_rgb(0, 255, 255)
        );

    // Avança para o próximo projetil na lista
    atual = atual->proximo;
  }
}

void desenharProjetil(Projetil **primeiro) {

  Projetil *atual = *primeiro;

  while (atual != NULL) {      

    // Determinar o frame atual na sprite sheet.
    short x = atual->frameAtual * atual->frameLargura;
    short y = 0; 

    // Desenhar a entidade.
    al_draw_bitmap_region(
        atual->imagem, x, y, 
        atual->frameLargura, atual->frameAltura, 
        atual->x - atual->frameLargura / 2, 
        atual->y - atual->frameAltura / 2, 0);

    atual = atual->proximo;
  }
}

void atualizarProjetil(Projetil **primeiro){

  Projetil *atual = *primeiro;

  while (atual != NULL) { 
    if (atual->frameContador >= atual->frameDelay) {

      // Verifica se é hora de mudar de frame
      if (atual->frameDirecao == 1) {
        // Se estamos indo para frente (aumentando)
        if (atual->frameAtual < atual->frameMax - 1) {
          atual->frameAtual++;  // Aumenta o frame
        } else {
          // Se atingiu o limite, inverte a direção
          atual->frameDirecao = -1;
          atual->frameAtual--;  // Começa a diminuir
        }
      } else {
        // Se estamos indo para trás (diminuindo)
        if (atual->frameAtual > 0) {
          atual->frameAtual--;  // Diminui o frame
        } else {
          // Se atingiu o limite, inverte a direção
          atual->frameDirecao = 1;
          atual->frameAtual++;  // Começa a aumentar
        }
      }

      // Reseta o contador de frames para o próximo ciclo
      atual->frameContador = 0;
    } else {
      // Incrementa o contador de frames
      atual->frameContador++;
    }
    atual = atual->proximo;
  }
}







