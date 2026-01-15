
# Nome do executável
TARGET = jogo

# Arquivos de origem
SRCS = Mundo.c Entidades.c Input.c Projetil.c Objetos.c 

# Arquivos de cabeçalho
HEADERS = Entidades.h Input.h Projetil.h Objetos.h

# Compilador
CC = gcc

# Flags do compilador obtidas via pkg-config
CFLAGS = -Wall $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_ttf-5 --cflags)
LDFLAGS = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 allegro_audio-5 allegro_acodec-5 allegro_ttf-5 --libs) -lm

# Regra padrão para compilar tudo
all: $(TARGET)

# Regra para compilar o executável
$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $(TARGET) $(LDFLAGS)

# Regra para limpar os arquivos gerados
clean:
	rm -f $(TARGET)

