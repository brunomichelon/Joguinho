#include <allegro5/allegro5.h>														
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdlib.h>
#include <stdio.h>
#include "Objetos.h"
#include <math.h>

// Numero de cada entidade
#define DEMONIO 0 
#define PIRANHA 1
#define GOLEM 2
#define DRAGAO 3
#define SOMBRA1 4
#define SOMBRA2 5
#define CHEFE2 6
#define BOOST 7

// Tamanho padrão de projetil
#define PROJ_TAM 50

// função que envia numeros aleatorios ente min e max
int aleat(int min, int max){
  return rand() % (max - min + 1) + min;
}

// Função que carrega uma musica e sua instancia no vetor
Som *carregarMusica(const char *arquivo, float volume) {
    
    Som *novaMusica = malloc(sizeof(Som));
    if (!novaMusica) {
        fprintf(stderr, "Erro ao alocar memória para Musica.\n");
        return NULL;
    }

    novaMusica->musica = al_load_sample(arquivo);
    if (!novaMusica->musica) {
        fprintf(stderr, "Erro ao carregar arquivo de música: %s\n", arquivo);
        free(novaMusica);
        return NULL;
    }

    novaMusica->controlador = al_create_sample_instance(novaMusica->musica);
    if (!novaMusica->controlador) {
        fprintf(stderr, "Erro ao criar controlador para a música.\n");
        al_destroy_sample(novaMusica->musica);
        free(novaMusica);
        return NULL;
    }

    // Seta o modo da musica para loop
    al_set_sample_instance_playmode
                               (novaMusica->controlador, ALLEGRO_PLAYMODE_LOOP);

    // Anexar ao mixer padrão
    al_attach_sample_instance_to_mixer
                              (novaMusica->controlador, al_get_default_mixer());

    // Ajustar o volume
    al_set_sample_instance_gain(novaMusica->controlador, volume);

    return novaMusica;
}

// Destroi o nodulo da musica completamente
void destruirMusica(Som *musica) {
    if (musica) {
        if (musica->controlador) {
            al_destroy_sample_instance(musica->controlador);
        }
        if (musica->musica) {
            al_destroy_sample(musica->musica);
        }
        free(musica);
    }
}

// Musica tocada ao spawnar dragão
void spawnDragao(Fase *fase, bool modo) {

// Toca a musica apenas uma vez
  al_set_sample_instance_playmode(fase->musicas[1]->controlador, ALLEGRO_PLAYMODE_ONCE);

  if (modo) {
    // Tocar a música de spawn do dragão
    al_set_sample_instance_playing(fase->musicas[1]->controlador, true);
  } 
}

// Função para tocar ou pausar o som do sopro do dragão
void soproDragao(Fase *fase, bool modo) {
    if (modo) {
        // Tocar o som do sopro
        al_set_sample_instance_playing(fase->musicas[2]->controlador, true);
    } else {
        // Pausar o som do sopro (similar a "parar" o sample)
        al_set_sample_instance_playing(fase->musicas[2]->controlador, false);
    }
}

// Função para tocar ou pausar o som das asas do dragão
void asasDragao(Fase *fase, bool modo) {
    if (modo) {
        // Tocar o som das asas
        al_set_sample_instance_playing(fase->musicas[3]->controlador, true);
    } else {
        // Pausar o som das asas
        al_set_sample_instance_playing(fase->musicas[3]->controlador, false);
    }
}

// Função que toca o som de disparo do projetil do dragão
void projetilDragao(Fase *fase, bool modo) {
        al_set_sample_instance_playmode(fase->musicas[4]->controlador, 
                                                         ALLEGRO_PLAYMODE_ONCE);
        al_play_sample_instance(fase->musicas[4]->controlador);
   
}

void projetilJogador(Fase *fase, bool modo) {

        
        al_set_sample_instance_playmode(fase->musicas[5]->controlador, 
                                                         ALLEGRO_PLAYMODE_ONCE);
        al_set_sample_instance_position(fase->musicas[5]->controlador, 1.0f);
        al_play_sample_instance(fase->musicas[5]->controlador);
   
}

void somProjetil(Fase *fase, short tipoProjetil, bool modo){

  if (tipoProjetil == 0 || tipoProjetil == 9 || tipoProjetil == 10)
  projetilJogador(fase, modo);

  if (tipoProjetil == 2 || tipoProjetil == 4){
    projetilDragao(fase, modo);
  }

  if (tipoProjetil == 3){
    soproDragao(fase, modo);
  }

}

void pausarMusicas(Fase *fase){

  for (short i = 0; i < fase->numMusicas; i++){
    al_set_sample_instance_playing(fase->musicas[i]->controlador, false);
  }
  
}


// Cria o background
Background* criarBackground(float x, unsigned short y,
               unsigned short largura, unsigned short altura, float velocidadeX,
                                float velocidadeY, bool direcaoX, bool direcaoY, 
                                                        ALLEGRO_BITMAP *imagem){

  // Aloca espaço dinamicamente para o background
  Background *background = (Background*) malloc(sizeof(Background));
  if (background == NULL) {
      return NULL;
  }

  background->x = x;
  background->y = y;
  background->largura = largura;
  background->altura = altura;
  background->velocidadeX = velocidadeX;
  background->velocidadeY = velocidadeY;
  background->direcaoX = direcaoX;
  background->direcaoY = direcaoY;
  background->imagem = imagem;

  return background;
}


void redimencionarVetor(Fase *fase, short quantidade){
  
  unsigned short novaQuantidade = fase->numEntidades + quantidade;
    
    fase->entidades = (entidade**) realloc(fase->entidades, novaQuantidade * sizeof(entidade*));
    if (!fase->entidades) {
        fprintf(stderr, "Erro ao redimensionar o vetor de entidades.\n");
        return;
    }
    fase->numEntidades++;
}

