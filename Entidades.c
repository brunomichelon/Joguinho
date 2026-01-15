#include <stdlib.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <stdbool.h>
#include <math.h>
#include "Entidades.h"
#include "Input.h"

#include "Objetos.h"




entidade* criarEntidade(unsigned short Largura, unsigned short Altura, 
    unsigned char velocidade, unsigned short x, 
    unsigned short y, unsigned short largura, unsigned short altura,
    unsigned char dano, char dispTemp, bool face, ALLEGRO_BITMAP *imagem, 
                             char tipoMov, bool atirar, bool inimigo, short hp){

  // Garante que tamanho e posição sejam válidos dentro da tela
  /*if (x < Largura / 2 || x > largura - Largura / 2 ||
      y < Altura / 2 || y > altura - Altura / 2) {
    return NULL;
  }*/

  entidade *novaEntidade = (entidade*) malloc(sizeof(entidade));

  if (novaEntidade == NULL) {
    printf("Falha na alocação de entidade, Memoria insuficiente");
    return NULL;
  }

  // Inicializa o tamanho e a coordenada onde será criado
  novaEntidade->largura = Largura;
  novaEntidade->altura = Altura;
  novaEntidade->x = x;
  novaEntidade->y = y;
  novaEntidade->teclado = criarTeclado();
  novaEntidade->teclado->shot = atirar;
  novaEntidade->velocidade = velocidade;
  novaEntidade->velocidadeAux = velocidade;
  novaEntidade->hp = hp;
  novaEntidade->dano = dano;
  novaEntidade->danoAux= dano;
  novaEntidade->dispTemp = dispTemp;
  novaEntidade->dispTempAux = dispTemp;
  novaEntidade->dispUlt = 0;
  novaEntidade->face = face;
  novaEntidade->tipoMov = tipoMov;
  novaEntidade->tipoMovAux = tipoMov;
  novaEntidade->xInit = x;
  novaEntidade->yInit = y;
  novaEntidade->direcao = 1;
  novaEntidade->inimigo = inimigo;
  novaEntidade->spawn = true;
  novaEntidade->especialTipo = 0;
  novaEntidade->especialTimer = 0;
  novaEntidade->qtdProjeteis = 1;

  novaEntidade->imagem = imagem;

  return novaEntidade;
}

void anexarImagem(entidade *entidade, char frameMax, char frameAtual, 
  char frameContador, char frameDelay, short frameLargura, short frameAltura, 
                               char face, char projetilTipo, short frameColuna){

  entidade->frameMax = frameMax;
  entidade->frameAtual = frameAtual;
  entidade->frameContador = frameContador;
  entidade->frameDelay = frameDelay;
  entidade->frameDelayAux = frameDelay;
  entidade->frameLargura = frameLargura;
  entidade->frameAltura = frameAltura;
  entidade->frameDirecao = 1;
  entidade->frameFace = entidade->face;
  entidade->frameColuna = 1;
  entidade->projetilTipo = projetilTipo;
  entidade->frameColuna = frameColuna;
  entidade->projetilTipoAux = projetilTipo;
}

