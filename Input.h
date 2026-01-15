#ifndef __INPUT__
#define __INPUT__	

typedef struct{
  unsigned char up;
  unsigned char down;
  unsigned char right;
  unsigned char left;
  unsigned char shot;

} input;

  input* criarTeclado();

  void destruirTeclado(input *teclado);
  void tecladoUp(input *teclado);
  void tecladoDown(input *teclado);
  void tecladoRight(input *teclado);
  void tecladoLeft(input *teclado);
  void tecladoShot(input *teclado, char modo);

#endif