// Seta tudo que é necessario para cada tipo de entidade
void spawnarEntidade(Fase *fase, unsigned short quantidade, short tipoEntidade){
   
    // Retorna se a quantidade for 0
    if (quantidade == 0) return;
    
    // Variaveis dinamicas
    unsigned short larguraEntidade;
    unsigned short alturaEntidade;
    unsigned char velocidade;
    unsigned char dano;
    char dispTemp;
    char tipoMov;
    short hp;

    // Variaveis comuns à todas as entidades
    bool face = true;
    bool atirar = true;
    bool inimigo = true;
    short largura = fase->largura;
    short altura = fase->altura;

    // Variavel de spawn aleatorio no mapa
    unsigned short spawn = 
                        aleat(fase->largura, fase->largura + 300);
    
   
    // ---------------------Criação das entidades-----------------------//

    // Demonio Vermelho
    if(tipoEntidade == 0){

      // Variaveis especificas da entidade
      larguraEntidade = 100;
      alturaEntidade = 90;
      velocidade = aleat(2, 4);
      dano = 10;
      dispTemp = 80;
      tipoMov = 0;
      hp = 100;
      bool criado = false;
      ALLEGRO_BITMAP *imagem = fase->inimImg[0];

      for (short i = 0; i < quantidade; i++){
        // Gera posições aleatórias dentro dos limites da tela
        unsigned short x = aleat
                             (fase->largura/2, fase->largura - larguraEntidade);
        unsigned short y = aleat
                           (alturaEntidade*2 , fase->altura - alturaEntidade*2);

        // Cria a entidade na posição gerada
        entidade *entidade = criarEntidade
                                (larguraEntidade, alturaEntidade, velocidade, x,
                                       y, largura, altura, dano, dispTemp, face,
                                            imagem, tipoMov, atirar, false, hp);

        // Tipo de movimento setado para 4 para sair do spawn e ir ao destino
        entidade->tipoMov = 4;
        
        // Verifica colisão
        bool colidiu = false;

        // Numero maximo de tentativas de criação de entidade sem colisão
        int tentativas = 0;

        // Tenta criar entidades sem colidir umas com as outras
        // Se o numero de tentativas estourar, ele cria colidindo mesmo
        while (!criado && tentativas < 5) {
          int j = 1;
          tentativas++;
          colidiu = false; 
        
          // Verifica a colisão entre as entidades do mundo
          while (j < fase->numEntidades) {
            if (colisaoEntidades(entidade, fase->entidades[j])) {
              colidiu = true;
              break;  // Colidiu, então sai do loop
            }
            j++;
          }

            // Move para uma nova posição se colidiu
          if (colidiu) {
            entidade->x = aleat
                           (fase->largura / 2, fase->largura - larguraEntidade);
            entidade->y = aleat(0, fase->altura - alturaEntidade*2);
          } else { // Se não houve colisão, a entidade pode ser criada
              entidade->inimigo = true;
              criado = true; // A entidade foi criada com sucesso
            }
        }
        
        // Se a entidade pode ser criada, ela é criada
        if (entidade) {
            entidade->inimigo = true; // Seta a entidade para inimigo
            entidade->x = spawn; // Spawn em um lugar aleatório fora da tela
            
            // Aumenta o tamanho do vetor em 1 e da um realloc
            redimencionarVetor(fase, quantidade);

            // Seta a entidade criada na ultima posição do vetor
            fase->entidades[fase->numEntidades - 1] = entidade;

            // Anexa a imagem e caracteristicas respectivas à entidade
            anexarImagem(fase->entidades[fase->numEntidades - 1], 4, 0, 0, 6, 
                                                             115, 100, 1, 1, 1);
        } else { // Se a entidade não for criada, mostra o erro
            fprintf(stderr, "Erro ao criar a entidade.\n");
        }
      }
    } else {

    // Piranha Demoniaca
    if(tipoEntidade == 1){

    // Variaveis especificas da entidade
    larguraEntidade = 35;
    alturaEntidade = 35;
    velocidade = aleat(5, 8);
    dano = 10;
    dispTemp = 10;
    tipoMov = 1;
    hp = 1;
    bool criado = false;  
    atirar = 0;
    ALLEGRO_BITMAP *imagem = fase->inimImg[1];
   

    for (short i = 0; i < quantidade; i++){

        // Gera posições aleatórias dentro dos limites da tela
        unsigned short x = 2000;
        unsigned short y = aleat(0 , fase->altura - alturaEntidade*2);
    
        // Cria a entidade na posição gerada
        entidade *entidade = criarEntidade(
            larguraEntidade, alturaEntidade, velocidade, x, y,
            largura, altura, dano, dispTemp, face,
            imagem, tipoMov, atirar, false, hp
        );
        
        bool colidiu = false;
        int tentativas = 0;
        while (!criado && tentativas < 5) {
          int j = 1;
          tentativas++;
          colidiu = false; 
        
          while (j < fase->numEntidades) {
            if (colisaoEntidades(entidade, fase->entidades[j])) {
              colidiu = true;
              break;  // Colidiu, então sai do loop
            }
            j++;
          }

          if (colidiu) {
            // Move para uma nova posição se colidiu
            entidade->y = aleat(0, fase->altura - alturaEntidade*2);
          } else {
            // Se não houve colisão, a entidade pode ser criada
              entidade->inimigo = true;
              criado = true; // A entidade foi criada com sucesso
            }
        }
        
        if (entidade) {
            // Adiciona a nova entidade ao vetor
            entidade->spawn = false;
            entidade->inimigo = true;
            redimencionarVetor(fase, quantidade);
            fase->entidades[fase->numEntidades - 1] = entidade;
            anexarImagem(fase->entidades[fase->numEntidades - 1], 4, 0, 0, 9, 43, 35, 1, 1, 1);
        } else {
            fprintf(stderr, "Erro ao criar a entidade.\n");
        }
    }
  } else {

    // Golem infernal
    if(tipoEntidade == 2){
    
    // Variaveis especificas da entidade
    larguraEntidade = 150;
    alturaEntidade = 150;
    velocidade = aleat(1, 3);
    dano = 10;
    dispTemp = 80;
    tipoMov = 2;
    hp = 100;
    bool criado = false;
    ALLEGRO_BITMAP *imagem = fase->inimImg[2];
  
    for (short i = 0; i < quantidade; i++){

        // Gera posições aleatórias dentro dos limites da tela
        unsigned short x = aleat(fase->largura/2 + larguraEntidade, fase->largura - larguraEntidade);
        unsigned short y = aleat(200 , fase->altura - 200);
    
        // Cria a entidade na posição gerada
        entidade *entidade = criarEntidade(
            larguraEntidade, alturaEntidade, velocidade, x, y,
            largura, altura, dano, dispTemp, face,
            imagem, tipoMov, atirar, false, hp
        );
        
        entidade->tipoMov = 4;

        bool colidiu = false;

        int tentativas = 0;
        while (!criado && tentativas < 5) {
          int j = 1;
          tentativas++;
          colidiu = false; 
        
          while (j < fase->numEntidades) {
            if (colisaoEntidades(entidade, fase->entidades[j])) {
              colidiu = true;
              break;  // Colidiu, então sai do loop
            }
            j++;
          }

          if (colidiu) {
            // Move para uma nova posição se colidiu
            entidade->x = aleat
                           (fase->largura / 2, fase->largura - larguraEntidade);
            entidade->y = aleat (0, fase->altura - alturaEntidade*2);
          } else {
            // Se não houve colisão, a entidade pode ser criada
              entidade->inimigo = true;
              criado = true; // A entidade foi criada com sucesso
            }
        }
        
        if (entidade) {
            // Adiciona a nova entidade ao vetor
            entidade->x = spawn;
            entidade->inimigo = true;
            redimencionarVetor(fase, quantidade);
            fase->entidades[fase->numEntidades - 1] = entidade;
            anexarImagem(fase->entidades[fase->numEntidades - 1], 4, 0, 0, 9, 
                                                             200, 300, 1, 4, 1);
        } else {
            fprintf(stderr, "Erro ao criar a entidade do golem.\n");
        }
    }
  } else {

  // Chefão :)
  if(tipoEntidade == 3){
       
    // Variaveis especificas da entidade
    larguraEntidade = 500;
    alturaEntidade = 420;
    velocidade = 2;
    dano = 100;
    dispTemp = 80;
    tipoMov = 2;
    hp = 3500;
    bool criado = false;
    ALLEGRO_BITMAP *imagem = fase->inimImg[3];

    for (short i = 0; i < quantidade; i++){

        // Gera posições aleatórias dentro dos limites da tela
        unsigned short x = fase->largura - larguraEntidade/2;
        unsigned short y = fase->altura/2;
    
        // Cria a entidade na posição gerada
        entidade *entidade = criarEntidade(
            larguraEntidade, alturaEntidade, velocidade, x, y,
            largura, altura, dano, dispTemp, face,
            imagem, tipoMov, atirar, true, hp
        );

        entidade->tipoMov = 4;
        
        if (entidade) {
            // Adiciona a nova entidade ao vetor
            entidade->x = spawn;
            entidade->inimigo = true;
            fase->chefeVivo = 1;
            redimencionarVetor(fase, quantidade);
            fase->entidades[fase->numEntidades - 1] = entidade;
            anexarImagem(fase->entidades[fase->numEntidades - 1], 4, 0, 0, 9, 
                                                             800, 500, 1, 2, 1);
            fase->chefaoIdx = fase->numEntidades - 1; // Atualiza o index chefe
        } else {
            fprintf(stderr, "Erro ao criar a entidade do golem.\n");
        }
    }

  } else 
  if(tipoEntidade == 4){
    
    // Variaveis especificas da entidade
    larguraEntidade = 150;
    alturaEntidade = 260;
    velocidade = aleat(1, 3);
    dano = 10;
    dispTemp = 100;
    tipoMov = 2;
    hp = 100;
    bool criado = false;
    ALLEGRO_BITMAP *imagem = fase->inimImg[4];
  
    for (short i = 0; i < quantidade; i++){

        // Gera posições aleatórias dentro dos limites da tela
        unsigned short x = aleat(fase->largura/2, fase->largura - larguraEntidade);
        unsigned short y = aleat(200 , fase->altura - 200);
    
        // Cria a entidade na posição gerada
        entidade *entidade = criarEntidade(
            larguraEntidade, alturaEntidade, velocidade, x, y,
            largura, altura, dano, dispTemp, face,
            imagem, tipoMov, atirar, false, hp
        );
        
        entidade->tipoMov = 4;

        bool colidiu = false;

        int tentativas = 0;
        while (!criado && tentativas < 5) {
          int j = 1;
          tentativas++;
          colidiu = false; 
        
          while (j < fase->numEntidades) {
            if (colisaoEntidades(entidade, fase->entidades[j])) {
              colidiu = true;
              break;  // Colidiu, então sai do loop
            }
            j++;
          }

          if (colidiu) {
            // Move para uma nova posição se colidiu
            entidade->x = aleat
                           (fase->largura / 2, fase->largura - larguraEntidade);
            entidade->y = aleat (0, fase->altura - alturaEntidade*2);
          } else {
            // Se não houve colisão, a entidade pode ser criada
              entidade->inimigo = true;
              criado = true; // A entidade foi criada com sucesso
            }
        }
        
        if (entidade) {
            // Adiciona a nova entidade ao vetor
            entidade->x = spawn;
            entidade->inimigo = true;
            redimencionarVetor(fase, quantidade);
            fase->entidades[fase->numEntidades - 1] = entidade;
            anexarImagem(fase->entidades[fase->numEntidades - 1], 4, 0, 0, 10, 
                                                             190, 300, 1, 5, 1);
        } else {
            fprintf(stderr, "Erro ao criar a entidade do golem.\n");
        }
    }
  } else 
  if(tipoEntidade == 5){
    
    // Variaveis especificas da entidade
    larguraEntidade = 110;
    alturaEntidade = 250;
    velocidade = aleat(1, 3);
    dano = 10;
    dispTemp = 40;
    tipoMov = 0;
    hp = 100;
    bool criado = false;
    ALLEGRO_BITMAP *imagem = fase->inimImg[5];
  
    for (short i = 0; i < quantidade; i++){

        // Gera posições aleatórias dentro dos limites da tela
        unsigned short x = aleat(fase->largura/2, fase->largura - larguraEntidade);
        unsigned short y = aleat(200 , fase->altura - 200);
    
        // Cria a entidade na posição gerada
        entidade *entidade = criarEntidade(
            larguraEntidade, alturaEntidade, velocidade, x, y,
            largura, altura, dano, dispTemp, face,
            imagem, tipoMov, atirar, false, hp
        );
        
        entidade->tipoMov = 4;

        bool colidiu = false;

        int tentativas = 0;
        while (!criado && tentativas < 5) {
          int j = 1;
          tentativas++;
          colidiu = false; 
        
          while (j < fase->numEntidades) {
            if (colisaoEntidades(entidade, fase->entidades[j])) {
              colidiu = true;
              break;  // Colidiu, então sai do loop
            }
            j++;
          }

          if (colidiu) {
            // Move para uma nova posição se colidiu
            entidade->x = aleat
                           (fase->largura / 2, fase->largura - larguraEntidade);
            entidade->y = aleat (0, fase->altura - alturaEntidade*2);
          } else {
            // Se não houve colisão, a entidade pode ser criada
              entidade->inimigo = true;
              criado = true; // A entidade foi criada com sucesso
            }
        }
        
        if (entidade) {
            // Adiciona a nova entidade ao vetor
            entidade->x = spawn;
            entidade->spawn = true;
            entidade->inimigo = true;
            redimencionarVetor(fase, quantidade);
            fase->entidades[fase->numEntidades - 1] = entidade;
            anexarImagem(fase->entidades[fase->numEntidades - 1], 4, 0, 0, 10, 
                                                             180, 300, 1, 5, 1);
        } else {
            fprintf(stderr, "Erro ao criar a entidade do golem.\n");
        }
    }
  } else // Chefao fase 2
  if(tipoEntidade == 6){
    
    // Variaveis especificas da entidade
    larguraEntidade = 300;
    alturaEntidade = 500;
    velocidade = aleat(1, 3);
    dano = 50;
    dispTemp = 70;
    tipoMov = 2;
    hp = 3000;
    bool criado = false;
    ALLEGRO_BITMAP *imagem = fase->inimImg[6];
  
    for (short i = 0; i < quantidade; i++){

        // Gera posições aleatórias dentro dos limites da tela
        unsigned short x = fase->largura - larguraEntidade + 20;
        unsigned short y = aleat(200 , fase->altura - 200);
    
        // Cria a entidade na posição gerada
        entidade *entidade = criarEntidade(
            larguraEntidade, alturaEntidade, velocidade, x, y,
            largura, altura, dano, dispTemp, face,
            imagem, tipoMov, atirar, false, hp
        );
        
        entidade->tipoMov = 4;

        if (entidade) {
            // Adiciona a nova entidade ao vetor
            entidade->x = spawn;
            entidade->y = fase->altura;
            entidade->spawn = true;
            entidade->inimigo = true;
            fase->chefeVivo = 1;
            redimencionarVetor(fase, quantidade);
            fase->entidades[fase->numEntidades - 1] = entidade;
            anexarImagem(fase->entidades[fase->numEntidades - 1], 4, 0, 0, 10, 
                                                             280, 500, 1, 6, 2);
            fase->chefaoIdx = fase->numEntidades - 1;
        } else {
            fprintf(stderr, "Erro ao criar a entidade.\n");
        }
    }
  } else // Entidade boost
  if(tipoEntidade == 7){
    
    // Variaveis especificas da entidade
    larguraEntidade = 70;
    alturaEntidade = 70;
    velocidade = aleat(1, 3);
    dano = 0;
    dispTemp = 0;
    tipoMov = 5;
    hp = 3000;
    bool criado = false;
    ALLEGRO_BITMAP *imagem = fase->inimImg[7];
  
    for (short i = 0; i < quantidade; i++){

        // Gera posições aleatórias dentro dos limites da tela
        unsigned short x = fase->largura;
        unsigned short y = aleat(larguraEntidade , fase->altura - larguraEntidade);
    
        // Cria a entidade na posição gerada
        entidade *entidade = criarEntidade(
            larguraEntidade, alturaEntidade, velocidade, x, y,
            largura, altura, dano, dispTemp, face,
            imagem, tipoMov, false, false, hp
        );

        if (entidade) {
            // Adiciona a nova entidade ao vetor
            entidade->inimigo = true;
            redimencionarVetor(fase, quantidade);
            fase->entidades[fase->numEntidades - 1] = entidade;
            anexarImagem(fase->entidades[fase->numEntidades - 1], 8, 0, 0, 9, 80, 80, 1, 1, 1);
        } else {
            fprintf(stderr, "Erro ao criar a entidade.\n");
        }
    }
  }
  }
  }
  }
}

    
// Criação da fase 1
Fase *criarFase1(ALLEGRO_BITMAP **backgrounds, short numBackgrounds, 
                 ALLEGRO_BITMAP **projeteisImg, short numImagens, 
                 short largInit, short altInit, Projetil **primeiro, 
                 ALLEGRO_FONT *fonte, ALLEGRO_BITMAP **inimImg, 
                             short numInimImg, Som **musicas, short numMusicas){

  Fase *fase = (Fase*) malloc(sizeof(Fase));
  
  fase->numEntidades = 1;
  fase->numEntidadesAux = fase->numEntidades;
  fase->numProjImg = numImagens;
  fase->numBackgrounds = 4;
  fase->tempo = 500;
  fase->primeiro = primeiro;
  fase->largura = largInit;
  fase->altura = altInit;
  fase->fonte = fonte;
  fase->chefaoIdx = 0;
  fase->faseNumero = 1;
  fase->chefeVivo = 0;
  fase->numMusicas = numMusicas;

  fase->backgrounds = 
                    (Background**) malloc(numBackgrounds * sizeof(Background*));
    
  // Nuvens
  fase->backgrounds[0] = criarBackground
                                     (0, 0, al_get_bitmap_width(backgrounds[0]), 
          al_get_bitmap_height(backgrounds[0]), 1 , 0, true, 1, backgrounds[0]);

  // Montanhas
  fase->backgrounds[1] = criarBackground
  (0, 300, al_get_bitmap_width(backgrounds[1]), 
          al_get_bitmap_height(backgrounds[1]), 3 , 0, true, 1, backgrounds[1]);

  // Vegetação 1
  fase->backgrounds[2] = criarBackground
  (0, altInit - 340, al_get_bitmap_width(backgrounds[2]), 
          al_get_bitmap_height(backgrounds[2]), 5 , 0, true, 1, backgrounds[2]);
    
  // Vegetação 2
  fase->backgrounds[3] = criarBackground
    (0, altInit - al_get_bitmap_height(backgrounds[3]), 
    al_get_bitmap_width(backgrounds[3]), al_get_bitmap_height(backgrounds[3]), 
                                               10 , 0, true, 1, backgrounds[3]);


    // Imagens dos projeteis
    fase->projImg = projeteisImg;
    fase->inimImg = inimImg;

    // Criação de entidades
    fase->entidades = 
                    (entidade**) malloc(fase->numEntidades * sizeof(entidade*));

    // Vetor de musicas
    fase->musicas = musicas;

    // Largura e altura do personagem principal
    unsigned short entLarg = 100;
    unsigned short entAlt = 50;

    /*(Largura, Altura, Velocidade, Pos X, Pos Y, Largura Tela, Altura Tela, dano, face, imagem da sprite, tipo de movimento, atira ou nao */

    // Personagem principal
    ALLEGRO_BITMAP *personagem = al_load_bitmap("Sprites/personagem1.png");
    fase->entidades[0] = criarEntidade(
    entLarg, entAlt, 7, 20 + entLarg/2 , altInit / 2, largInit, altInit, 50, 25, 
                                      false, personagem, 50, false, false, 100);
    
    anexarImagem(fase->entidades[0], 4, 0, 0, 5, 149, 150, 1, 0, 1);


    return fase;
}