void anexarProjetil(Projetil *novoProjetil, entidade *entidade, 
                                              short projetilTipo, bool direcao){

  // Inicializações comuns a todos os tipos
  novoProjetil->frameDirecao = direcao;
  novoProjetil->frameAtual = 0;
  novoProjetil->frameContador = 0;

  // Inicializações de caracteristicas do tipo 0 e 1
  if (projetilTipo == 0 || projetilTipo == 1 || projetilTipo == 9 || projetilTipo == 10){
  novoProjetil->frameMax = 9;
  novoProjetil->frameDelay = 2;
  novoProjetil->frameLargura = 50;
  novoProjetil->frameAltura = 45;
  novoProjetil->largura = 30;
  novoProjetil->altura = 30;
  return;
  } else 
  // Inicializações de caracteristicas do tipo 5
  if(projetilTipo == 5){
    novoProjetil->frameMax = 4;
    novoProjetil->frameDelay = 2;
    novoProjetil->frameLargura = 50;
    novoProjetil->frameAltura = 45;
    novoProjetil->largura = 30;
    novoProjetil->altura = 30;
    novoProjetil->velocidade = 10;
    return;
  } else
  // Inicializações de caracteristicas do tipo 2
  if (projetilTipo == 2){
  novoProjetil->frameMax = 9;
  novoProjetil->frameDelay = 2;
  novoProjetil->frameLargura = 153;
  novoProjetil->frameAltura = 130;
  novoProjetil->largura = 72;
  novoProjetil->altura = 72;
  novoProjetil->velocidade = 15;
  entidade->frameColuna = 2; 
  novoProjetil->y = entidade->y + 40;
  novoProjetil->x += 20;
  return;
  } else  
    // Inicializações de caracteristicas do tipo 3
  if (projetilTipo == 3){
    novoProjetil->frameMax = 4;
    novoProjetil->frameDelay = 10;
    novoProjetil->frameLargura = 1500;
    novoProjetil->frameAltura = 500;
    novoProjetil->largura = 1200;
    novoProjetil->altura = 200;
    novoProjetil->velocidade = 0;
    novoProjetil->y = entidade->y + 20;
    novoProjetil->x -= 680;
    novoProjetil->dano = 0.0001;
    novoProjetil->timer = 220;
    entidade->frameColuna = 3; 
    entidade->tipoMov = 3;
    entidade->dispTemp = 0;
    entidade->teclado->shot = 0;
    return;
  } else 
  
  if (projetilTipo == 4){
    novoProjetil->frameMax = 5;
    novoProjetil->frameDelay = 0;
    novoProjetil->frameLargura = 92;
    novoProjetil->frameAltura = 100;
    novoProjetil->largura = 80;
    novoProjetil->altura = 80;
  return;
  } else

  if (projetilTipo == 6){
    novoProjetil->frameMax = 9;
    novoProjetil->frameDelay = 8;
    novoProjetil->frameLargura = 110;
    novoProjetil->frameAltura = 110;
    novoProjetil->largura = 80;
    novoProjetil->altura = 80;
  return;

  } else
  // Raio chefao 2
  if (projetilTipo == 7){
    novoProjetil->frameMax = 10;
    novoProjetil->frameDelay = 4;
    novoProjetil->frameLargura = 200;
    novoProjetil->frameAltura = 1080;
    novoProjetil->largura = 40;
    novoProjetil->altura = 500;
    novoProjetil->x = aleat(20, 500);
    entidade->frameColuna = 2;
    entidade->dispTemp = 255;
    novoProjetil->timer = 50;
    novoProjetil->dano = 0.000007;
  return;

  } else
  // Raio chefao 2
  if (projetilTipo == 8){
    novoProjetil->frameMax = 4;
    novoProjetil->frameDelay = 4;
    novoProjetil->frameLargura = 200;
    novoProjetil->frameAltura = 200;
    novoProjetil->largura = 150;
    novoProjetil->altura = 150;
  return;

}
}



void resetarEntidade(entidade *entidade){
  entidade->tipoMov = entidade->tipoMovAux;
  entidade->velocidade = entidade->velocidadeAux;
  entidade->dano = entidade->danoAux;
  entidade->projetilTipo = entidade->projetilTipoAux;
  entidade->dispTemp = entidade->dispTempAux;
  entidade->teclado->shot = 1;
}

bool colisaoEntidades(entidade *entidade1, entidade *entidade2){
 
  // Desabilita a colisão amiga
  if (entidade1->inimigo == entidade2->inimigo)
    return true;

  // Calcula as bordas da primeira entidade
  unsigned short esquerda1 = entidade1->x - entidade1->largura / 2;
  unsigned short direita1 = entidade1->x + entidade1->largura / 2;
  unsigned short topo1 = entidade1->y - entidade1->altura / 2;
  unsigned short base1 = entidade1->y + entidade1->altura / 2;

  // Calcula as bordas da segunda entidade
  unsigned short esquerda2 = entidade2->x - entidade2->largura / 2;
  unsigned short direita2 = entidade2->x + entidade2->largura / 2;
  unsigned short topo2 = entidade2->y - entidade2->altura / 2;
  unsigned short base2 = entidade2->y + entidade2->altura / 2;

  //Calcula as colisao entre as 2 entidades

  bool colisaoX = direita1 > esquerda2 && esquerda1 < direita2;
  bool colisaoY = base1 > topo2 && topo1 < base2;

  if (colisaoX && colisaoY){
    return false;
  } else {
    return true;
  }
}


