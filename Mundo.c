#include <allegro5/allegro5.h>														
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "Entidades.h"
#include "Objetos.h"
#include "Projetil.h"

#define FPS 60

const char* introText[] = {
    "Nas profundezas insondaveis do Multiverso, onde as sombras",
    "se estendem além dos limites da visão e o tempo se curva",
    "como uma serpente faminta, uma força sombria desperta.",
    "",
    "Seu nome, Morgra, uma bruxa imortal cuja sede de poder",
    "não conhece limites. Por seculos, ela viajou entre dimensões,",
    "criando monstros para subjugar reinos esquecidos.",
    "",
    "Mas sua criacao mais ambiciosa, uma criatura de asas negras",
    "e olhos vazios chamada Corvo, se tornou algo inesperado...",
    "não uma arma, mas uma alma livre, consciente de seu destino.",
    "",
    "Agora, o Corvo deve atravessar dimensões hostis",
    "em uma luta desesperada por liberdade. Guiado por uma",
    "voz misteriosa chamada Ashira, ele deve resistir à escuridão",
    "que ameaca consumi-lo a cada passo.",
    "",
    "Mas enquanto Kael avança Morgra observa.",
    "E ela está decidida a não perder sua criação...",
    "",
    "A liberdade esta proxima ou sera ele apenas mais uma",
    "arma nas mãos da Bruxa Eterna?"
};

const char* introText2[] = {
    "..."
  
};
const char* introText3[] = {
    "Após uma jornada dolorosa por dimensões hostis,",
    "o Corvo finalmente chegou à fortaleza de Morgra,",
    "a bruxa imortal. Ela, envolta em uma aura de poder negro,",
    "o esperava com um sorriso frio.",
    "",
    "\"Você não pode me derrotar, Corvo\", ela disse, sua voz",
    "ecoando. \"Sou eterna.\"",
    "",
    "Mas o Corvo não respondeu. Ele havia aprendido a lutar",
    "pela sua liberdade. Com uma explosão de força, ele enfrentou",
    "a bruxa, rompendo suas ilusões e resistindo à sua magia.",
    "Não era mais uma criatura sem alma, mas uma alma livre.",
    "",
    "No auge da batalha, Morgra tentou invocar todo o seu poder",
    "para consumi-lo, mas o Corvo, com suas asas negras brilhando",
    "intensamente, atravessou seu coração com um golpe mortal.",
    "A bruxa, a fonte da escuridão, foi destruída, sua magia",
    "desvanecendo-se.",
    "",
    "Com a queda de Morgra, o Corvo sentiu a verdadeira liberdade.",
    "Ele não era mais uma arma, mas um ser independente.",
    "O multiverso agora estava aberto para ele, livre das correntes",
    "da bruxa.",
    "",
    "Ele voou para o desconhecido, sem limites, finalmente livre."
};

const char* introText4[] = {
    "Após uma longa jornada, o Corvo chegou à fortaleza de Morgra.",
    "Ele foi cercado pelos monstros da bruxa, sem forças.",
    "",
    "Ele lutou bravamente, mas os inimigos eram demais, e Morgra",
    "o enfraquecia a cada golpe.",
    "",
    "No final, o Corvo foi derrotado. O líder dos monstros",
    "o derrubou com um golpe fatal.",
    "",
    "Morgra observou, sorrindo. Com a morte do Corvo",
    "seu controle sobre o multiverso permanecia firme",
    "e a liberdade do Corvo, perdida para sempre."
};


// Atualiza o display
void atualizarDisplay(unsigned char red, unsigned char green, 
    unsigned char blue){
  al_flip_display();
  al_clear_to_color(al_map_rgb(red, green, blue));

}

// Pega a maior resolução disponivel no computador
ALLEGRO_DISPLAY_MODE displayRes(){
ALLEGRO_DISPLAY_MODE modo;
ALLEGRO_DISPLAY_MODE melhor_modo;

bool encontrado = false;

// Pega os modos disponiveis
int modos = al_get_num_display_modes();

// Retorna esse modo para a variavel melho_modo
al_get_display_mode(0, &melhor_modo);

return melhor_modo;

}