Fase *criarFase2(ALLEGRO_BITMAP **backgrounds, short numBackgrounds, 
                 ALLEGRO_BITMAP **projeteisImg, short numImagens, 
                 short largInit, short altInit, Projetil **primeiro, 
                 ALLEGRO_FONT *fonte, ALLEGRO_BITMAP **inimImg, 
                             short numInimImg, Som **musicas, short numMusicas){

  Fase *fase = (Fase*) malloc(sizeof(Fase));
  
  fase->numEntidades = 1;
  fase->numEntidadesAux = fase->numEntidades;
  fase->numProjImg = numImagens;
  fase->numBackgrounds = numBackgrounds;
  fase->tempo = 500;
  fase->primeiro = primeiro;
  fase->largura = largInit;
  fase->altura = altInit;
  fase->fonte = fonte;
  fase->chefaoIdx = 0;
  fase->faseNumero = 2;
  fase->chefeVivo = 0;
   fase->numMusicas = numMusicas;


    // Inicializa os backgrounds da fase
    fase->backgrounds = 
                    (Background**) malloc(numBackgrounds * sizeof(Background*));

    // Nuvens
    fase->backgrounds[0] = criarBackground
                                     (0, 0, al_get_bitmap_width(backgrounds[0]), 
          al_get_bitmap_height(backgrounds[0]), 1 , 0, true, 1, backgrounds[0]);

    fase->backgrounds[1] = criarBackground
                                     (0, 300, al_get_bitmap_width(backgrounds[1]), 
          al_get_bitmap_height(backgrounds[1]), 2 , 0, true, 1, backgrounds[1]);

    fase->backgrounds[2] = criarBackground
                                     (0, 300, al_get_bitmap_width(backgrounds[2]), 
          al_get_bitmap_height(backgrounds[2]), 3 , 0, true, 1, backgrounds[2]);

    fase->backgrounds[3] = criarBackground
                                     (0, 300, al_get_bitmap_width(backgrounds[3]), 
          al_get_bitmap_height(backgrounds[3]), 7 , 0, true, 1, backgrounds[3]);


    // Imagens dos projeteis
    fase->projImg = projeteisImg;
    fase->inimImg = inimImg;

    // Criação de entidades
    fase->entidades = 
                    (entidade**) malloc(fase->numEntidades * sizeof(entidade*));

    // Vetor de musicas
    fase->musicas = musicas;

    // Largura e altura do personagem principal
    unsigned short entLarg = 100;
    unsigned short entAlt = 50;

    /*(Largura, Altura, Velocidade, Pos X, Pos Y, Largura Tela, Altura Tela, dano, face, imagem da sprite, tipo de movimento, atira ou nao */

    // Personagem principal
    ALLEGRO_BITMAP *personagem = al_load_bitmap("Sprites/personagem1.png");
    fase->entidades[0] = criarEntidade(
    entLarg, entAlt, 7, 20 + entLarg/2 , altInit / 2, largInit, altInit, 15, 25, 
                                      false, personagem, 50, false, false, 100);
    
    anexarImagem(fase->entidades[0], 4, 0, 0, 5, 149, 150, 1, 0, 1);


    return fase;
}