void colisaoProjeteisEntidades(Projetil **primeiro, entidade **entidades, 
                                                              int numEntidades){

  Projetil *projetilAux = *primeiro;
  Projetil *projetilProximo;

  while (projetilAux != NULL){
    projetilProximo = projetilAux->proximo;

    // Colisão entre projéteis e entidades
    for (int i = 0; i < numEntidades; i++){

      entidade *entidadeAtual = entidades[i];

      // Desabilita o dano amigo 
      if(entidadeAtual->inimigo != projetilAux->trajetoria){

      // Calcula as bordas da entidade
      unsigned short esquerda1 = entidadeAtual->x - entidadeAtual->largura / 2;
      unsigned short direita1 = entidadeAtual->x + entidadeAtual->largura / 2;
      unsigned short topo1 = entidadeAtual->y - entidadeAtual->altura / 2;
      unsigned short base1 = entidadeAtual->y + entidadeAtual->altura / 2;

      // Calcula a bordas do projétil
      unsigned short esquerda2 = projetilAux->x - projetilAux->largura / 2;
      unsigned short direita2 = projetilAux->x + projetilAux->largura / 2;
      unsigned short topo2 = projetilAux->y - projetilAux->altura / 2;
      unsigned short base2 = projetilAux->y + projetilAux->altura / 2;

      // Calcula colisão entre o projetil e a entidade
      bool colisaoX = direita1 > esquerda2 && esquerda1 < direita2;
      bool colisaoY = base1 > topo2 && topo1 < base2;

      if (projetilAux->tipo == 3){
       if (colisaoX && colisaoY){
          entidadeAtual->hp -= projetilAux->dano;
          break;
       }
      }

      if (projetilAux->tipo == 7){
       if (colisaoX && colisaoY){
          entidadeAtual->hp -= projetilAux->dano;
          break;
       }
      }

      if (colisaoX && colisaoY){
        entidadeAtual->hp -= projetilAux->dano;
        destruirProjetil(primeiro, projetilAux);  
        break;
      }
    }
    }
    projetilAux = projetilProximo;
  }
}

void colisaoEntreProjeteis(Projetil **primeiro) {
  Projetil *projetilAux = *primeiro;

  while (projetilAux != NULL) {
    Projetil *projetilProximo = projetilAux->proximo;
    Projetil *projetilInterno = projetilAux->proximo;


    // Verificar colisão entre projéteis diferentes
    while (projetilInterno != NULL) {
      // Ignorar colisão entre projéteis com a mesma trajetória
      if (projetilAux->trajetoria != projetilInterno->trajetoria) {
        // Detectar colisão na borda
        bool colisaoX = projetilAux->x < projetilInterno->x + 
                        projetilInterno->largura / 2 &&
                        projetilAux->x + projetilAux->largura / 2 > 
                        projetilInterno->x - projetilInterno->largura / 2;

        bool colisaoY = projetilAux->y < projetilInterno->y + 
                        projetilInterno->altura / 2 &&
                        projetilAux->y + projetilAux->altura / 2 > 
                        projetilInterno->y - projetilInterno->altura / 2;

        
        // Se o projetil for do tipo 8, destroi o outro
        if (projetilInterno->tipo == 8){
          if (colisaoX && colisaoY) 
          destruirProjetil(primeiro, projetilAux);
          break;
        }

        /*if (projetilAux->tipo == 8){
          destruirProjetil(primeiro, projetilInterno);
          break;
        }*/
        
        // Se houver colisão, destruir ambos os projéteis
        if (projetilInterno->tipo != 3 && projetilAux->tipo != 3 &&
              projetilInterno->tipo != 8 && projetilAux->tipo != 8){
        if (colisaoX && colisaoY) {
          destruirProjetil(primeiro, projetilAux);
          destruirProjetil(primeiro, projetilInterno);
          break;  // Sair do loop após destruir os projéteis
        }
        }
      }
      projetilInterno = projetilInterno->proximo;
    }

    projetilAux = projetilProximo;
  }
}


void colisaoProjeteis(Projetil **primeiro, entidade **entidades, 
                                                              int numEntidades){

  colisaoProjeteisEntidades(primeiro, entidades, numEntidades);
  colisaoEntreProjeteis(primeiro);
}