// Prototipo de função para atualização do display
void atualizarPosicao(entidade* entidades, short numEntidades, 
    unsigned short larguraAtual, unsigned short alturaAtual,
    unsigned short largInit, unsigned short altInit){

  float Largura = (float)larguraAtual / largInit;
  float Altura = (float)alturaAtual / altInit;

  for (short i = 0; i < numEntidades; i++) {   
    // Atualiza a posição em x e y com as proporções
    entidades[i].x *= Largura;
    entidades[i].y *= Altura;

    // Garante que as entidades não saiam dos limites do display
    if (entidades[i].y < entidades[i].altura / 2) {
      entidades[i].y = entidades[i].altura / 2;  
    } 

    if (entidades[i].y > alturaAtual - entidades[i].altura / 2) {
      entidades[i].y = alturaAtual - entidades[i].altura / 2; 
    } 

    if (entidades[i].x < entidades[i].largura / 2) {
      entidades[i].x = entidades[i].largura / 2;          
    } 

    if (entidades[i].x > larguraAtual - entidades[i].largura / 2) {
      entidades[i].x = larguraAtual - entidades[i].largura / 2;  
    } 
  }
}


int main(){

  // Inicia os addons do allegro
  al_init();				
  al_install_keyboard();
  al_init_primitives_addon();
  al_init_image_addon();
  al_init_font_addon();
  al_install_audio();
  al_init_acodec_addon();
  al_init_ttf_addon();


  // Reserva 5 samples de musica
  if (!al_reserve_samples(5)) {
    fprintf(stderr, "Falha ao reservar amostras de áudio.\n");
    return -1;
}

  // ---------------------------- Musicas ----------------------------------- //

  short numMusicas = 6; // Tamanho do vetor de musicas

  Som **musicas = (Som **)malloc(numMusicas * sizeof(Som *));

    musicas[0] = carregarMusica("sons/white.ogg", 0.7); // Fundo
    musicas[1] = carregarMusica("sons/dragao.ogg", 1); // Spawn do dragão
    musicas[2] = carregarMusica("sons/sopro.ogg", 0.7); // Sopro do dragão
    musicas[3] = carregarMusica("sons/asasDragao.ogg", 0.9); // Asas do dragão
    musicas[4] = carregarMusica("sons/projetildragao.ogg", 0.1); // Projetil 
    musicas[5] = carregarMusica("sons/spell.ogg", 0.1); // Projetil Jogador

  // ------------------------------------------------------------------------ //

  // ---------------------------- Background -------------------------------- // 

  // Quantidade de imagens
  short numBackgrounds = 4;

  ALLEGRO_BITMAP **backgrounds = 
           (ALLEGRO_BITMAP **)malloc(numBackgrounds * sizeof(ALLEGRO_BITMAP *));
  
  // Nuvens Fase 1
   backgrounds[0] = al_load_bitmap("Backgrounds/1.png");
  if (backgrounds[0] == NULL) {
    return -1;  
  }

  // Montanhas Fase 1
  backgrounds[1] = al_load_bitmap("Backgrounds/2.png");
  if (backgrounds[1] == NULL) {
    return -1;  
  }
  
  // Vegetaçaõ 2 Fase 1
  backgrounds[2] = al_load_bitmap("Backgrounds/3.png");
  if (backgrounds[2] == NULL) {
    return -1;   
  }

  // Vegetação 1 Fase 1
  backgrounds[3] = al_load_bitmap("Backgrounds/4.png");
  if (backgrounds[3] == NULL) {
    return -1; 
  }

  short numBackgrounds2 = 4;

  ALLEGRO_BITMAP **backgrounds2 = 
           (ALLEGRO_BITMAP **)malloc(numBackgrounds2 * sizeof(ALLEGRO_BITMAP *));
  
  // Nuvens Fase 1
   backgrounds2[0] = al_load_bitmap("Backgrounds/fase2_4.png");
   backgrounds2[1] = al_load_bitmap("Backgrounds/fase2_3.png");
   backgrounds2[2] = al_load_bitmap("Backgrounds/fase2_2.png");
   backgrounds2[3] = al_load_bitmap("Backgrounds/fase2_1.png");
   
  // -------------------------------------------------------------------------//
  // -------------------------- Sprites Entidades ----------------------------//

  short numIniImg = 8; // Tamanho do vetor de sprites

  ALLEGRO_BITMAP **inimImg = 
                (ALLEGRO_BITMAP **)malloc(numIniImg * sizeof(ALLEGRO_BITMAP *));  
  
  // Demonio Vermelho
  ALLEGRO_BITMAP *imagem = al_load_bitmap("Sprites/inimigo.png");
  if (!imagem) {
    fprintf(stderr, "Erro ao carregar a imagem.\n");
    return 1;
  }

  // Piranha demoniaca
  ALLEGRO_BITMAP *imagem1 = al_load_bitmap("Sprites/inimigo2.png");
  if (!imagem1) {
    fprintf(stderr, "Erro ao carregar a imagem.\n");
    return 1;
  }
  
  // Golem Demoniaco
  ALLEGRO_BITMAP *imagem2 = al_load_bitmap("Sprites/inimigo3.png");
  if (!imagem2) {
    fprintf(stderr, "Erro ao carregar a imagem.\n");
    return 1;
  }

  // Chefão
  ALLEGRO_BITMAP *imagem3 = al_load_bitmap("Sprites/inimigo4.png");
  if (!imagem3) {
    fprintf(stderr, "Erro ao carregar a imagem.\n");
    return 1;
  }

  // Demonio das sombras 
  ALLEGRO_BITMAP *imagem4 = al_load_bitmap("Sprites/inimigo5.png");
  if (!imagem4) {
    fprintf(stderr, "Erro ao carregar a imagem.\n");
    return 1;
  }
  
  ALLEGRO_BITMAP *imagem5 = al_load_bitmap("Sprites/inimigo6.png");
  if (!imagem5) {
    fprintf(stderr, "Erro ao carregar a imagem.\n");
    return 1;
  }

  ALLEGRO_BITMAP *imagem6 = al_load_bitmap("Sprites/inimigo7.png");
  if (!imagem6) {
    fprintf(stderr, "Erro ao carregar a imagem.\n");
    return 1;
  }

  ALLEGRO_BITMAP *imagem7 = al_load_bitmap("Sprites/boost.png");
  if (!imagem7) {
    fprintf(stderr, "Erro ao carregar a imagem.\n");
    return 1;
  }

  // Coloca as imagens no vetor em ordem
  inimImg[0] = imagem;
  inimImg[1] = imagem1;
  inimImg[2] = imagem2;
  inimImg[3] = imagem3;
  inimImg[4] = imagem4;
  inimImg[5] = imagem5;
  inimImg[6] = imagem6;
  inimImg[7] = imagem7;


   
  //--------------------------------------------------------------------------//
  // -------------------------- Vetor de Projeteis ---------------------------//
  
  // Quantidade de imagens de projeteis
  short numImagens = 11;

  // Vetor de projeteis
  ALLEGRO_BITMAP **projeteisImg = 
               (ALLEGRO_BITMAP **)malloc(numImagens * sizeof(ALLEGRO_BITMAP *));

  projeteisImg[0] = al_load_bitmap("Projeteis/projetil.png"); // Azul
  projeteisImg[1] = al_load_bitmap("Projeteis/projetilInimigo1.png");// Vermelho
  projeteisImg[2] = al_load_bitmap("Projeteis/projetil3.png"); // Dragão
  projeteisImg[3] = al_load_bitmap("Projeteis/sopro.png"); // Sopro dragão
  projeteisImg[4] = al_load_bitmap("Projeteis/teleguiado.png"); // Teleguiado
  projeteisImg[5] = al_load_bitmap("Projeteis/sombra1.png"); // 
  projeteisImg[6] = al_load_bitmap("Projeteis/sombra2.png"); // 
  projeteisImg[7] = al_load_bitmap("Projeteis/raio.png"); // 
  projeteisImg[8] = al_load_bitmap("Projeteis/sombra3.png"); // 
  projeteisImg[9] = al_load_bitmap("Projeteis/especial.png"); // 
  projeteisImg[10] = al_load_bitmap("Projeteis/especial2.png"); // 


  // ------------------------------------------------------------------------ //
  // -------------------------- Variaveis allegro --------------------------- //
  
  // Cria o relógio, controla FPS, cria fila de eventos e instala fonte
  ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);	                   
  ALLEGRO_EVENT_QUEUE* fila = al_create_event_queue();                       
  ALLEGRO_FONT *fonte = al_load_ttf_font("ANNA____.ttf", 36, 0);