// Atualiza as imagens do background
void atualizarBackground(Fase *fase){
  
  // Movimentação do background
  for (short i = 0; i < fase->numBackgrounds; i++){
  fase->backgrounds[i]->x -= fase->backgrounds[i]->velocidadeX;

  // Reseta a imagem ao ser menor que a largura
  if(fase->backgrounds[i]->x + fase->backgrounds[i]->largura <= fase->largura)
    fase->backgrounds[i]->x = 0;
  }
}

// Desenha o backgroud na tela
void desenharBackground(Fase *fase){

  // Faz para todas as imagens do vetor
  for (short i = 0; i < fase->numBackgrounds; i++){
  al_draw_bitmap(fase->backgrounds[i]->imagem, fase->backgrounds[i]->x, 
                                                    fase->backgrounds[i]->y, 0);

  if(fase->backgrounds[i]->x + fase->backgrounds[i]->largura < fase->largura)
    al_draw_bitmap(fase->backgrounds[i]->imagem, fase->backgrounds[i]->x + 
                     fase->backgrounds[i]->largura, fase->backgrounds[i]->y, 0);
  }

}

void destruirBackground(Background *background) {
    
    if (background == NULL) {
        return;
    }

    // Libera o bitmap, se existir
    if (background->imagem != NULL) {
        al_destroy_bitmap(background->imagem);
        background->imagem = NULL;
    }
    // Libera o ponteiro para o Background
    free(background);
}

