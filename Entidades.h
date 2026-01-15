#ifndef __ENTIDADES__
#define __ENTIDADES__

#include "Input.h"
#include "Projetil.h"


typedef struct{
    unsigned char velocidade;
    unsigned char velocidadeAux;
    unsigned short dano;
    unsigned short danoAux;
    unsigned char dispTemp;
    unsigned char dispTempAux;
    unsigned char dispUlt;
    bool spawn;
    int qtdProjeteis;


    unsigned short largura;
    unsigned short altura;
    float x;
    float y;
    float xInit;
    float yInit;
    char direcao;
    bool inimigo;
    int especialTipo;
    int especialTimer;
    
    bool face;
    short hp;

    input *teclado;

    char frameMax;
    char frameAtual;
    char frameContador;
    char frameDelay;
    char frameDelayAux;
    short frameLargura;
    short frameAltura;
    char frameDirecao;
    char frameFace;
    char frameColuna;
    char projetilTipo;
    char projetilTipoAux;

    char tipoMov;
    char tipoMovAux;

    ALLEGRO_BITMAP *imagem;

    
} entidade;


entidade* criarEntidade(unsigned short Largura, unsigned short Altura,
  unsigned char velocidade, unsigned short x, unsigned short y, 
  unsigned short largura, unsigned short altura, unsigned char dano, 
  char dispTemp, bool face, ALLEGRO_BITMAP *imagem, char tipoMov, bool atirar, 
  bool inimigo, short hp);

void resetarEntidade(entidade *entidade);

void anexarImagem(entidade *entidade, char frameMax, char frameAtual, 
  char frameContador, char frameDelay, short frameLargura, short frameAltura, 
                               char face, char projetilTipo, short frameColuna);

void anexarProjetil(Projetil *novoProjetil, entidade *entidade, 
                                              short projetilTipo, bool direcao);

bool colisaoEntidades(entidade *entidade1, entidade* entidade2);
void colisaoProjeteisEntidades(Projetil **primeiro, entidade **entidades, 
                                                              int numEntidades);



void moverAux(entidade **entidades, unsigned char numEntidades, 
                                 unsigned short largura, unsigned short altura);


void moverEntidade(entidade *entidade1, int tecla, char modo);

void destruirEntidade(entidade *entidade);

void colisaoProjeteis(Projetil **primeiro, entidade **entidades, 
                                                              int numEntidades);

void desenharEntidade(entidade **entidades, short numEntidades);
void atualizarEntidade(entidade **entidades, short numEntidades);
void movimentoSenoidal(entidade *entidade);
void movimentoVertical(entidade *entidadeAtual);
void movimentoPerseguir(entidade **entidades, entidade *entidadeAtual);
void movimentacaoAleatoria(entidade **entidades, char numEntidades, 
                           unsigned short largura, unsigned short altura);
void destruirEntidadesExcetoPrimeira(entidade **entidades, int numEntidades);


#endif
