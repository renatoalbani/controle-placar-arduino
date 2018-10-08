/*
   types.h - Definicao dos tipos utilizados no sistema
    @Criador <Incluir dados de quem escreveu para Leandro>
            Leandro Morozowski <lmorozowski@hotmail.com>, 
            Renato Albani dos Santos <renato.utfpr@gmail.com>, 
            2018
    @Atualizado Renato Albani dos Santos <renato.utfpr@gmail.com>, Outubro 2018        
 */
#ifndef types_h
#define types_h
/**
 * Definicao de tipos para facilitar codificacao do programa:
 **/
#define  uint  unsigned int  // tipo inteiros positivos de 16 bits.
#define ulong unsigned long // tipo inteiros positivos de 32 bits.

typedef unsigned char BYTE; // tipo 8 bits, nao sinalizado.
typedef unsigned short  WORD; // tipo 16 bits, nao sinalizado.
typedef unsigned long DWORD;  // tipo 32 bits, nao sinalizado.

typedef signed char sBYTE;  // tipo 8 bits, sinalizado.
typedef signed short  sWORD;  // tipo 16 bits, sinalizado.
typedef signed long sDWORD; // tipo 32 bits, sinalizado.

typedef BYTE* BYTE_PTR; // tipo Ponteiro para acesso a Bytes.

/**
 * Definicao de padroes de bit para facilitar codificacao do
 * Programa em acessos individuais aos respectivos bits:
 **/
#define BIT_0 0x01  // padrao binario correspondente ao "bit 0".
#define BIT_1 0x02  // padrao binario correspondente ao "bit 1".
#define BIT_2 0x04  // padrao binario correspondente ao "bit 2".
#define BIT_3 0x08  // padrao binario correspondente ao "bit 3".
#define BIT_4 0x10  // padrao binario correspondente ao "bit 4".
#define BIT_5 0x20  // padrao binario correspondente ao "bit 5".
#define BIT_6 0x40  // padrao binario correspondente ao "bit 6".
#define BIT_7 0x80  // padrao binario correspondente ao "bit 7".
#define BIT_blank 0x00  // padrao para todos os bits em "0".

/*
 * Estrutura para controle 
 * de cronometro
 */
struct  CRONO_type
{
  uint periodo_ms;  // periodo de disparo do cronometro.

  bool trigger;   // indica que ocorreu um evento do cronometro.

  bool run;   // habilita cadencia do cronometro.

  void (*CRONO_notify) ();  // funcao para informe da cadencia.

  ulong registro_ms;  // uso interno para deteccao de evento.
};

/**
 * Definicao de uma estrutura para acesso aos padroes de bits dos
 * segmentos de LEDs do Display, e especificacao dos pinos de HW
 * para controle do Placar fisico:
 **/
struct  DISPLAY_desc
{
  BYTE_PTR SEGM_code_TAB; // Tabela de codificacao dos segmentos do Display.

  BYTE_PTR TMO_code_TAB;  // Tabela com codificacao do Timeout "Home/Guest".

  BYTE_PTR VANT_code_TAB; // Tabela com codificacao do "Vantage".

  bool  polarity; // polaridade do Display (anodo/katodo comum).

  BYTE SDOUT_pin;   // pino correspondente ao sinal "SDOUT".

  BYTE SCLK_pin;    // pino correspondente ao sinal "SCLK".

  BYTE PCLK_pin;    // pino correspondente ao sinal "PCLK".

};
#endif