void destruirProjeteis(Projetil **primeiro) {
    if (primeiro == NULL) {
        return;
    }

    Projetil *atual = *primeiro;
    while (atual != NULL) {
        Projetil *proximo = atual->proximo;

        free(atual);  // Liberar a memória do projétil
        atual = proximo;
    }

    *primeiro = NULL;  // Garantir que a lista está vazia
}

void destruirFase(Fase *fase) {
    if (fase == NULL) {
        return;
    }

    // Liberar os backgrounds
    if (fase->backgrounds != NULL) {
        for (short i = 0; i < fase->numBackgrounds; i++) {
            if (fase->backgrounds[i] != NULL) {
                destruirBackground(fase->backgrounds[i]);
            }
        }
        free(fase->backgrounds); 
    }

    destruirProjeteis(fase->primeiro);
      
    free(fase);
}

void desenharEntidades(entidade **entidades, short numEntidades){

  for (short i = 0; i < numEntidades; i++) {
    if (entidades[i]->hp > 0){
      al_draw_filled_rectangle(
          entidades[i]->x - entidades[i]->largura / 2,
          entidades[i]->y - entidades[i]->altura / 2,
          entidades[i]->x + entidades[i]->largura / 2,
          entidades[i]->y + entidades[i]->altura / 2,
          al_map_rgb(255, 255, 0)
          );
    }
    else destruirEntidade(entidades[i]);
  }
}

