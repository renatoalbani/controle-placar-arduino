/*
  main.h - Biblioteca para definicoes de tipos e
           funcoes utilitarias do sistema
  @Criador <Incluir dados de quem escreveu para Leandro>
           Leandro Morozowski <lmorozowski@hotmail.com>,
           Renato Albani dos Santos <renato.utfpr@gmail.com>, 
           2018
  @Atualizado Renato Albani dos Santos <renato.utfpr@gmail.com>, Outubro 2018
 */
#ifndef main_h
#define main_h
/**
 * Definicao dos modelos (ou tipos) de placas da Plataforma
 * Arduino, suportados por este Sistema:
 **/
#define UNO_rev3  1 // placa modelo "Arduino UNO rev 3".
#define MEGA_2560 2 // placa modelo "Arduino Mega 2560".

/**
 * Opcao para a placa de Hardware utilizada no Sistema:
 * UNO = opcao para utilizacao da placa Arduino UNO.
 * MEGA_2560 = opcao para utilizacao da placa Arduino Mega 2560.
 **/
#define HW_Board  MEGA_2560 // seleciona a placa utilizada.

/**
 * Opcao para execucao "real" ou "simulada":
 *
 * 0 = execucao "real" via placa fisica.
 * 1 = execucao "simulada" via placa virtual.
 *
 **/
#define SIMUL 1
/**
 *  Opcao para disponibilidade dos Placares "Main" e "Piloto":
 *
 *  0 = Placar ausente.
 *  1 = Placar presente.
 **/
#define Main_DISPLAY_ON   1 // indica se o Placar "Main" esta
                            // presente no Sistema.
#define Pilot_DISPLAY_ON  1 // indica se o Placar "Piloto" esta
                            // presente no Sistema.
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
#define  BIT_0 0x01  // padrao binario correspondente ao "bit 0".
#define BIT_1 0x02  // padrao binario correspondente ao "bit 1".
#define BIT_2 0x04  // padrao binario correspondente ao "bit 2".
#define BIT_3 0x08  // padrao binario correspondente ao "bit 3".
#define BIT_4 0x10  // padrao binario correspondente ao "bit 4".
#define BIT_5 0x20  // padrao binario correspondente ao "bit 5".
#define BIT_6 0x40  // padrao binario correspondente ao "bit 6".
#define BIT_7 0x80  // padrao binario correspondente ao "bit 7".
#define BIT_blank 0x00  // padrao para todos os bits em "0".

/**
 * Polarizacao dos displays
 */
#define ANODO_comum 0   // indica Display tipo "anodo comum".
#define KATODO_comum  1 // indica Display tipo "katodo comum".

/**
 *  Mapeamento dos segmentos do Display no Placar "Main":
 **/
#define SEG_aM  BIT_7  // bit correspondente ao Segmento "a".
#define SEG_bM  BIT_1  // bit correspondente ao Segmento "b".
#define SEG_cM  BIT_2  // bit correspondente ao Segmento "c".
#define SEG_dM  BIT_3  // bit correspondente ao Segmento "d".
#define SEG_eM  BIT_4  // bit correspondente ao Segmento "e".
#define SEG_fM  BIT_5  // bit correspondente ao Segmento "f".
#define SEG_gM  BIT_6  // bit correspondente ao Segmento "g".

/**
 * Mapeamento dos segmentos do Display no Placar "Piloto":
 **/
#define SEG_aP  BIT_2  // bit correspondente ao Segmento "a".
#define SEG_bP  BIT_1  // bit correspondente ao Segmento "b".
#define SEG_cP  BIT_5  // bit correspondente ao Segmento "c".
#define SEG_dP  BIT_6  // bit correspondente ao Segmento "d".
#define SEG_eP  BIT_7  // bit correspondente ao Segmento "e".
#define SEG_fP  BIT_3  // bit correspondente ao Segmento "f".
#define SEG_gP  BIT_4  // bit correspondente ao Segmento "g".

/**
 *  Codificacao dos padroes numericos BCD em 7 segmentos, para o
 *  Placar "Main".
 **/
#define Seven_SEG_0_patt_M  ( SEG_aM + SEG_bM + SEG_cM + SEG_dM + SEG_eM + SEG_fM )
#define Seven_SEG_1_patt_M  ( SEG_bM + SEG_cM )
#define Seven_SEG_2_patt_M  ( SEG_aM + SEG_bM + SEG_dM + SEG_eM + SEG_gM )
#define Seven_SEG_3_patt_M  ( SEG_aM + SEG_bM + SEG_cM + SEG_dM + SEG_gM )
#define Seven_SEG_4_patt_M  ( SEG_bM + SEG_cM + SEG_fM + SEG_gM )
#define Seven_SEG_5_patt_M  ( SEG_aM + SEG_cM + SEG_dM + SEG_fM + SEG_gM )
#define Seven_SEG_6_patt_M  ( SEG_aM + SEG_cM + SEG_dM + SEG_eM + SEG_fM + SEG_gM )
#define Seven_SEG_7_patt_M  ( SEG_aM + SEG_bM + SEG_cM )
#define Seven_SEG_8_patt_M  ( SEG_aM + SEG_bM + SEG_cM + SEG_dM + SEG_eM + SEG_fM + SEG_gM )
#define Seven_SEG_9_patt_M  ( SEG_aM + SEG_bM + SEG_cM + SEG_dM + SEG_fM + SEG_gM )

/**
 *  Codificacao dos padroes numericos BCD em 7 segmentos, para o
 *  Placar "Piloto".
 **/
#define Seven_SEG_0_patt_P  ( SEG_aP + SEG_bP + SEG_cP + SEG_dP + SEG_eP + SEG_fP )
#define Seven_SEG_1_patt_P  ( SEG_bP + SEG_cP )
#define Seven_SEG_2_patt_P  ( SEG_aP + SEG_bP + SEG_dP + SEG_eP + SEG_gP )
#define Seven_SEG_3_patt_P  ( SEG_aP + SEG_bP + SEG_cP + SEG_dP + SEG_gP )
#define Seven_SEG_4_patt_P  ( SEG_bP + SEG_cP + SEG_fP + SEG_gP )
#define Seven_SEG_5_patt_P  ( SEG_aP + SEG_cP + SEG_dP + SEG_fP + SEG_gP )
#define Seven_SEG_6_patt_P  ( SEG_aP + SEG_cP + SEG_dP + SEG_eP + SEG_fP + SEG_gP )
#define Seven_SEG_7_patt_P  ( SEG_aP + SEG_bP + SEG_cP )
#define Seven_SEG_8_patt_P  ( SEG_aP + SEG_bP + SEG_cP + SEG_dP + SEG_eP + SEG_fP + SEG_gP )
#define Seven_SEG_9_patt_P  ( SEG_aP + SEG_bP + SEG_cP + SEG_dP + SEG_fP + SEG_gP )


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

} DISPLAY_info;

#endif