// Move apenas a entidade jogavel, e verifica a colisão com as outras
void moverAux(entidade **entidades, unsigned char numEntidades, 
                                 unsigned short largura, unsigned short altura){ 

    entidade *entidadeAtual = entidades[0];
    
    if (entidadeAtual->teclado != NULL){

      unsigned short centralX = entidadeAtual->largura / 2;
      unsigned short centralY = entidadeAtual->altura / 2;
      unsigned short velocidade = entidadeAtual->velocidade;

      // Coordenadas antigas
      unsigned short xAntigo = entidadeAtual->x;
      unsigned short yAntigo = entidadeAtual->y;

       // Se nenhum botão for precionado, a sprite volta ao normal
      if (entidadeAtual->teclado->up == 0 && entidadeAtual->teclado->down == 0 &&
          entidadeAtual->teclado->right == 0 && entidadeAtual->teclado->left == 0 &&
          entidadeAtual->x - centralX >= 0){

          entidadeAtual->x -= 0.3;
          entidadeAtual->frameDelay = entidadeAtual->frameDelayAux; 
      } 

      // Movimentos condicionais com limites de borda
      
      if (entidadeAtual->teclado->up && entidadeAtual->y - centralY > 0){
        entidadeAtual->y -= velocidade;
        entidadeAtual->frameDelay += entidadeAtual->frameDelay/2;
      }
 
      if (entidadeAtual->teclado->right && 
                                         entidadeAtual->x + centralX < largura){ 
                                          
        entidadeAtual->x += velocidade;
        entidadeAtual->frameDelay += entidadeAtual->frameDelay/2;
      }

       if (entidadeAtual->teclado->down && entidadeAtual->y + centralY < altura){
        entidadeAtual->y += velocidade;
        entidadeAtual->frameAtual = 1;
      }

      if (entidadeAtual->teclado->left && entidadeAtual->x - centralX > 0){
        entidadeAtual->x -= velocidade;
        entidadeAtual->frameDelay = 15;
      } 
      

      // Verifica colisão com todas as outras entidades
      for (int j = 0; j < numEntidades; j++) {
        if (j != 0 && !colisaoEntidades(entidadeAtual, entidades[j])) {
          // Se houver colisão, retorna a posição antiga

          if (entidades[j]->tipoMov != 5){
            entidadeAtual->hp = 0;
            entidades[j]->hp = 0;
            entidadeAtual->x = xAntigo;
            entidadeAtual->y = yAntigo;
          break;
          } else {
          entidadeAtual->especialTipo = aleat(1,2);
          entidades[j]->hp = 0;
          break;
          }
          
        }
      }
    }  
    
}


void moverEntidade(entidade *entidade1, int tecla, char modo) {

  if (entidade1->teclado != NULL){    
    // Controle usando W, A, S, D para a primeira entidade
    if (tecla == ALLEGRO_KEY_W) {
      entidade1->teclado->up = modo;
    } else if (tecla == ALLEGRO_KEY_S) {
      entidade1->teclado->down = modo;
    } else if (tecla == ALLEGRO_KEY_D) {
      entidade1->teclado->right = modo;
    } else if (tecla == ALLEGRO_KEY_A) {
      entidade1->teclado->left = modo;
    } else if (tecla == ALLEGRO_KEY_C && modo == 0) { 
      entidade1->teclado->shot = !entidade1->teclado->shot;
    }
  }

}


void destruirEntidade(entidade *entidade){
  if (entidade != NULL){
    // Libera o ponteiro teclado, se alocado dinamicamente
    if (entidade->teclado != NULL){
      free(entidade->teclado);
      entidade->teclado = NULL;
    }

    if (entidade->imagem != NULL){
      entidade->imagem = NULL;
    }

    entidade->velocidade = 0;
    entidade->largura = 0;
    entidade->altura = 0;
    entidade->x = 3000;
    entidade->y = 3000;
    entidade->hp = 0;
  }
}