void projetilTeleguiado(Fase *fase, Projetil *projetil) {
    
    short deltaX = fase->entidades[0]->x - projetil->x;
    short deltaY = fase->entidades[0]->y - projetil->y;

    // Acelerando o movimento, ajustando a direção em X e Y
    unsigned char velocidade = projetil->velocidade;

    // Normalizando o movimento para evitar que a entidade se mova mais rápido diagonalmente
    float distancia = sqrt(deltaX * deltaX + deltaY * deltaY);

    

    if (distancia != 0) {
        // Movendo a entidade na direção de X e Y
        projetil->x += (deltaX / distancia) * velocidade;
        projetil->y += (deltaY / distancia) * velocidade;
    }
}

void projetilEstatico(Fase *fase, Projetil *projetil) {
  
    projetil->y = fase->altura/2;

    // Acelerando o movimento, ajustando a direção em X e Y
    projetil->velocidade = 0;

}




bool moverProjetil(Fase *fase, Projetil *projetil, unsigned short larguraTela){
    
  // Caso mais comum: tipo não é 3.
  if (projetil->tipo <= 2 || projetil->tipo == 5 || projetil->tipo == 8 || projetil->tipo == 9 || projetil->tipo == 10){
    if (projetil->trajetoria) {
      projetil->x -= projetil->velocidade;
    } else if (projetil->velocidade != 0) {
      projetil->x += projetil->velocidade;
    }
    if (projetil->x - projetil->largura / 2 < 0 || projetil->x > larguraTela){
      destruirProjetil(fase->primeiro, projetil);
      return false;
    }
    return true;
  }
  
  // Cuida do tipo 4 de projetil, e se esse projetil for do dragão, reseta ele
  if (projetil->tipo == 4 || projetil->tipo == 6 ){
      
      if (projetil->tipo == 4 || projetil->tipo == 6)
        projetilTeleguiado(fase, projetil);
  

      if (fase->chefaoIdx != 0 && projetil->tipo <= 4)
        resetarEntidade(fase->entidades[fase->chefaoIdx]);

      if (projetil->x - projetil->largura / 2 < 0 || projetil->x > larguraTela){
        destruirProjetil(fase->primeiro, projetil);
        return false;
      } return true;
  }

  if (projetil->tipo == 7){
        projetilEstatico(fase, projetil);
      }
  

  // Administra o timer do sopro do dragão
  if (fase->chefaoIdx != 0 && fase->entidades[fase->chefaoIdx]->hp < 1){
    destruirProjetil(fase->primeiro, projetil);
    fase->chefaoIdx = 0;
    return false;
  }

  
  // Administra projetil tipo 3 explusivo do chefao
  if (projetil->timer == 0 && fase->chefaoIdx != 0 && projetil->tipo != 4
      && fase->faseNumero == 1) {
    resetarEntidade(fase->entidades[fase->chefaoIdx]);
    destruirProjetil(fase->primeiro, projetil);
    soproDragao(fase, false);
    return false;
  }
    
  if (projetil->timer == 0 && fase->chefaoIdx != 0 && projetil->tipo != 4
      && fase->faseNumero == 2) {
   
    resetarEntidade(fase->entidades[fase->chefaoIdx]);
    destruirProjetil(fase->primeiro, projetil);
    return false;
  }
  
  if (projetil->tipo == 3 && fase->chefeVivo == 0)
    destruirProjetil(fase->primeiro, projetil);

  projetil->timer -= 1;
  

  return true;
}

bool moverTodosProjeteis(Fase *fase, unsigned short larguraTela){

  Projetil *atual = *fase->primeiro;
  Projetil *proximo = NULL;

  while (atual != NULL) {
    proximo = atual->proximo;  

    if (!moverProjetil(fase, atual, larguraTela)) {
    }
    atual = proximo;  
  }

  return true;  
}