if (!fonte) {
    fprintf(stderr, "Erro ao carregar a fonte!\n");
    return -1;  // Ou algum código para finalizar a execução
}  // 36 é o tamanho da fonte

  // Redimencionamento de display ativado
  al_set_new_display_flags(ALLEGRO_RESIZABLE);
  al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW); // Seta fullscren 

  // Captura a maior resolução do monitor
  ALLEGRO_DISPLAY_MODE displayInit = displayRes();

  // Cria do display na maior resolução capturada
  ALLEGRO_DISPLAY* display = 
                       al_create_display(1280, 720); 
  /* ----------------------------------------------------------------------*/


  // Prepara a fila para receber eventos de teclado, display e relogio
  al_register_event_source(fila, al_get_keyboard_event_source());            
  al_register_event_source(fila, al_get_display_event_source(display));	       
  al_register_event_source(fila, al_get_timer_event_source(timer));		   

  //Inicia o relógio
  al_start_timer(timer);	

  // ----------------- Espaço de variaveis em C para uso geral -------------- //

  // Inidica se o programa deve seguir rodando ou não
  bool rodando = true;

  // Altura / Largura do display
  unsigned short largura, altura;
  unsigned short largInit = displayInit.width;
  unsigned short altInit = displayInit.height;
 
  // Cria o primeiro projetil da lista encadeada para referencia em funções
  struct Projetil *primeiro = NULL;  

  // Semente randomica para a função aleat
  srand(0);

    // Carrega a imagem de fundo
