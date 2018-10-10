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
#include "types.h"
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

/*
 * Pinos para controle do 
 * modulo nRF24L01
 * para a board UNO
 */
#if (HW_Board == MEGA_2560)
  #define RF24_CE_PIN 9
  #define RF24_CSN_PIN 10 
#endif

/*
 * Pinos de controle do
 * modulo nRF24L01
 * para a board MEGA
 */
#if (HW_Board == UNO_rev3)
  #define RF24_CE_PIN 9
  #define RF24_CSN_PIN 10
#endif

/**
 * Opcao para execucao "real" ou "simulada":
 *
 * 0 = execucao "real" via placa fisica.
 * 1 = execucao "simulada" via placa virtual.
 *
 **/
#define SIMUL 1

/*
 * Taxa de atualizacao dos displays
 */
#if  ( SIMUL == 0 )

  #define DISPLAY_refresh_RATE  ( Second_ms/4 )

#else

  #define DISPLAY_refresh_RATE  ( Second_ms/4 )

#endif

/*
 * Altera o fator de conversao
 * de segundos
 */
#if  ( SIMUL == 0 )
  #define Second_ms 1000
#else
  #define Second_ms 1000
#endif

/*
 * taxa de atualizacao de cronometro 
 */
#define SYS_100ms_CRONO_RATE  ( Second_ms/10 )

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
 *  Definicao da polaridade dos Displays 
 *   (Anodo ou Katodo Comum), 
 *   para o Placar "Main"
 **/
 #define Main_DISPLAY_pol KATODO_comum

/**
 * Definicao do tipo do Display 
 *  (Anodo ou Katodo Comum), 
 * para o Placar "Piloto"
 **/
#define Pilot_DISPLAY_pol ANODO_comum

/**
 * Codificacao dos padroes dos segmentos de "timeout", para o
 * Placar "Main":
 **/
#define  HOME_TMO_0_patt_M ( BIT_blank )
#define HOME_TMO_1_patt_M ( BIT_3 )
#define HOME_TMO_2_patt_M ( BIT_3 + BIT_2 )
#define HOME_TMO_3_patt_M ( BIT_3 + BIT_2 + BIT_1 )
#define HOME_TMO_mask_M   ( BIT_3 + BIT_2 + BIT_1 )

#define GUEST_TMO_0_patt_M  ( BIT_blank )
#define GUEST_TMO_1_patt_M  ( BIT_6 )
#define GUEST_TMO_2_patt_M  ( BIT_6 + BIT_5 )
#define GUEST_TMO_3_patt_M  ( BIT_6 + BIT_5 + BIT_4 )
#define GUEST_TMO_mask_M  ( BIT_6 + BIT_5 + BIT_4 )

/**
 * Codificacao dos padroes dos segmentos de "timeout", para o
 * Placar "Piloto":
 **/
#define HOME_TMO_0_patt_P ( BIT_blank )
#define HOME_TMO_1_patt_P ( BIT_3 )
#define HOME_TMO_2_patt_P ( BIT_3 + BIT_2 )
#define HOME_TMO_3_patt_P ( BIT_3 + BIT_2 + BIT_1 )
#define HOME_TMO_mask_P   ( BIT_3 + BIT_2 + BIT_1 )

#define GUEST_TMO_0_patt_P  ( BIT_blank )
#define GUEST_TMO_1_patt_P  ( BIT_6 )
#define GUEST_TMO_2_patt_P  ( BIT_6 + BIT_5 )
#define GUEST_TMO_3_patt_P  ( BIT_6 + BIT_5 + BIT_4 )
#define GUEST_TMO_mask_P  ( BIT_6 + BIT_5 + BIT_4 )

enum { HOME_VANTAGE, GUEST_VANTAGE };

/**
 * Codificacao dos padroes dos segmentos de "vantagem", para o
 * Placar "Main"
 */
#define HOME_VANT_patt_M  BIT_7
#define GUEST_VANT_patt_M BIT_0
#define VANT_PATT_mask_M  ( HOME_VANT_patt_M + GUEST_VANT_patt_M )

/**
 *  Codificacao dos padroes dos segmentos de "vantagem", para o
 *  Placar "Piloto"
 */
#define HOME_VANT_patt_P  BIT_7
#define GUEST_VANT_patt_P BIT_0
#define VANT_PATT_mask_P  ( HOME_VANT_patt_P + GUEST_VANT_patt_P )

#define VIEW_OFF  0
#define VIEW_ON   1

/**
 * Estrutura para tipagem do estado
 * do indicador
 */
typedef union
{
  WORD  VIEW_STS; // "view status" geral do Display.

  struct      // flags individuais de "view status":
  {
    WORD GAME_TIME  :1; // "view status" do indicador...
    WORD GAME_quarter:1;  // "view status" do indicador...

    WORD BALL_pos :1; // "view status" do indicador...
    WORD BALL_down  :1; // "view status" do indicador...
    WORD BALL_to_go :1; // "view status" do indicador...

    WORD SCORE_home :1; // "view status" do indicador...
    WORD VANT_home  :1; // "view status" do indicador...
    WORD TMOUT_home :1; // "view status" do indicador...

    WORD SCORE_guest:1; // "view status" do indicador...
    WORD VANT_guest :1; // "view status" do indicador...
    WORD TMOUT_guest:1; // "view status" do indicador...

    WORD PLAY_CLOCK :1; // // "view status" do indicador...
  };

} DISPLAY_VIEW_type;

/*
 * Definicao do caracter de finalizacao de um comando:
 */
#if  ( SIMUL == 0 )

  #define CMD_end 0x0A  // caracter de finalizacao de comando = "line feed".

#else
  
  //#define CMD_end '.' // caracter de finalizacao de comando = "ponto".
  #define CMD_end 0x0D
#endif

#define  CMD_no_reply  0   // sem resposta aos comandos recebidos.
#define CMD_full_reply  1 // resposta "completa" aos comandos recebidos.
#define CMD_code_reply  2 // resposta "codificada" aos comandos recebidos.

typedef  bool (*CMD_func_PTR) ( BYTE_PTR CMD_data, BYTE index );

/*
 * Estrutura para mapeamento entre 
 * tipo de comando e funcao de comando
 */
struct  CMD_LOC_info
{
  char CMD_code;

  CMD_func_PTR  CMD_func;
};

typedef struct  CMD_LOC_info  CMD_LOC_type;


#define CMD_LIST_end_mark 1 // marcador do fim de uma lista
                            // de comandos.
#endif