void atirarProjetil(Fase *fase, ALLEGRO_BITMAP **imagem) { 
    
  for (int i = 0; i < fase->numEntidades; i++) {

    // Verifica se a entidade está viva para atirar
    if (fase->entidades[i] == NULL || fase->entidades[i]->teclado == NULL || 
                                                   fase->entidades[i]->hp <= 0){
      continue;
    }

    short pontoDisparoX = 
         fase->entidades[i]->x + fase->entidades[i]->largura / 2 + PROJ_TAM + 5;

    short pontoDisparoX2 = 
         fase->entidades[i]->x - fase->entidades[i]->largura / 2 - PROJ_TAM - 5;

    unsigned char velocidade = fase->entidades[i]->velocidade + 5;
    unsigned char dano = fase->entidades[i]->dano;
    bool face = fase->entidades[i]->face;
    char projetilTipo = fase->entidades[i]->projetilTipo;

    if (i != 0 && i == fase->chefaoIdx && fase->entidades[i]->x >= fase->entidades[i]->xInit + 20){
      break;
    }
        

    Projetil *projetil;

    // Timer de disparo / direção
    if (fase->entidades[i]->teclado->shot == 1 && 
                   fase->entidades[i]->dispUlt >= fase->entidades[i]->dispTemp){
      
      for (int j = 0; j < fase->entidades[i]->qtdProjeteis; j++){
      // Verifica para qual lado está o projetil    
      if (face == false) {
        // Função que cria um projetil e o incere na lista ligada
        projetil = criarProjetil(
        fase->primeiro, pontoDisparoX, fase->entidades[i]->y + j, velocidade, dano,
                 PROJ_TAM, PROJ_TAM, face, &imagem[projetilTipo], projetilTipo);
        
        // Função que seta a imagem / caracteristicas em cada tipo de projetil
        anexarProjetil(projetil, fase->entidades[i], projetilTipo, face);
      } else {
          projetil = criarProjetil(
          fase->primeiro, pontoDisparoX2, fase->entidades[i]->y, velocidade, 
            dano, PROJ_TAM, PROJ_TAM,face, &imagem[projetilTipo], projetilTipo);

          anexarProjetil(projetil, fase->entidades[i], projetilTipo, face);
        }

        somProjetil(fase, projetilTipo, 1);
      }
        // Resetando o timer de disparo
        fase->entidades[i]->dispUlt = 0; 

        // Administra o tipo especial 1
        if (fase->entidades[i]->especialTipo == 1 && fase->entidades[i]->inimigo == false){
          fase->entidades[i]->dispTemp /= 2;
          fase->entidades[i]->dano *= 3;
          fase->entidades[i]->especialTimer = fase->tempo/60 + 5;
          fase->entidades[i]->velocidade += 2;
          fase->entidades[i]->especialTipo = 0;
          fase->entidades[i]->dano *= 2;
          fase->entidades[i]->projetilTipo = 9;
        }
        // Administra o tipo especial 2
        if (fase->entidades[i]->especialTipo == 2 && fase->entidades[i]->inimigo == false){
          fase->entidades[i]->especialTimer = fase->tempo/60 + 4;
          fase->entidades[i]->especialTipo = 0;
          fase->entidades[i]->dano = 300;
          fase->entidades[i]->projetilTipo = 10;
        }

        // Reseta o especial
        if (fase->entidades[i]->especialTipo == 0 && fase->tempo/60 > fase->entidades[i]->especialTimer 
                      && fase->entidades[i]->inimigo == false){
          resetarEntidade(fase->entidades[i]);
          fase->entidades[i]->qtdProjeteis = 1;
    
        
        }
  
        } else {

            fase->entidades[i]->dispUlt += 1;
          }
          
      if (fase->entidades[i]->dispUlt == 10 && fase->entidades[i]->tipoMov != 3)
        fase->entidades[i]->frameColuna = 1;

    }
}





// Funções aleatorias de controle do chefão
void aleatorizarChefao(Fase *fase){

  // Só funciona se o chefão ja foi criado e estiver vivo
  if (fase->chefaoIdx != 0 && fase->entidades[fase->chefaoIdx]->hp > 0
                                             && fase->faseNumero == 1){
                                       
    
    // Variavel aleatoria que controla quando o especial do dragão é ativado
    int aleatorio;

    // Se o chefão tiver mais de 2000 de hp, ele fica normal
    if (fase->entidades[fase->chefaoIdx]->hp > 2000)
      aleatorio = aleat(0, 1000);

    // Se tiver menos de 2000 ele fica mais dificil
    if (fase->entidades[fase->chefaoIdx]->hp < 2000){
      aleatorio = aleat(0, 200);
      fase->entidades[fase->chefaoIdx]->velocidade = 4;
      fase->entidades[fase->chefaoIdx]->dispTemp = 50;
    }

    // Fica alternando entre os projeteis tipo 2 e 3
    if (aleatorio == 100){
      fase->entidades[fase->chefaoIdx]->projetilTipo = 3;
    } 
    
    // Alterna entre projeteis tipo 2 e 4
    if (aleatorio < 5){
      fase->entidades[fase->chefaoIdx]->projetilTipo = 4;
    }

  } 

  if (fase->chefaoIdx != 0 && fase->entidades[fase->chefaoIdx]->hp > 0 && 
                                       fase->entidades[fase->chefaoIdx]->x <= 
                                       fase->entidades[fase->chefaoIdx]->xInit + 10
                                       && fase->faseNumero == 2){

    int aleatorio;

    if (fase->entidades[fase->chefaoIdx]->hp > 2000)
      aleatorio = aleat(0, 1000);

    if (fase->entidades[fase->chefaoIdx]->hp < 2000){
      aleatorio = aleat(0, 120);
      fase->entidades[fase->chefaoIdx]->velocidade = 4;
      fase->entidades[fase->chefaoIdx]->dispTemp = 50;
    }

    if (aleatorio < 100){
      fase->entidades[fase->chefaoIdx]->projetilTipo = 8;
    } 

    if (aleatorio <= 40){
      fase->entidades[fase->chefaoIdx]->projetilTipo = 7;
    } 

    if (aleatorio > 100){
      fase->entidades[fase->chefaoIdx]->projetilTipo = 6;
    } 


  }

}
// Gerencia os eventos de tempo
 void gerenciarEventos(Fase *fase, int tempo){

  if (fase->faseNumero == 1)
    switch (tempo) {

        case 200:
        spawnarEntidade(fase, 2, DEMONIO);
        spawnarEntidade(fase, 1, BOOST);
       
      
        break;
        case 800:
        spawnarEntidade(fase, 1, PIRANHA);
        spawnarEntidade(fase, 1, BOOST);
        break;
        case 1200:
        spawnarEntidade(fase, 2, GOLEM);
        destruirEntidadesExcetoPrimeira(fase->entidades, fase->numEntidades);
        break;
        case 1600:
        spawnarEntidade(fase, 2, DEMONIO);
        spawnarEntidade(fase, 1, PIRANHA);
        spawnarEntidade(fase, 1, BOOST);
        break;
        case 2200:
        spawnarEntidade(fase, 2, DEMONIO);
        break;
        case 2800:
        spawnarEntidade(fase, 2, DEMONIO);
        break;
        case 3500:
        spawnarEntidade(fase, 2, DEMONIO);
        spawnarEntidade(fase, 1, BOOST);
        break;

        case 4000:
        spawnDragao(fase, 1);
        destruirEntidadesExcetoPrimeira(fase->entidades, fase->numEntidades);
        break;

        case 4900:
        spawnarEntidade(fase, 1, DRAGAO);
        spawnarEntidade(fase, 1, BOOST);
        asasDragao(fase, 1);
        break;

        case 5500:
        spawnarEntidade(fase, 2, BOOST);
        break;

        case 6000:
        spawnarEntidade(fase, 2, BOOST);
        break;

        case 7000:
        spawnarEntidade(fase, 2, BOOST);
        break;

        case 8000:
        spawnarEntidade(fase, 2, BOOST);
        break;

    }

    if (fase->faseNumero == 2)
    switch (tempo) {

      case 200:
        spawnarEntidade(fase, 2, SOMBRA1);
        spawnarEntidade(fase, 1, BOOST);

        break;

        case 800:
        spawnarEntidade(fase, 1, SOMBRA1);
        spawnarEntidade(fase, 1, BOOST);
        break;
        case 1200:
        spawnarEntidade(fase, 2, SOMBRA2);
        break;
        case 1600:
        spawnarEntidade(fase, 2, SOMBRA1);
        spawnarEntidade(fase, 1, SOMBRA2);
        spawnarEntidade(fase, 1, BOOST);
        break;
        case 2200:
        spawnarEntidade(fase, 2, SOMBRA2);
        break;
        case 2800:
        spawnarEntidade(fase, 2, SOMBRA2);
        break;
        case 3500:
        destruirEntidadesExcetoPrimeira(fase->entidades, fase->numEntidades);
        spawnarEntidade(fase, 1, BOOST);
        break;

        case 4900:
        spawnarEntidade(fase, 1, CHEFE2);
        spawnarEntidade(fase, 1, BOOST);
  
        break;

        case 5500:
        spawnarEntidade(fase, 2, BOOST);
        break;

        case 6000:
        spawnarEntidade(fase, 2, BOOST);
        break;

        case 7000:
        spawnarEntidade(fase, 2, BOOST);
        break;

        case 8000:
        spawnarEntidade(fase, 2, BOOST);
        break;

    }
}


