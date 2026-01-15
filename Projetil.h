#ifndef __PROJETIL__
#define __PROJETIL__


typedef struct Projetil{
  unsigned short x;
  unsigned short y;
  unsigned char velocidade;
  float dano;
  unsigned short largura;
  unsigned short altura;
  unsigned short timer;
  bool trajetoria;
  struct Projetil *proximo;
  ALLEGRO_BITMAP *imagem;

  char frameMax;
  char frameAtual;
  char frameContador;
  char frameDelay;
  short frameLargura;
  short frameAltura;
  char frameDirecao;
  bool vivo;
  char tipo;

} Projetil;

Projetil* criarProjetil(Projetil **primeiro, unsigned short x, unsigned short y,
  unsigned char velocidade, unsigned char dano, unsigned short largura, 
  unsigned short altura, bool face, ALLEGRO_BITMAP **imagem, char projetilTipo);

void destruirDoisProjeteis(Projetil **primeiro, Projetil *projetil, 
                                                           Projetil *projetil2);

void destruirProjetil(Projetil **primeiro, Projetil *projetil);

void desenharProjeteis(Projetil **primeiroProjetil);

void desenharProjetil(Projetil **primeiro);

void atualizarProjetil(Projetil **primeiro);

#endif
