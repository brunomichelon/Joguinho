#ifndef __OBJETOS__
#define __OBJETOS__

#include <allegro5/allegro5.h>														
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdbool.h>


#include "Projetil.h"
#include "Entidades.h"
#include "Input.h"

typedef struct{
  float x;
  unsigned short y;
  unsigned short largura;
  unsigned short altura;
  float velocidadeX;
  float velocidadeY;

  bool direcaoX;
  bool direcaoY;

  ALLEGRO_BITMAP *imagem;
 
} Background;

typedef struct {

  ALLEGRO_SAMPLE *musica;
  ALLEGRO_SAMPLE_INSTANCE *controlador;

} Som;

typedef struct{

  short numEntidades;
  short numEntidadesAux;
  short numProjImg;
  short numBackgrounds;
  short largura;
  short altura;
  short chefaoIdx;
  bool chefeVivo;
  short numMusicas;

  Background **backgrounds;
  ALLEGRO_BITMAP **projImg;
  ALLEGRO_BITMAP **inimImg;
  entidade **entidades;
  entidade **entidadesAux;
  struct Projetil **primeiro;
  ALLEGRO_FONT *fonte;
  Som **musicas;
  int tempo;

  short faseNumero;
  

} Fase;



int aleat(int min, int max);

Som *carregarMusica(const char *arquivo, float volume);
void destruirMusica(Som *musica);

void redimencionarVetor(Fase *fase, short quantidade);
Background* criarBackground(float x, unsigned short y,
    unsigned short largura, unsigned short altura, float velocidadeX,
    float velocidadeY, bool direcaoX, bool direcaoY, ALLEGRO_BITMAP *imagem);
void atualizarEntidade(entidade **entidades, short numEntidades);
void atualizarBackground(Fase *fase);
void spawnarEntidade(Fase *fase, unsigned short quantidade, short tipoEntidade);
void desenharBackground(Fase *fase);

Fase *criarFase1(ALLEGRO_BITMAP **backgrounds, short numBackgrounds, 
 ALLEGRO_BITMAP **projeteisImg, short numImagens, short largInit, short altInit, 
             Projetil **primeiro, ALLEGRO_FONT *fonte, ALLEGRO_BITMAP **inimImg, 
                             short numInimImg, Som **musicas, short numMusicas);

Fase *criarFase2(ALLEGRO_BITMAP **backgrounds, short numBackgrounds, 
                 ALLEGRO_BITMAP **projeteisImg, short numImagens, 
                 short largInit, short altInit, Projetil **primeiro, 
                 ALLEGRO_FONT *fonte, ALLEGRO_BITMAP **inimImg, 
                             short numInimImg, Som **musicas, short numMusicas);

void destruirFase(Fase *fase);
void destruirBackground(Background *background);
bool moverProjetil(Fase *fase, Projetil *projetil, unsigned short larguraTela);
bool moverTodosProjeteis(Fase *fase, unsigned short larguraTela);
void atirarProjetil(Fase *fase, ALLEGRO_BITMAP **imagem);
void aleatorizarChefao(Fase *fase);

void gerenciarEventos(Fase *fase, int tempo);

short gerenciarFase(Fase *fase, ALLEGRO_TIMER *timer, unsigned short largInit, 
                                                        unsigned short altInit);

entidade *retornarEntidade(Fase *fase);


void desenharEntidades(entidade **entidades, short numEntidades);






#endif