// Reduz o vetor quando a entidade morre
void atualizarVetor(Fase *fase){
  for (short i = 1; i < fase->numEntidades; i++){
    if (fase->entidades[i]->hp < 1){
      // Destrói a entidade
      destruirEntidade(fase->entidades[i]);

      // Move as entidades no vetor para manter ordenado
      for (short j = i; j < fase->numEntidades - 1; j++){
        fase->entidades[j] = fase->entidades[j + 1];
      }
      
      // Reduz o tamanho do vetor
      fase->numEntidades--;
      // Realoca o tamanho do vetor
      fase->entidades = (entidade**) 
               realloc(fase->entidades, fase->numEntidades * sizeof(entidade*));
      
      if (!fase->entidades && fase->numEntidades > 0){
        fprintf(stderr, "Erro ao redimensionar o vetor de entidades.\n");
        return;
      }

      if (fase->chefaoIdx != 0){
        fase->chefaoIdx--;

      if (fase->chefaoIdx == 0)
        fase->chefeVivo == false;
        fase->chefaoIdx++;
      }

      // Se a entidade morreru, tem que fazer pra ela denovo
      i--;
    }
  }
}

// Gerencia todas as funções da fase
short gerenciarFase(Fase *fase, ALLEGRO_TIMER *timer, unsigned short largInit, 
                                                        unsigned short altInit){

  fase->tempo = al_get_timer_count(timer);
  
  gerenciarEventos(fase, fase->tempo);

  atualizarBackground(fase);
  desenharBackground(fase);

  atirarProjetil(fase, fase->projImg);

  moverAux(fase->entidades, fase->numEntidades, largInit, altInit);
  movimentacaoAleatoria(fase->entidades, fase->numEntidades, largInit, altInit);
  //desenharEntidades(fase->entidades, fase->numEntidades);
  
  moverTodosProjeteis(fase, largInit);
  
  colisaoProjeteis(fase->primeiro, fase->entidades, fase->numEntidades);

  desenharEntidade(fase->entidades, fase->numEntidades);
  atualizarVetor(fase);
  desenharProjetil(fase->primeiro);
  aleatorizarChefao(fase);
  //desenharProjeteis(fase->primeiro);
 
  atualizarEntidade(fase->entidades, fase->numEntidades);
  atualizarProjetil(fase->primeiro);

  char buffer[50]; 
  sprintf(buffer, "Jogador: %d", fase->entidades[0]->hp);
  al_draw_text(fase->fonte, al_map_rgb(255, 255, 255), largInit/2, 50, ALLEGRO_ALIGN_CENTER, buffer);
  if (fase->chefaoIdx != 0){
  sprintf(buffer, "Chefe: %d", fase->entidades[fase->chefaoIdx]->hp);
  al_draw_text(fase->fonte, al_map_rgb(255, 255, 255), largInit/2, 100, ALLEGRO_ALIGN_CENTER, buffer);
  }
  sprintf(buffer, "Timer: %d", fase->tempo/60 );
  al_draw_text(fase->fonte, al_map_rgb(255, 255, 255), 200, 100, ALLEGRO_ALIGN_RIGHT, buffer);



  // Se o chefão foi morto, gerenciar fases retorna true e pausa todas as musicas
  if (fase->chefaoIdx != 0 && fase->entidades[fase->chefaoIdx]->hp < 10){
    pausarMusicas(fase);
  return 1;
  }

  if (fase->entidades[0]->hp < 1){
     pausarMusicas(fase);
    return 2;
    }

  return 0;

}

entidade *retornarEntidade(Fase *fase){

  return fase->entidades[0];
}