void desenharEntidade(entidade **entidades, short numEntidades){
    for (short i = 0; i < numEntidades; i++) {      
        if (entidades[i]->hp > 0) { 
            short x = entidades[i]->frameAtual * entidades[i]->frameLargura;
            short y = 0;

            if (entidades[i]->frameColuna == 2){
              y = 500;
            } else if(entidades[i]->frameColuna == 3){
              y = 1000;
            }

            // Se a face for false (direção invertida), inverte a imagem sem mudar a posição
            if (entidades[i]->face == true) {
                // Desenha a imagem invertida horizontalmente sem alterar a posição
                al_draw_bitmap_region(
                    entidades[i]->imagem, x, y, 
                    entidades[i]->frameLargura, entidades[i]->frameAltura, 
                    entidades[i]->x - entidades[i]->frameLargura / 2, 
                    entidades[i]->y - entidades[i]->frameAltura / 2, 
                    ALLEGRO_FLIP_HORIZONTAL
                );
            } else {
                // Caso contrário, desenha normalmente
                al_draw_bitmap_region(
                    entidades[i]->imagem, x, y, 
                    entidades[i]->frameLargura, entidades[i]->frameAltura, 
                    entidades[i]->x - entidades[i]->frameLargura / 2, 
                    entidades[i]->y - entidades[i]->frameAltura / 2, 
                    0
                );
            }
        } else {
            destruirEntidade(entidades[i]);
        }
    }
}




void atualizarEntidade(entidade **entidades, short numEntidades) {
    for (short i = 0; i < numEntidades; i++) { 
        // Acessa as entidades corretamente usando ponteiro duplo
        if (entidades[i]->frameContador >= entidades[i]->frameDelay) {

            // Verifica se é hora de mudar de frame
            if (entidades[i]->frameDirecao == 1) {
                // Se estamos indo para frente (aumentando)
                if (entidades[i]->frameAtual < entidades[i]->frameMax - 1) {
                    entidades[i]->frameAtual++;  // Aumenta o frame
                } else {
                    // Se atingiu o limite, inverte a direção
                    entidades[i]->frameDirecao = -1;
                    entidades[i]->frameAtual--;  // Começa a diminuir
                }
            } else {
                // Se estamos indo para trás (diminuindo)
                if (entidades[i]->frameAtual > 0) {
                    entidades[i]->frameAtual--;  // Diminui o frame
                } else {
                    // Se atingiu o limite, inverte a direção
                    entidades[i]->frameDirecao = 1;
                    entidades[i]->frameAtual++;  // Começa a aumentar
                }
            }

            // Reseta o contador de frames para o próximo ciclo
            entidades[i]->frameContador = 0;
        } else {
            // Incrementa o contador de frames
            entidades[i]->frameContador++;
        }
    }
}

void movimentoSenoidal(entidade *entidade) {
    // Parâmetros do movimento
    float amplitude = 200;  
    float frequencia = 0.01; 
    short velocidade = entidade->velocidade / 2;     
    short suavizacao = 100;     

    // Limites de movimento baseados nas dimensões da entidade
    short larguraMax = entidade->xInit + entidade->largura;
    short larguraMin = entidade->xInit - entidade->largura;
    short alturaMax = entidade->yInit + entidade->altura * 20;
    short alturaMin = entidade->yInit - entidade->altura * 20;

    // Verificar se a entidade atingiu o limite superior ou inferior para a posição horizontal
    if (entidade->direcao == 1) {
        entidade->x += velocidade; // Movimento para a direita
    } else {
        entidade->x -= velocidade; // Movimento para a esquerda
    }

    // Suavização na volta ao atingir os limites
    if (entidade->x >= larguraMax) {
        // Quando a entidade atinge o limite superior, começa a desacelerar
        entidade->x = larguraMax; // Limita a posição para não ultrapassar
        if (entidade->x - larguraMax < suavizacao) {
            entidade->direcao = 0; // Muda a direção para a esquerda
        }
    } else if (entidade->x <= larguraMin) {
        // Quando a entidade atinge o limite inferior, começa a desacelerar
        entidade->x = larguraMin; // Limita a posição para não ultrapassar
        if (larguraMin - entidade->x < suavizacao) {
            entidade->direcao = 1; // Muda a direção para a direita
        }
    }

    // Atualizar a posição vertical (Y) com movimento senoidal
    entidade->y = (short)(entidade->yInit + amplitude * sin(frequencia * entidade->x));

    // Garantir que o movimento vertical respeite os limites
    if (entidade->y > alturaMax) {
        entidade->y = alturaMax;
    } else if (entidade->y < alturaMin) {
        entidade->y = alturaMin;
    }
}