ALLEGRO_BITMAP *backgroundtela = al_load_bitmap("Backgrounds/fundo.jpg");
if (!backgroundtela) {
    fprintf(stderr, "Erro ao carregar a imagem de fundo.\n");
    return -1;
}

float tempoAparecimento = 2.0;  // Tempo para aparecer o texto completo
float duracaoTotal = 15.0;      // Duração total para a introdução (10 segundos)
bool mostrandoTexto = true;     // Flag para controlar a exibição do texto
bool textoCompleto = false;     // Flag para saber se o texto completo foi exibido
bool mostrarAperteEspaco = false; // Flag para mostrar "Aperte espaço para jogar"
int textoIndex = 0;
int numLines = 23;

while (mostrandoTexto) {
    ALLEGRO_EVENT evento;
    al_wait_for_event(fila, &evento);

    if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE) {
            mostrandoTexto = false;  // Salta o texto ao pressionar ESPAÇO
            break;
        }
    }

    if (evento.type == ALLEGRO_EVENT_TIMER) {
        // Desenha a imagem de fundo
        al_draw_bitmap(backgroundtela, 0, 0, 0);

        // Se o texto ainda não foi exibido completamente, exibe o texto
        if (tempoAparecimento < duracaoTotal) {
            tempoAparecimento += 1.0 / 60.0;  // Incrementa o tempo a cada "frame" (1/60 segundo)

            // Desenha cada linha de texto
            for (int i = 0; i < numLines; i++) {
                al_draw_text(fonte, al_map_rgb(255, 255, 255), 465, 100 + i * 40, ALLEGRO_ALIGN_CENTER, introText[i]);
            }
        }

        // Quando o tempo de 10 segundos passar, exibe "Aperte espaço para jogar"
        if (tempoAparecimento >= duracaoTotal && !textoCompleto) {
            mostrarAperteEspaco = true;
            textoCompleto = true;  // Marca que o texto completo foi exibido
        }

        // Se for hora de mostrar "Aperte espaço para jogar", desenha a mensagem
        if (mostrarAperteEspaco) {
            al_draw_text(fonte, al_map_rgb(255, 255, 255), 465, displayInit.height/2, ALLEGRO_ALIGN_CENTER, "Aperte espaço para jogar");
        }

        // Atualiza a tela
        al_flip_display();
    }
}

// Libera a memória após o uso
al_destroy_bitmap(backgroundtela);

al_set_timer_count(timer, 0);



  Fase *fase = criarFase1(backgrounds, numBackgrounds, projeteisImg, numImagens,
                                   largInit, altInit, &primeiro, fonte, inimImg, 
                                                numIniImg, musicas, numMusicas);
  
  Fase *fase2 = criarFase2(backgrounds2, numBackgrounds2, projeteisImg, numImagens,
                                   largInit, altInit, &primeiro, fonte, inimImg, 
                                                numIniImg, musicas, numMusicas);
                                         
  short mudarFase = 0;
  short numFase = 1; 
  bool morreu = 0;

  
  // --------------------------- Inicio do programa-------------------------- //
  
  while(numFase < 2 && morreu != 1){
  // Laço principal
  while(rodando){								

    // Guarda o evento 
    ALLEGRO_EVENT evento;	

    // Captura o evento e armazena na variavél evento
    al_wait_for_event(fila, &evento);   

    
    // Switch case que lida com os eventos
    switch (evento.type){

      // Evento de timer que roda o tempo inteiro
      case 30:
      
        // Função que administra a fase e faz tudo que é necessario
        mudarFase = gerenciarFase(fase, timer, largInit, altInit);

        // Se mudar fase for true, muda de fase e reseta o timer
        if (mudarFase == 1){
          rodando = 0;
          al_set_timer_count(timer, 0);
        }

        if (mudarFase == 2){
          rodando = 0;
          morreu = 1;
        }
        
        atualizarDisplay(0, 0, 0); 
        break;


      // Teclada apertada
      case 11:

        // Sai do jogo caso aperte Q
        if (evento.keyboard.keycode == ALLEGRO_KEY_Q){
          rodando = 0;
          numFase = 3;
          break;
        } 

        if (evento.keyboard.keycode == ALLEGRO_KEY_E){
      
          mudarFase++;
          break;
        } 

        
      
        // Seta os movimentos na entidade conforme botões são apertados
        moverEntidade(retornarEntidade(fase), evento.keyboard.keycode, 1);

        break;

      // Tecla solta
      case 12:

        // Retira movimentos quanto o botão é solto
        moverEntidade(retornarEntidade(fase), evento.keyboard.keycode, 0); 
        
        break;

      case 42:

        rodando = false;

        break;
    }



    // Realiza a mudança de fases
    if (mudarFase == 1){
      if (numFase == 1){
        destruirFase(fase);
        fase = fase2;
        numFase++;
        mudarFase = false;
        rodando = 1;
      }

    }
  }
  }

  ALLEGRO_BITMAP *backgroundtela2 = al_load_bitmap("Backgrounds/fundo2.jpg");
if (!backgroundtela) {
    fprintf(stderr, "Erro ao carregar a imagem de fundo.\n");
    return -1;
}

tempoAparecimento = 2.0;  // Tempo para aparecer o texto completo
duracaoTotal = 15.0;      // Duração total para a introdução (10 segundos)
mostrandoTexto = true;     // Flag para controlar a exibição do texto
textoCompleto = false;     // Flag para saber se o texto completo foi exibido
mostrarAperteEspaco = false; // Flag para mostrar "Aperte espaço para jogar"
textoIndex = 0;
numLines = 25;

while (mostrandoTexto) {
    ALLEGRO_EVENT evento;
    al_wait_for_event(fila, &evento);

    if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE) {
            mostrandoTexto = false;  // Salta o texto ao pressionar ESPAÇO
            break;
        }
    }
    if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
      if (evento.keyboard.keycode == ALLEGRO_KEY_Q) {
          mostrandoTexto = false;  // Salta o texto ao pressionar ESPAÇO
          break;
      }
  }

    if (evento.type == ALLEGRO_EVENT_TIMER) {
        // Desenha a imagem de fundo
        al_draw_bitmap(backgroundtela2, 0, 0, 0);

        // Se o texto ainda não foi exibido completamente, exibe o texto
        if (tempoAparecimento < duracaoTotal) {
            tempoAparecimento += 1.0 / 60.0;  // Incrementa o tempo a cada "frame" (1/60 segundo)

            // Desenha cada linha de texto
            if (morreu == 0)
            for (int i = 0; i < numLines; i++) {
                al_draw_text(fonte, al_map_rgb(255, 255, 255), 465, 100 + i * 30, ALLEGRO_ALIGN_CENTER, introText3[i]);
            }

            if (morreu == 1){
              numLines = 11;
            for (int i = 0; i < numLines; i++) {
                al_draw_text(fonte, al_map_rgb(255, 255, 255), 465, 100 + i * 30, ALLEGRO_ALIGN_CENTER, introText4[i]);
            }
        }
        }

        // Quando o tempo de 10 segundos passar, exibe "Aperte espaço para jogar"
        if (tempoAparecimento >= duracaoTotal && !textoCompleto) {
            mostrarAperteEspaco = true;
            textoCompleto = true;  // Marca que o texto completo foi exibido
        }

        // Se for hora de mostrar "Aperte espaço para jogar", desenha a mensagem
        if (mostrarAperteEspaco) {
            al_draw_text(fonte, al_map_rgb(255, 255, 255), 465, displayInit.height/2, ALLEGRO_ALIGN_CENTER, "Aperte espaço para sair");
        }

        // Atualiza a tela
        al_flip_display();
    }
}

  al_destroy_bitmap(backgroundtela2);

  // Liberam a memoria das variaveis após o fechamento do programa

 
  al_uninstall_audio();
  
  for (short i = 0; i < numIniImg; i++)
  al_destroy_bitmap(inimImg[i]);

  free(inimImg);

  for (short i = 0; i < numImagens; i++)
  al_destroy_bitmap(projeteisImg[i]);

  free(projeteisImg);

  al_uninstall_audio();

  free(musicas);
  
  al_destroy_font(fonte);													   
  al_destroy_display(display);												   
  al_destroy_timer(timer);												   
  al_destroy_event_queue(fila);							   

  return 0;
}