void movimentoPerseguir(entidade **entidades, entidade *entidadeAtual) {
    // Calculando as diferenças de posição entre a entidade atual e a entidade[0]
    
    if (entidadeAtual->spawn == false){

    short deltaX = entidades[0]->x - entidadeAtual->x;
    short deltaY = entidades[0]->y - entidadeAtual->y;

    if (deltaX < 0){
    entidadeAtual->face = false;
    }
    else {entidadeAtual->face = true;}

    // Acelerando o movimento, ajustando a direção em X e Y
    unsigned char velocidade = entidadeAtual->velocidade;

    // Normalizando o movimento para evitar que a entidade se mova mais rápido diagonalmente
    float distancia = sqrt(deltaX * deltaX + deltaY * deltaY);
    if (distancia != 0) {
        // Movendo a entidade na direção de X e Y
        entidadeAtual->x += (deltaX / distancia) * velocidade;
        entidadeAtual->y += (deltaY / distancia) * velocidade;
    }
    } else { 
      /* O movimento perseguir é reaproveitado para o spawn, então ao spawnar
      o movimeno spawn == true serve para setar o jogador do x fora da dela para 
      o X desejado, ao alcnçar o X desejado, seu movimento é retornado ao padrão*/
      
      short deltaX = entidadeAtual->xInit - entidadeAtual->x;
      short deltaY = entidadeAtual->yInit -entidadeAtual->y;

      // Acelerando o movimento, ajustando a direção em X e Y
      unsigned char velocidade = entidadeAtual->velocidade;

      // Normalizando o movimento para evitar que a entidade se mova mais rápido diagonalmente
      float distancia = sqrt(deltaX * deltaX + deltaY * deltaY);
      if (distancia != 0) {
        // Movendo a entidade na direção de X e Y
        entidadeAtual->x += (deltaX / distancia) * velocidade;
        entidadeAtual->y += (deltaY / distancia) * velocidade;
      }

      if (entidadeAtual->x <= entidadeAtual->xInit + 10){
      entidadeAtual->tipoMov = entidadeAtual->tipoMovAux;
      entidadeAtual->spawn = false;
      }

    }

}


void movimentoVertical(entidade *entidadeAtual){
    
    short velocidade = entidadeAtual->velocidade;

    if (entidadeAtual->y - entidadeAtual->altura/2 - velocidade <= 0){
       
        entidadeAtual->direcao = 0;
    } else if (entidadeAtual->y + entidadeAtual->altura/2 + velocidade > 1000){
    
        entidadeAtual->direcao = 1;  
    }

    // Movimenta a entidade com base na direção
    if (entidadeAtual->direcao == 1) {
        entidadeAtual->y -= velocidade;  // Move para cima
    } else if (entidadeAtual->direcao == 0) {
        entidadeAtual->y += velocidade;  // Move para baixo
    }
}


void movimentacaoAleatoria(entidade **entidades, char numEntidades, 
                           unsigned short largura, unsigned short altura){
    
  for (short i = 1; i < numEntidades; i++){
    entidade *entidadeAtual = entidades[i];
    
    
      unsigned short centralX = entidadeAtual->largura / 2;
      unsigned short centralY = entidadeAtual->altura / 2;
      unsigned short velocidade = entidadeAtual->velocidade;

      // Coordenadas antigas
      unsigned short xAntigo = entidadeAtual->x;
      unsigned short yAntigo = entidadeAtual->y;

      
      // Movimentos condicionais com limites de borda
      /*if (entidadeAtual->y - centralY > 0 && entidadeAtual->x + centralX < largura && 
            entidadeAtual->y + centralY < altura && entidadeAtual->x - centralX > 0){*/
            
        if (entidadeAtual->tipoMov == 0){
          movimentoSenoidal(entidadeAtual);
        } else
          if (entidadeAtual->tipoMov == 1 || entidadeAtual->tipoMov == 4){
            movimentoPerseguir(entidades, entidadeAtual);
        } else 
          if (entidadeAtual->tipoMov == 2){
            movimentoVertical(entidadeAtual);
        }

        if (entidadeAtual->tipoMov == 5){
            entidadeAtual->x -= 4;
        }
        
      // Verifica colisão com todas as outras entidades
      for (int j = 0; j < numEntidades; j++) {
        if (j != 0 && !colisaoEntidades(entidadeAtual, entidades[j])) {
          // Se houver colisão, retorna a posição antiga
          entidadeAtual->x = xAntigo;
          entidadeAtual->y = yAntigo;
          break;
        }
      }
  }
}

void destruirEntidadesExcetoPrimeira(entidade **entidades, int numEntidades) {
  for (int i = 1; i < numEntidades; i++) {
      if (entidades[i]) {
          destruirEntidade(entidades[i]);
      }
  }
}







