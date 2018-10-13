#include <Arduino.h>

#include "types.h" // definicoes de tipos utilizados pelo sistema
#include "util.h" // funcoes utilitarias e definicoes de tipos
#include "main.h" // biblioteca com definicoes do sistema
#include "pins.h" //definicoes dos pinos da placa utilizados
#include "limits.h" //definicoes dos limites de sistema
#include "pgmrom.h" //funcoes para acesso a memória de programa
#include "crono.h" //funcoes para controle de cronometros

#include <stdio.h>  // "stdio" nativo do C++, para Arduino (CPU AVR).
#include <stdlib.h> // "stdlib" nativo do C++, para Arduino (CPU AVR).
#include <avr/pgmspace.h> // para acesso à FLASH da CPU.

#include "C74HC595.h" //funcoes para controle dos 74HC595

#include "rxplacar.h"

/**
 * Definicao de uma estrutura para acesso aos padroes de bits dos
 * segmentos de LEDs do Display, e especificacao dos pinos de HW
 * para controle do Placar fisico:
 **/
DISPLAY_desc DISPLAY_info;
DISPLAY_VIEW_type  DISPLAY_VIEW_STS;
/**
 * Inicializacao de instancia para controle dos 74HC595
 */
C74HC595 DISPLAY_driver(SCLK_main, SDOUT_main, PCLK_main);
/**
 * Espelhamento de timeout e vantage
 */
BYTE  TMO_VANT_REG = 0; // espelho do Registro de HW com bits do
                        // "timeout" e "vantage".

/**
 *  Definicao dos elementos referentes ao tempo do jogo:
 **/
BYTE  GAME_TIME_min;  // indica os minutos do tempo de jogo (00..59).
BYTE  GAME_TIME_seg;  // indica os segundos do tempo de jogo (00..59).

/**
 * Definicao dos elementos referentes ao "spot de bola":
 */
BYTE  BALL_position;  // indica a posicao atual da Bola (00..99).
BYTE  BALL_down;      // indica o "down" da Bola (0..9).
BYTE  BALL_to_go;     // indica o "to go" da Bola (00..99).

/*
 * Definicao dos elementos referentes à Pontuacao e Timeout da 
 * "casa" e dos "visitantes", da "vantagem" atual, e do "quarter"
 * atual do jogo.
 */

BYTE  SCORE_home;   // indica a pontuacao da "casa" (00..99).
BYTE  TMOUT_home;   // indica o "timeout" da "casa" (0..3).
BYTE  SCORE_guest;  // indica a pontuacao do "visitante" (00..99).
BYTE  TMOUT_guest;  // indica o "timeout" do "visitante" (0..3).
BYTE  GAME_VANTAGE; // indica a vantagem atual no jogo.
BYTE  GAME_quarter; // indica o "quarter" do jogo (1, 2, 3, ou 4).

/*
 * Definicao do elemento referente ao "Play Clock" atual do jogo:
 */
BYTE  PLAY_CLOCK;               // indica o "Play Clock" atual do jogo (00..99).
bool  PLAY_CLOCK_swap = false;  // indica se o "smart swap" esta
                                // ativo para o "Play Clock".
                               
/**
 * Definicao de um contador para controle do cadenciamento de
 * operacoes "HMI" (Interface Homem/Maquina) neste Sistema:
 */
WORD  SYS_COUNTER_100ms = 0;
/*
 * Definicao de um cronometro de 100ms para cadenciamento de
 * operacoes "HMI" (Interface Homem/Maquina) neste Sistema:
 */
CRONO_type  SYS_100ms_CRONO;  // cronometro para cadenciamento
                              // de operacoes "HMI" no Sistema.

bool  DISPLAY_refresh_force = false;

/*
 *  Definicao de um cronometro para a taxa de atualizacao do
 *  Display.
 */
CRONO_type  DISPLAY_refresh_CRONO;  // cronometro para temporizacao
                                    // da atualizacao do Display.                                                             
/*
 * Definicao de um cronometro para a cadencia do "Game Time"
 * e suas funcoes
 */
CRONO_type  GAME_TIME_CRONO;  // cronometro para cadencia do
                              // "Game Time".
                              
/*
 *  Definicao de um cronometro para a cadencia do "Play Clock":
 */
CRONO_type  PLAY_CLOCK_CRONO; // cronometro para cadencia do
                              // "Play Clock".

BYTE  CMD_reply = CMD_full_reply; // indica o tipo de resposta aos
                                  // comandos recebidos.

/*
 * Definicoes do buffer de comandos
 */
 // maximo de caracteres em um comando.
#define CMD_size 10
BYTE  CMD_BUFF [CMD_size];  // Buffer de Comandos recebidos.
BYTE  CMD_BUFF_idx; // posicao atual no Buffer de Comandos.


CMD_func_PTR SYS_CMD_PTR; // Ponteiro para o Comando a ser executado.
/*
 * Inicializa o cronometro de cadencia do "Play clock"
 */
void  PLAY_CLOCK_startup ()
{
  CRONO_init ( &PLAY_CLOCK_CRONO, Second_ms );
}

/*
 * Registra a funcao a ser invocada pelas
 * notificacoes de cadencia do cronometro
 * de cadencia do "Play clock"
 */
void  PLAY_CLOCK_notify_ON ( void (*PLAY_CLOCK_notify) () )
{
  CRONO_notify_ON ( &PLAY_CLOCK_CRONO, PLAY_CLOCK_notify );
}

/*
 * Cancela o registro da funcao de noticacao
 * do cronometro de cadencia do "Play clock"
 */
void  PLAY_CLOCK_notify_OFF ()
{
  CRONO_notify_OFF ( &PLAY_CLOCK_CRONO );
}

/*
 * Pausa o cronometro de cadencia
 * do "Play clock"
 */
void  PLAY_CLOCK_stop ()
{
  CRONO_stop ( &PLAY_CLOCK_CRONO );
}

/*
 * Coloca o cronometro de cadencia
 * do "Play clock" em estado de
 * execucao
 */
void  PLAY_CLOCK_run ()
{
  CRONO_run ( &PLAY_CLOCK_CRONO );
}

/*
 * Verifica o estado de execucao do 
 * cronometro de cadencia do "Play clock"
 *  @returns True caso este esteja em execucao e False caso contrario
 */
bool  PLAY_CLOCK_running_CHECK ()
{
  return ( CRONO_running_CHECK ( &PLAY_CLOCK_CRONO ) );
}

/*
 * Controla a cadencia do cronometro
 * de cadencia do "Play clock"
 */
void  PLAY_CLOCK_cadence ()
{
  if ( ! PLAY_CLOCK  )
  {
    CRONO_stop ( &PLAY_CLOCK_CRONO );
    return;
    
  }
  
  CRONO_atualize ( &PLAY_CLOCK_CRONO );
  bool now = CRONO_trigger_CHECK ( &PLAY_CLOCK_CRONO );
  
  if ( ! now )
  {
    return;    
  }
  
  PLAY_CLOCK--;
  CRONO_detected ( &PLAY_CLOCK_CRONO );
    
}

/**
 * Funcao utilitaria para serializacao do estado dos 
 * segmentos de um display 
 */
void  DISPLAY_PATTERN_send ( BYTE PATTERN )
{
  if ( DISPLAY_info.polarity == 0 ) PATTERN = ~PATTERN;
  
  DISPLAY_driver.writeData ( PATTERN );
}

/*
 * Codifica e serializa um digito decimal para os segmentos 
 * de um display de acordo com a tabela
 * de codificacao e polarizacao configurada
 */
void  DISPLAY_DIGIT_send ( BYTE DATA )
{
  BYTE  LSB;
  BYTE  PATTERN;

  LSB = ( DATA & 0x0F );

  PATTERN = ROM_get8 ( DISPLAY_info.SEGM_code_TAB + LSB );

  if ( DISPLAY_info.polarity == 0 ) PATTERN = ~PATTERN;

  DISPLAY_driver.writeData ( PATTERN );
}

/*
 * Codifica e serializa um dado no padrao BCD 
 * para os seguimentos de um display de acordo 
 * com a tabela de codificacao e polarizacao
 * configurada
 */
void  DISPLAY_BCD_BYTE_send ( BYTE DATA )
{
  BYTE  LSB;
  BYTE  MSB;
  BYTE  PATTERN;

  LSB = ( DATA & 0x0F );
  MSB = ( DATA & 0xF0 ) >> 4;

  PATTERN = ROM_get8 ( DISPLAY_info.SEGM_code_TAB + LSB );

  if ( DISPLAY_info.polarity == 0 ) PATTERN = ~PATTERN;

  DISPLAY_driver.writeData ( PATTERN );

  PATTERN = ROM_get8 ( DISPLAY_info.SEGM_code_TAB + MSB );

  if ( DISPLAY_info.polarity == 0 ) PATTERN = ~PATTERN;

  DISPLAY_driver.writeData ( PATTERN );
}

/*
 * Envia o digito 0 para 
 * os segmentos de um display
 */
void  BLANK_PATTERN_send ()
{
  DISPLAY_PATTERN_send (0);
}

/**
 * Envia o digito 0 para
 * os segmentos de dois display
 * em sequencia
 */
void  BLANK_PATTERN_DOUBLE_send ()
{
  DISPLAY_PATTERN_send (0);
  DISPLAY_PATTERN_send (0);
}

/*
 * Codifica e envia o estado dos displays de GAME TIME
 *  @param BYTE minutos Decimal dos minutos
 *  @param BYTE segundos Decimal dos segundos
 */
void  GAME_TIME_send ( BYTE minutos, BYTE segundos )
{
  if ( DISPLAY_VIEW_STS.GAME_TIME )
  {
    DISPLAY_BCD_BYTE_send ( HEX8_to_BCD8 ( segundos ) );
    DISPLAY_BCD_BYTE_send ( HEX8_to_BCD8 ( minutos ) );
  }
  else
  {
    BLANK_PATTERN_DOUBLE_send ();
    BLANK_PATTERN_DOUBLE_send ();
  }
}

/*
 *  Codifica e envia o estado do display de BALL POSITION
 *   @param BYTE BALL_pos Posicao da bola
 */
void  BALL_POSITION_send ( BYTE BALL_pos )
{
  if ( DISPLAY_VIEW_STS.BALL_pos )
  {
    DISPLAY_BCD_BYTE_send ( HEX8_to_BCD8 ( BALL_pos ) );
  }
  else  BLANK_PATTERN_DOUBLE_send ();
}

/*
 * Codifica e envia o estado do display de BALL DOWN
 *  @param BYTE BALL_down Estado de BALL DOWN
 */
void  BALL_DOWN_send ( BYTE BALL_down )
{
  if ( DISPLAY_VIEW_STS.BALL_down )
  {
    DISPLAY_DIGIT_send ( BALL_down );
  }
  else  BLANK_PATTERN_send ();
}

/*
 * Codifica e envia o estado do display BALL TO GO
 *  @param BYTE BALL_to_go Estado de BALL TO GO
 */
void  BALL_to_GO_send ( BYTE BALL_to_go )
{
  if ( DISPLAY_VIEW_STS.BALL_to_go )
  {
    DISPLAY_BCD_BYTE_send ( HEX8_to_BCD8 ( BALL_to_go ) );
  }
  else  BLANK_PATTERN_DOUBLE_send ();
}

/*
 * Codifica e envia o estado do display SCORE HOME (CASA)
 *  @param BYTE SCORE_home Estado de SCORE HOME
 */
void  SCORE_HOME_send ( BYTE SCORE_home )
{
  if ( DISPLAY_VIEW_STS.SCORE_home )
  {
    DISPLAY_BCD_BYTE_send ( HEX8_to_BCD8 ( SCORE_home ) );
  }
  else  BLANK_PATTERN_DOUBLE_send ();
}

/*
 * Codifica e envia o estado do display SCORE GUEST (VISITANTE)
 *  @param BYTE SCORE_guest Estado de SCORE GUEST
 */
void  SCORE_GUEST_send ( BYTE SCORE_guest )
{
  if ( DISPLAY_VIEW_STS.SCORE_guest )
  {
    DISPLAY_BCD_BYTE_send ( HEX8_to_BCD8 ( SCORE_guest ) );
  }
  else  BLANK_PATTERN_DOUBLE_send ();
}

/**
 * Codifica e envia o estado do display GAME QUARTER
 *  @param BYTE GAME_quarter Estado de GAME QUARTER
 */
void  GAME_QUARTER_send ( BYTE GAME_quarter )
{
  if ( DISPLAY_VIEW_STS.GAME_quarter )
  {
    DISPLAY_DIGIT_send ( GAME_quarter );
  }
  else  BLANK_PATTERN_send ();
}

/*
 * Codifica e envia o estado do display PLAY CLOCK
 *  @param BYTE PLAY_CLOCK Estado de PLAY CLOCK
 */
void  PLAY_CLOCK_send ( BYTE PLAY_CLOCK )
{
  if ( DISPLAY_VIEW_STS.PLAY_CLOCK )
  {
    DISPLAY_BCD_BYTE_send ( HEX8_to_BCD8 ( PLAY_CLOCK ) );
  }
  else  BLANK_PATTERN_DOUBLE_send ();
}

/*
 * Codifica e envia o estado do display TIMEOUT VANTAGE  
 */
void  TMO_VANT_REG_send ()
{
  DISPLAY_PATTERN_send ( TMO_VANT_REG );
}

/*
 * Atualiza e codifica o estado em memoria 
 * de TIMEOUT VANTAGE de acordo com a 
 * tabela de codificacao
 *  @param BYTE vantage Estado de TIMEOUT VANTAGE
 */
void  VANTAGE_actualize ( BYTE vantage )
{
  BYTE  VANT_patt;

  if ( vantage == HOME_VANTAGE )
  {
    VANT_patt = ROM_get8 ( DISPLAY_info.VANT_code_TAB + 0 );  // obtem padrao de bits.

    VANT_patt &= BOOL_extent( DISPLAY_VIEW_STS.VANT_home );
  }
  else
  {
    VANT_patt = ROM_get8 ( DISPLAY_info.VANT_code_TAB + 1 );  // obtem padrao de bits.

    VANT_patt &= BOOL_extent( DISPLAY_VIEW_STS.VANT_guest );
  }

  TMO_VANT_REG &= (~ROM_get8 ( DISPLAY_info.VANT_code_TAB + 2 ) );  // aplica mascara.

  TMO_VANT_REG |= VANT_patt;
}


/*
 * Atualiza e codifica o estado em memoria 
 * do HOME TIMEOUT de acordo com a tabela
 * de codificacao
 */
void  HOME_TIMEOUT_actualize ( BYTE timeout )
{
  BYTE  TMO_patt;
  BYTE  index;

  index = 2*timeout;

  TMO_patt = ROM_get8 ( DISPLAY_info.TMO_code_TAB + index );  // obtem padrao de bits.

  TMO_patt &= BOOL_extent( DISPLAY_VIEW_STS.TMOUT_home );

  TMO_VANT_REG &= ( ~ROM_get8 ( DISPLAY_info.TMO_code_TAB + 2*4 ) );  // aplica mascara.

  TMO_VANT_REG |= TMO_patt;
}

/*
 * Atualiza e codifica o estado em memoria 
 * de GUEST TIMEOUT de acordo com a tabela
 * de codificacao
 */
void  GUEST_TIMEOUT_actualize ( BYTE timeout )
{
  BYTE  TMO_patt;
  BYTE  index;

  index = 2*timeout +1;

  TMO_patt = ROM_get8 ( DISPLAY_info.TMO_code_TAB + index );  // obtem padrao de bits.

  TMO_patt &= BOOL_extent( DISPLAY_VIEW_STS.TMOUT_guest );

  TMO_VANT_REG &= ( ~ROM_get8 ( DISPLAY_info.TMO_code_TAB + 2*4 +1 ) ); // aplica mascara.

  TMO_VANT_REG |= TMO_patt;
}


/*
 * Atualiza e envia o estado dos displays de
 * HOME e GUEST timeout e vantage 
 *  @param BYTE HOME_tmo Timeout de HOME (CASA)
 *  @param BYTE GUEST_tmo Timeout de GUEST (VISITANTE)
 *  @param BYTE vantage VANTAGE
 */
void  TIMEOUT_VANTAGE_send ( BYTE HOME_tmo, BYTE GUEST_tmo, BYTE vantage )
{
  HOME_TIMEOUT_actualize ( HOME_tmo );

  GUEST_TIMEOUT_actualize ( GUEST_tmo );

  VANTAGE_actualize ( vantage );

  TMO_VANT_REG_send ();
}

/**
 * Serializacao do estado de todos os displays
 * do placar de acordo com a ordem
 * de ligacao e cascateamento dos displays
 * nos circuitos de comando dos mesmos
 */
void  PLACAR_DISPLAY_normal_serialize ()
{
  BALL_to_GO_send ( BALL_to_go );

  BALL_DOWN_send ( BALL_down );

  BALL_POSITION_send ( BALL_position );
  
  PLAY_CLOCK_send ( PLAY_CLOCK );

  TIMEOUT_VANTAGE_send ( TMOUT_home, TMOUT_guest, GAME_VANTAGE );

  GAME_QUARTER_send ( GAME_quarter );

  SCORE_GUEST_send ( SCORE_guest );

  SCORE_HOME_send ( SCORE_home );

  GAME_TIME_send ( GAME_TIME_min, GAME_TIME_seg );
}


/**
 * Serializacao do estado de todos os displays
 * do placar de acordo com a ordem
 * de ligacao e cascateamento dos displays
 * nos circuitos de comando dos mesmos com
 * sobreposicao de PLAY CLOCK em BALL POSITION
 */
void  PLACAR_DISPLAY_swap_serialize ()
{
  BALL_to_GO_send ( BALL_to_go );

  BALL_DOWN_send ( BALL_down );

  BALL_POSITION_send ( PLAY_CLOCK );  // overlap "Ball" com o "Play Clock".

  PLAY_CLOCK_send ( PLAY_CLOCK );

  TIMEOUT_VANTAGE_send ( TMOUT_home, TMOUT_guest, GAME_VANTAGE );

  GAME_QUARTER_send ( GAME_quarter );

  SCORE_GUEST_send ( SCORE_guest );

  SCORE_HOME_send ( SCORE_home );

  GAME_TIME_send ( GAME_TIME_min, GAME_TIME_seg );
}

/*
 * Atualizacao geral do estado dos displays do
 * placar
 */
void  PLACAR_DISPLAY_refresh ()
{
  if ( !( PLAY_CLOCK_swap && PLAY_CLOCK_running_CHECK() ) )

    PLACAR_DISPLAY_normal_serialize();

  else  PLACAR_DISPLAY_swap_serialize();
}

/*
 * Atualiza as tabelas de codificacao
 * e o estado atual do placar "Main"
 */
void  PLACAR_MAIN_DISPLAY_refresh ()
{
  DISPLAY_info.SEGM_code_TAB = (BYTE_PTR) Seven_SEG_code_M; // Tabela de codificacao dos segmentos do Display.

  DISPLAY_info.TMO_code_TAB = (BYTE_PTR) TMO_code_TAB_M;    // Tabela com codificacao do Timeout "Home/Guest".

  DISPLAY_info.VANT_code_TAB = (BYTE_PTR) VANT_code_TAB_M;  // Tabela com codificacao do "Vantage".

  DISPLAY_info.polarity = Main_DISPLAY_pol; // polaridade do Display (anodo/katodo comum).

  DISPLAY_info.SDOUT_pin = SDOUT_main;  // especifica pino "SDOUT" para o "Main".

  DISPLAY_info.SCLK_pin = SCLK_main;  // especifica pino "SCLK" para o "Main".

  DISPLAY_info.PCLK_pin = PCLK_main;  // especifica pino "PCLK" para o "Main".

  PLACAR_DISPLAY_refresh ();

  delayMicroseconds(10);

  DISPLAY_driver.latch ();
}

/*
 * Atualiza o estado da flag de controle
 * em memoria para display VIEW SET 
 *  @param bool view Flag que indica o estado (1 ativo/ 0 inativo)
 */
void  DISPLAY_view_SET ( bool view )
{
  DISPLAY_VIEW_STS.VIEW_STS = view ? 0xFFFF : 0;
}

/* 
 * Atualiza o estado da flag de controle 
 * em memoria para o GAME TIME
 *  @param bool view Flag que indica o estado (1 ativo/ 0 inativo)
 */
void  GAME_TIME_view_SET ( bool view )
{
  DISPLAY_VIEW_STS.GAME_TIME = view;
}


/*
 * Atualiza o estado da flag de controle
 * em memoria para o GAME QUARTER
 *  @param bool view Flag que indica o estado (1 ativo/ 0 inativo)
 */
void  GAME_QUARTER_view_SET ( bool view )
{
  DISPLAY_VIEW_STS.GAME_quarter = view;
}

/*
 * Atualiza o estado da flag de controle
 * em memoria para o BALL POSITION
 *  @param bool view Flag que indica o estado (1 ativo/ 0 inativo)
 */
void  BALL_POSITION_view_SET ( bool view )
{
  DISPLAY_VIEW_STS.BALL_pos = view;
}

/*
 * Atualiza o estado da flag de controle
 * em memoria para o BALL DOWN
 *  @param bool view Flag que indica o estado (1 ativo/ 0 inativo)
 */
void  BALL_DOWN_view_SET ( bool view )
{
  DISPLAY_VIEW_STS.BALL_down = view;
}

/*
 * Atualiza o estado da flag de controle
 * em memoria para o BALL TO GO
 *  @param bool view Flag que indica o estado (1 ativo/ 0 inativo)
 */

void  BALL_TO_GO_view_SET ( bool view )
{
  DISPLAY_VIEW_STS.BALL_to_go = view;
}

/*
 * Atualiza o estado da flag de controle
 * em memoria para o SCORE HOME
 *  @param bool view Flag que indica o estado (1 ativo/ 0 inativo)
 */
void  SCORE_HOME_view_SET ( bool view )
{
  DISPLAY_VIEW_STS.SCORE_home = view;
}

/*
 * Atualiza o estado da flag de controle
 * em memoria para o VANTAGE HOME
 *  @param bool view Flag que indica o estado (1 ativo/ 0 inativo)
 */
void  VANT_HOME_view_SET ( bool view )
{
  DISPLAY_VIEW_STS.VANT_home = view;
}

/*
 * Atualiza o estado da flag de controle
 * em memoria para o TIMEOUT HOME
 *  @param bool view Flag que indica o estado (1 ativo/ 0 inativo)
 */
void  TMOUT_HOME_view_SET ( bool view )
{
  DISPLAY_VIEW_STS.TMOUT_home = view;
}

/*
 * Atualiza o estado da flag de controle
 * em memoria para o SCORE GUEST
 *  @param bool view Flag que indica o estado (1 ativo/ 0 inativo)
 */
void  SCORE_GUEST_view_SET ( bool view )
{
  DISPLAY_VIEW_STS.SCORE_guest = view;
}

/*
 * Atualiza o estado da flag de controle
 * em memoria para o VANTAGE GUEST
 *  @param bool view Flag que indica o estado (1 ativo/ 0 inativo)
 */
void  VANT_GUEST_view_SET ( bool view )
{
  DISPLAY_VIEW_STS.VANT_guest = view;
}

/*
 * Atualiza o estado da flag de controle
 * em memoria para o TIMEOUT GUEST
 *  @param bool view Flag que indica o estado (1 ativo/ 0 inativo)
 */
void  TMOUT_GUEST_view_SET ( bool view )
{
  DISPLAY_VIEW_STS.TMOUT_guest = view;
}

/*
 * Atualiza o estado da flag de controle
 * em memoria para o PLAY CLOCK
 *  @param bool view Flag que indica o estado (1 ativo/ 0 inativo)
 */
void  PLAY_CLOCK_view_SET ( bool view )
{
  DISPLAY_VIEW_STS.PLAY_CLOCK = view;
}

/*
 * NOOP
 */
void  DISPLAY_BLANK_refresh (){}

void  SYS_COUNTER_100ms_increment ()
{
  SYS_COUNTER_100ms++;

  if ( SYS_COUNTER_100ms > 1000 )
  {
    SYS_COUNTER_100ms = 0;
  }
}

/*
 * Inicializa o cronometro HMI 100ms
 */
void  SYS_100ms_CRONO_startup ()
{
  CRONO_init ( &SYS_100ms_CRONO, SYS_100ms_CRONO_RATE );
}

/*
 * Coloca o cronometro 100ms HMI em execucao
 */
void  SYS_100ms_CRONO_run ()
{
  CRONO_run ( &SYS_100ms_CRONO );
}

/*
 * Controla a cadencia do cronometro 
 * 100ms HMI
 */
void  SYS_100ms_CRONO_cadence ()
{
  CRONO_atualize ( &SYS_100ms_CRONO );

  if ( CRONO_trigger_CHECK ( &SYS_100ms_CRONO ) )
  {
    SYS_COUNTER_100ms_increment ();

    CRONO_detected ( &SYS_100ms_CRONO );
  }
}

/*
 * Inicializa o cronometro de taxa de atualizacao do
 * display
 */
void  DISPLAY_refresh_startup ()
{
  CRONO_init ( &DISPLAY_refresh_CRONO, DISPLAY_refresh_RATE );
}

/*
 * Pausa o cronometro de taxa de atualizacao do
 * display
 */
void  DISPLAY_refresh_stop ()
{
  CRONO_stop ( &DISPLAY_refresh_CRONO );
}

/*
 * Coloca o cronometro de taxa de atualizacao 
 * de display em estado de execucao
 */
void  DISPLAY_refresh_run ()
{
  CRONO_run ( &DISPLAY_refresh_CRONO );
}

/*
 * Controla a cadencia do cronometro
 * de taxa de atualizacao de display
 */
void  DISPLAY_refresh_cadence ()
{
  CRONO_atualize ( &DISPLAY_refresh_CRONO );

  if ( CRONO_trigger_CHECK ( &DISPLAY_refresh_CRONO ) )
  {
    DISPLAY_refresh_force = true;

    CRONO_detected ( &DISPLAY_refresh_CRONO );
  }
}

/*
 * Atualiza o estado dos displays
 * de acordo com as configuracao
 * do cronometro de taxa de atualizacao
 */
void  DISPLAY_refresh_make ()
{
  if ( !DISPLAY_refresh_force )
  {
    return;   
  }
  DISPLAY_refresh_force = false;
  PLACAR_MAIN_DISPLAY_refresh ();
}

void  GAME_TIME_startup ()
{
  CRONO_init ( &GAME_TIME_CRONO, Second_ms );
}

void  GAME_TIME_notify_ON ( void (*GAME_TIME_notify) () )
{
  CRONO_notify_ON ( &GAME_TIME_CRONO, GAME_TIME_notify );
}

void  GAME_TIME_notify_OFF ()
{
  CRONO_notify_OFF ( &GAME_TIME_CRONO );
}

void  GAME_TIME_stop ()
{
  CRONO_stop ( &GAME_TIME_CRONO );
}

void  GAME_TIME_run ()
{
  CRONO_run ( &GAME_TIME_CRONO );
}

bool  GAME_TIME_running_CHECK ()
{
  return ( CRONO_running_CHECK ( &GAME_TIME_CRONO ) );
}

void  GAME_TIME_cadence ()
{
  if ( ( GAME_TIME_seg != 0 ) || ( GAME_TIME_min != 0 ) )
  {
    CRONO_atualize ( &GAME_TIME_CRONO );

    if ( CRONO_trigger_CHECK ( &GAME_TIME_CRONO ) )
    {
      if ( GAME_TIME_seg > 0 )
      {
        GAME_TIME_seg--;
      }
      else
      {
        GAME_TIME_seg = 59;

        GAME_TIME_min--;
      }

      CRONO_detected ( &GAME_TIME_CRONO );
    }
  }
  else  CRONO_stop ( &GAME_TIME_CRONO );
}

/*
 * Limpa o buffer de comandos
 */
void  CMD_BUFF_reset ()
{
  CMD_BUFF_idx = 0;

  for (BYTE i=0; i< CMD_size; i++ )
  {
    CMD_BUFF [i] = 0;
  }
}

/*
 * Adiciona um caracter ao buffer de
 * comandos
 */
void  CMD_BUFF_ins ( BYTE new_char )
{
  if ( CMD_BUFF_idx < CMD_size )
  {
    CMD_BUFF [CMD_BUFF_idx] = LOW_CASE_conv (new_char);

    CMD_BUFF_idx++;
  }
}

/*
 * Le um caracter de comando 
 * da interface serial
 *  @returns bool True caso seja o caracter de fim de comando
 *                False caso contrario
 */
bool  COMMAND_get ()
{
  
  if ( 0 >= Serial.available() )
  {
    return false;
  } 
  
  BYTE rcv_char = Serial.read();

  if ( CMD_end == rcv_char )
  {
    return true;
  }
    
  CMD_BUFF_ins (rcv_char);
  
  return false;
}

/*
 * Localiza o comando a ser executado de acordo 
 * com o tipo de comando enviado memorizado
 * no buffer de comandos
 *  @returns bool True caso o comando tenha sido localizado
 *                False caso contrario
 */
bool  COMMAND_decode ( struct CMD_LOC_info *CMD_list_PTR )
{
  char CMD_code, LOC_code;
  BYTE CMD_idx;
  
  CMD_code = CMD_BUFF [0];

  CMD_idx = 0;

  do
  {
    LOC_code = ROM_get8( &CMD_list_PTR [CMD_idx].CMD_code );

    if ( LOC_code == CMD_code )
    {
      SYS_CMD_PTR = (CMD_func_PTR) ROM_PTR_get( &CMD_list_PTR [CMD_idx].CMD_func );
      return true;
    }
    
    CMD_idx++;
    

  } while ( LOC_code != CMD_LIST_end_mark );

  return false;
}

/*
 * Envia quebras de linha pela
 * interface serial
 *  @param BYTE n Numero de quebras de linha
 *                a ser enviado
 */
void  SERIAL_newline_send ( BYTE n )
{
  for ( ; n > 0; n-- ) Serial.println ();
}

/*
 * Resposta de falha a um comando 
 * enviado
 */
void  COMMAND_FAIL_reply ()
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_LF_print ("comando ignorado!!!");
    SERIAL_newline_send (1);
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_LF_print ("#0");
  }
}

/*
 * Resposta de sucesso a um comando enviado
 */
void  COMMAND_OK_reply ()
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_LF_print ("ok");
    SERIAL_newline_send (1);
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_LF_print ("#1");
  }
}

/*
 * Executa o comando interpretado
 * pelo sistema recebido
 * via interface serial
 */
void  COMMAND_exec ()
{
  bool ok = SYS_CMD_PTR ( CMD_BUFF, 1 ); // executa o comando
  if ( ! ok ) { 
     COMMAND_FAIL_reply ();
     return;
  }
  COMMAND_OK_reply ();    // resposta de sucesso
}

/*
 * Processa comandos recebidos
 * via interface serial
 */
void  COMMAND_proc ( struct CMD_LOC_info *CMD_list_PTR )
{
  if ( ! COMMAND_get () )
  {
    return;    
  }

  bool ok =  COMMAND_decode ( CMD_list_PTR );
  
  if ( ! ok ){
    
    COMMAND_FAIL_reply ();
    return;
 
  }
    
  COMMAND_exec ();
  CMD_BUFF_reset ();
    
}
/*
 * Verifica se o indice de referencia
 * aponta para o byte de final de comando
 *  @param BYTE_PTR CMD_data Referencia para o buffer com dados do comando
 *  @param BYTE index Indice de referencia para a posicao do buffer
 *  @returns bool True caso o byte na posicao seja fim de comando
 *                False caso contrario
 */
bool  END_CMD_CHK ( BYTE_PTR CMD_data, BYTE index )
{
  BYTE VAL_1 = CMD_data [index];
  return ( VAL_1 == 0 ) || ( VAL_1 == 0x0D );
}


/*
 * Escreve na serial de comando  o GAME QUARTER
 *  @param BYTE value Valor do GAME QUARTER
 */
void  QUARTER_inform ( BYTE value )
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_print ("GAME QUARTER = ");
    Serial.println ( value, DEC );
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_print ("#q");
    Serial.println ( value, DEC );
  }
}

/*
 *  Execucao do comando "qX"
 *  Especifica o "QUARTER" atual.
 *   @param BYTE_PTR CMD_data Buffer com dados do comando
 *   @param BYTE index Posicao atual de leitura do buffer
 *   @returns bool True caso a execucao seja bem sucedida
 *                 False caso contrario
 */
bool  qX_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  BYTE VAL_1;
  bool OK = false;

  if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
  {
    if ( END_CMD_CHK ( CMD_data, index ) )
    {
      VAL_1 = BCD8_to_HEX8 ( VAL_1 );

      if ( QUARTER_value_CHK ( VAL_1 ) )
      {
        GAME_quarter = VAL_1;

        QUARTER_inform ( GAME_quarter );

        OK = true;
      }
    }
  }

  return (OK);
}

/*
 * Escreve na serial de comando  o GUEST VANTAGE
 */
void  GUEST_VANTAGE_inform ()
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_LF_print ("GUEST vantage!");
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_LF_print ("#lv");
  }
}

/*
 * Escreve na serial de comando  o HOME VANTAGE
 */
void  HOME_VANTAGE_inform ()
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_LF_print ("HOME vantage!");
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_LF_print ("#lc");
  }
}


/*
 *  Execucao do comando "lt"
 *  Troca a Vantagem atual.
 *   @param BYTE_PTR Buffer com os dados do comando
 *   @pararm BYTE index Posicao de leitura do buffer
 *   @returns bool True caso a execucao seja bem sucedida
 *                 False caso contrario
 */

bool  lt_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  if ( END_CMD_CHK ( CMD_data, index ) )
  {
    if ( GAME_VANTAGE == GUEST_VANTAGE )
    {
      GAME_VANTAGE = HOME_VANTAGE;

      HOME_VANTAGE_inform ();
    }
    else
    {
      GAME_VANTAGE = GUEST_VANTAGE;

      GUEST_VANTAGE_inform ();
    }

    OK = true;
  }

  return (OK);
}

/*
 *  Execucao do comando "lv"
 *  Seleciona a "Vantagem visitante".
 *   @param BYTE_PTR CMD_data Buffer com dados do comando
 *   @param BYTE index Posicao de leitura do buffer
 *   @returns bool True caso a execucao seja bem sucedida
 *                 False caso contrario
 */

bool  lv_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  if ( END_CMD_CHK ( CMD_data, index ) )
  {
    GAME_VANTAGE = GUEST_VANTAGE;

    GUEST_VANTAGE_inform ();

    OK = true;
  }

  return (OK);
}

/*
 *  Execucao do comando "lc": 
 *  Seleciona a "Vantagem casa".
 *   @param BYTE_PTR CMD_data Buffer com dados do comando
 *   @param BYTE index Posicao de leitura do buffer
 *   @returns bool True caso a excucao seja bem sucedida
 *                False caso contrario
 */

bool  lc_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  if ( END_CMD_CHK ( CMD_data, index ) )
  {
    GAME_VANTAGE = HOME_VANTAGE;

    HOME_VANTAGE_inform ();

    OK = true;
  }

  return (OK);
}

/*
 * Escreve na serial de comando  um valor de TIMEOUT
 *  @param BYTE value Valor a ser escrito
 */
void  TIMEOUT_guest_inform ( BYTE value )
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_print ("TIMEOUT GUEST = ");
    Serial.println ( value, DEC );
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_print ("#tv");
    Serial.println ( value, DEC );
  }
}

/*
 * Escreve na serial de comando  um valor de TIMEOUT HOME
 *  @param BYTE value Valor a ser escrito
 */
void  TIMEOUT_home_inform ( BYTE value )
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_print ("TIMEOUT HOME = ");
    Serial.println ( value, DEC );
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_print ("#tc");
    Serial.println ( value, DEC );
  }
}

/*
 * Execucao do comando "tv-": 
 * Decrementa o "TIMEOUT visitante".
 *  @param BYTE_PTR CMD_data Buffer com dados do comando   
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  tv_DECR_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  if ( END_CMD_CHK ( CMD_data, index ) )
  {
    if ( TMOUT_guest != 0 )
    {
      TMOUT_guest--;

      TIMEOUT_guest_inform ( TMOUT_guest );

      OK = true;
    }
  }

  return (OK);
}

/*
 *  Execuca o comando "tc-": 
 *  Decrementa o "TIMEOUT casa".
 *    @param BYTE_PTR CMD_data Buffer com dados do comando
 *    @param BYTE index Posicao de leitura do buffer
 *    @returns bool True caso a execucao seja bem sucedida
 *                  False caso contrario
 */
bool  tc_DECR_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  if ( END_CMD_CHK ( CMD_data, index ) )
  {
    if ( TMOUT_home != 0 )
    {
      TMOUT_home--;

      TIMEOUT_home_inform ( TMOUT_home );

      OK = true;
    }
  }

  return (OK);
}

/*
 *  Executa o comando "tvXX": 
 *  Especifica o "TIMEOUT visitante".
 *   @param BYTE_PTR CMD_data Buffer com dados do comando
 *   @param BYTE index Posicao de leitura do buffer
 *   @returns bool True caso a execucao seja bem sucedida
 *                 False caso contrario
 */
bool  tvX_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  BYTE VAL_1;
  bool OK = false;

  if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
  {
    if ( END_CMD_CHK ( CMD_data, index ) )
    {
      VAL_1 = BCD8_to_HEX8 ( VAL_1 );

      if ( Timeout_value_CHK ( VAL_1 ) )
      {
        TMOUT_guest = VAL_1;

        TIMEOUT_guest_inform ( TMOUT_guest );

        OK = true;
      }
    }
  }

  return (OK);
}

/*
 * Executa o comando "tcXX": 
 * Especifica o "TIMEOUT casa".
 *  @param BYTE_PTR CMD_data Buffer com dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  tcX_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  BYTE VAL_1;
  bool OK = false;

  if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
  {
    if ( END_CMD_CHK ( CMD_data, index ) )
    {
      VAL_1 = BCD8_to_HEX8 ( VAL_1 );

      if ( Timeout_value_CHK ( VAL_1 ) )
      {
        TMOUT_home = VAL_1;

        TIMEOUT_home_inform ( TMOUT_home );

        OK = true;
      }
    }
  }

  return (OK);
}

/*
 * Escreve na serial de comando  o valor decrementado em SCORE GUEST
 * e o SCORE atualizado
 *  @param BYTE delta Valor de decremento
 *  @param BYTE score Valor atual de SCORE GUEST
 */
void  SCORE_guest_DECR_inform ( BYTE delta, BYTE score )
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_print ("SCORE GUEST decremented by ");
    Serial.println ( delta, DEC );
    ROM_SERIAL_print ("new SCORE GUEST = ");
    Serial.println ( score, DEC );
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_print ("#v");
    Serial.println ( score, DEC );
  }
}

/*
 * Escreve na serial de comando  o valor incrementado em SCORE GUEST
 * e o SCORE atualizado
 *  @param BYTE delta Valor de incremento
 *  @param BYTE score Valor atual de SCORE GUEST
 */
void  SCORE_guest_INCR_inform ( BYTE delta, BYTE score )
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_print ("SCORE GUEST incremented by ");
    Serial.println ( delta, DEC );
    ROM_SERIAL_print ("new SCORE GUEST = ");
    Serial.println ( score, DEC );
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_print ("#v");
    Serial.println ( score, DEC );
  }
}

/*
 * Escreve na serial de comando  o valor atual do SCORE GUEST
 *  @param BYTE score Valor atual de SCORE GUEST
 */
void  SCORE_guest_inform ( BYTE score )
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_print ("SCORE GUEST = ");
    Serial.println ( score, DEC );
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_print ("#v");
    Serial.println ( score, DEC );
  }
}

/*
 * Escreve na serial de comando  o valor decrementado em SCORE HOME
 * e o SCORE atualizado
 *  @param BYTE delta Valor de decremento
 *  @param BYTE score Valor atual de SCORE HOME
 */
void  SCORE_home_DECR_inform ( BYTE delta, BYTE score )
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_print ("SCORE HOME decremented by ");
    Serial.println ( delta, DEC );
    ROM_SERIAL_print ("new SCORE HOME = ");
    Serial.println ( score, DEC );
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_print ("#c");
    Serial.println ( score, DEC );
  }
}

/*
 * Escreve na serial de comando  o valor incrementado em SCORE HOME
 * e o SCORE atualizado
 *  @param BYTE delta Valor de incremento
 *  @param BYTE score Valor atual de SCORE HOME
 */
void  SCORE_home_INCR_inform ( BYTE delta, BYTE score )
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_print ("SCORE HOME incremented by ");
    Serial.println ( delta, DEC );
    ROM_SERIAL_print ("new SCORE HOME = ");
    Serial.println ( score, DEC );
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_print ("#c");
    Serial.println ( score, DEC );
  }
}

/*
 * Escreve na serial de comando  o valor atual do SCORE HOME
 *  @param BYTE score Valor atual de SCORE HOME
 */
void  SCORE_home_inform ( BYTE score )
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_print ("SCORE HOME = ");
    Serial.println ( score, DEC );
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_print ("#c");
    Serial.println ( score, DEC );
  }
}

/*
 * Execucao do comando "v-XX": 
 * Decrementa o "Placar visitante" em "XX" unidades.
 *  @param BYTE_PTR CMD_data Buffer com dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  vXX_DECR_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  BYTE delta;
  int new_score;
  bool OK = false;

  if ( BCD_8_NEXT_get ( CMD_data, &index, &delta ) )
  {
    if ( END_CMD_CHK ( CMD_data, index ) )
    {
      delta = BCD8_to_HEX8 ( delta );

      if ( SCORE_delta_value_CHK ( delta ) )
      {
        new_score = SCORE_guest - delta;

        if ( GAME_SCORE_value_CHK ( new_score ) )
        {
          SCORE_guest = new_score;

          SCORE_guest_DECR_inform ( delta, SCORE_guest );

          OK = true;
        }
      }
    }
  }

  return (OK);
}

/*
 *  Execucao do comando "v+XX": 
 *  Incrementa o "Placar visitante" em "XX" unidades.
 *    @param BYTE_PTR CMD_data Buffer com dados do comando
 *    @param BYTE index Posicao de leitura do buffer
 *    @returns bool True caso a execucao seja bem sucedida
 *                  False caso contrario
 */
bool  vXX_INCR_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  BYTE delta;
  int new_score;
  bool OK = false;

  if ( BCD_8_NEXT_get ( CMD_data, &index, &delta ) )
  {
    if ( END_CMD_CHK ( CMD_data, index ) )
    {
      delta = BCD8_to_HEX8 ( delta );

      if ( SCORE_delta_value_CHK ( delta ) )
      {
        new_score = SCORE_guest + delta;

        if ( GAME_SCORE_value_CHK ( new_score ) )
        {
          SCORE_guest = new_score;

          SCORE_guest_INCR_inform ( delta, SCORE_guest );

          OK = true;
        }
      }
    }
  }

  return (OK);
}

/*
 * Execucao do comando "vXX": 
 * Especifica o "Placar visitante".
 *  @param BYTE_PTR CMD_data Buffer com os dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  vXX_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  BYTE VAL_1;
  bool OK = false;

  if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
  {
    if ( END_CMD_CHK ( CMD_data, index ) )
    {
      VAL_1 = BCD8_to_HEX8 ( VAL_1 );

      if ( GAME_SCORE_value_CHK ( VAL_1 ) )
      {
        SCORE_guest = VAL_1;

        SCORE_guest_inform ( SCORE_guest );

        OK = true;
      }
    }
  }

  return (OK);
}

/*
 * Execucao do comando "c-XX": 
 * Decrementa o "Placar casa" em "XX" unidades.
 *  @param BYTE_PTR CMD_data Buffer com os dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  cXX_DECR_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  BYTE delta;
  int new_score;
  bool OK = false;

  if ( BCD_8_NEXT_get ( CMD_data, &index, &delta ) )
  {
    if ( END_CMD_CHK ( CMD_data, index ) )
    {
      delta = BCD8_to_HEX8 ( delta );

      if ( SCORE_delta_value_CHK ( delta ) )
      {
        new_score = SCORE_home - delta;

        if ( GAME_SCORE_value_CHK ( new_score ) )
        {
          SCORE_home = new_score;

          SCORE_home_DECR_inform ( delta, SCORE_home );

          OK = true;
        }
      }
    }
  }

  return (OK);
}

/*
 * Execucao do comando "c+XX": 
 * Incrementa o "Placar casa" em "XX" unidades.
 *  @param BYTE_PTR CMD_data Buffer com os dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  cXX_INCR_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  BYTE delta;
  int new_score;
  bool OK = false;

  if ( BCD_8_NEXT_get ( CMD_data, &index, &delta ) )
  {
    if ( END_CMD_CHK ( CMD_data, index ) )
    {
      delta = BCD8_to_HEX8 ( delta );

      if ( SCORE_delta_value_CHK ( delta ) )
      {
        new_score = SCORE_home + delta;

        if ( GAME_SCORE_value_CHK ( new_score ) )
        {
          SCORE_home = new_score;

          SCORE_home_INCR_inform ( delta, SCORE_home );

          OK = true;
        }
      }
    }
  }

  return (OK);
}

/*
 * Execucao do comando "cXX": 
 * Especifica o "Placar casa".
 *  @param BYTE_PTR CMD_data Buffer com os dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  cXX_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  BYTE VAL_1;
  bool OK = false;

  if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
  {
    if ( END_CMD_CHK ( CMD_data, index ) )
    {
      VAL_1 = BCD8_to_HEX8 ( VAL_1 );

      if ( GAME_SCORE_value_CHK ( VAL_1 ) )
      {
        SCORE_home = VAL_1;

        SCORE_home_inform ( SCORE_home );

        OK = true;
      }
    }
  }

  return (OK);
}

/*
 * Escreve na serial de comando  o valor atual da
 * cadencia do GAME TIME
 */
void  GAME_TIME_cadence_inform ()
{
  if ( CMD_reply == CMD_code_reply )  // se modo resposta "codificada":
  {
    ROM_SERIAL_print ("#gm");
    Serial.println ( GAME_TIME_min, DEC );
    ROM_SERIAL_print ("#gs");
    Serial.println ( GAME_TIME_seg, DEC );

    if ( ( GAME_TIME_seg == 0 ) && ( GAME_TIME_min == 0 ) )
    {
      ROM_SERIAL_LF_print ("#gp");
    }
  }
}

/*
 * Escreve na serial de comando  o valor do GAME TIME  
 * de acordo com min e seg especificados
 *  @param BYTE min Valor de minutos
 *  @param BYTE seg Valor de segundos
 */
void  GAME_TIME_inform ( BYTE min, BYTE seg )
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_print ("Game Time = ");

    if ( min < 10 ) ROM_SERIAL_print ("0");
    Serial.print ( min, DEC );

    ROM_SERIAL_print (":");

    if ( seg < 10 ) ROM_SERIAL_print ("0");
    Serial.println ( seg, DEC );
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_print ("#gm");
    Serial.println ( min, DEC );
    ROM_SERIAL_print ("#gs");
    Serial.println ( seg, DEC );
  }
}

/*
 * Execucao do comando "gXX:YY": 
 * Especifica o "Game Time" atual.
 *  @param BYTE_PTR CMD_data Buffer com dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  gXX_YY_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  BYTE VAL_1, VAL_2;  
  bool OK = false;

  if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
  {
    if ( CMD_data[index] == ':' )
    {
      index++;

      VAL_1 = BCD8_to_HEX8 ( VAL_1 );

      OK = true;
    }
  }

  if ( OK && BCD_8_NEXT_get ( CMD_data, &index, &VAL_2 ) )
  {
    OK = false;

    if ( END_CMD_CHK ( CMD_data, index ) )
    {
      VAL_2 = BCD8_to_HEX8 ( VAL_2 );

      OK = Game_Time_value_CHK ( VAL_1, VAL_2 );
    }
  }
  else  OK = false;

  if ( OK )
  {
    GAME_TIME_min = VAL_1;
    GAME_TIME_seg = VAL_2;

    GAME_TIME_inform ( GAME_TIME_min, GAME_TIME_seg );
  }

  return (OK);
}

/*
 * Escreve na serial de comando o estado de pausa
 * do GAME TIME
 */
void  GAME_TIME_stop_inform ()
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_LF_print ("Game Time stopped !!!");
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_LF_print ("#gp");
  }
}

/*
 * Execucao do comando "gp": 
 * Pausa a contagem do "Game Time".
 *  @param BYTE_PTR CMD_data Buffer com dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  gp_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  if ( END_CMD_CHK ( CMD_data, index ) )
  {
    GAME_TIME_stop ();

    GAME_TIME_stop_inform ();

    OK = true;
  }

  return (OK);
}

/*
 * Escreve na serial de comando o 
 * estado de execucao do GAME TIME
 */
void  GAME_TIME_run_inform ()
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_LF_print ("Game Time running !!!");
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_LF_print ("#gi");
  }
}

/*
 * Execucao do comando "gi": 
 * Inicia a contagem do "Game Time".
 *  @param BYTE_PTR CMD_data Buffer com dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  gi_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  if ( END_CMD_CHK ( CMD_data, index ) )
  {
    if ( ( GAME_TIME_seg != 0 ) || ( GAME_TIME_min != 0 ) )
    {
      GAME_TIME_run ();

      GAME_TIME_run_inform ();
    }

    OK = true;
  }

  return (OK);
}

/*
 * Escreve na serial de comando o valor 
 * de cadencia atual do PLAY CLOCK
 */
void  PLAY_CLOCK_cadence_inform ()
{
  if ( CMD_reply == CMD_code_reply )  // se modo resposta "codificada":
  {
    ROM_SERIAL_print ("#p");
    Serial.println ( PLAY_CLOCK, DEC );

    if ( PLAY_CLOCK == 0 )  ROM_SERIAL_LF_print ("#pp");
  }
}

/*
 * Escreve na serial de comando o estado
 * pausado de PLAY CLOCK on BALL 
 */
void  PLAY_CLOCK_swap_OFF_inform ()
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_LF_print ("PLAY CLOCK on BALL = OFF");
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_LF_print ("#pb0");
  }
}

/*
 * Escreve na seria de comando o estado
 * de execucao do PLAY CLOCK on BALL
 */
void  PLAY_CLOCK_swap_ON_inform ()
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_LF_print ("PLAY CLOCK on BALL = ON");
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_LF_print ("#pb1");
  }
}

/*
 * Controla a escrita na serial de comando
 * do estado de PLAY CLOCK on BALL
 *  @param bool swap Estado (1 swap ON/ 2 swap OFF)
 */
void  PLAY_CLOCK_swap_inform ( bool swap )
{
  if ( swap )

    PLAY_CLOCK_swap_ON_inform ();

  else  PLAY_CLOCK_swap_OFF_inform ();
}

/*
 * Escreve na serial de comando o valor 
 * de decremento de PLAY CLOCK
 *  @param BYTE delta Quantidade decrementada
 *  @param BYTE value Valor final de PLAY CLOCK
 */
void  PLAY_CLOCK_DECR_inform ( BYTE delta, BYTE value )
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_print ("PLAY CLOCK decremented by ");
    Serial.println ( delta, DEC );
    ROM_SERIAL_print ("new PLAY CLOCK = ");
    Serial.println ( value, DEC );
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_print ("#p");
    Serial.println ( value, DEC );
  }
}

/*
 * Escreve na serial de comando o valor de 
 * incremento do PLAY CLOCK
 *  @param BYTE delta Valor incrementado
 *  @param BYET value Valor final do PLAY CLOCK
 */
void  PLAY_CLOCK_INCR_inform ( BYTE delta, BYTE value )
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_print ("PLAY CLOCK incremented by ");
    Serial.println ( delta, DEC );
    ROM_SERIAL_print ("new PLAY CLOCK = ");
    Serial.println ( value, DEC );
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_print ("#p");
    Serial.println ( value, DEC );
  }
}

/*
 * Escreve na serial de comando um 
 * valor de PLAY CLOCK
 *  @param BYTE value Valor do PLAY CLOCK
 */
void  PLAY_CLOCK_inform ( BYTE value )
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_print ("PLAY CLOCK = ");
    Serial.println ( value, DEC );
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_print ("#p");
    Serial.println ( value, DEC );
  }
}

/*
 * Execucao do comando "p-XX": 
 * Decrementa o "Play Clock" em "XX" unidades.
 *  @param BYTE_PTR CMD_data Buffer com dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  pXX_DECR_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  BYTE delta;
  int new_PCLOCK;
  bool OK = false;

  if ( BCD_8_NEXT_get ( CMD_data, &index, &delta ) )
  {
    if ( END_CMD_CHK ( CMD_data, index ) )
    {
      delta = BCD8_to_HEX8 ( delta );

      if ( PCLOCK_delta_value_CHK ( delta ) )
      {
        new_PCLOCK = PLAY_CLOCK - delta;

        if ( PLAY_CLOCK_value_CHK ( new_PCLOCK ) )
        {
          PLAY_CLOCK = new_PCLOCK;

          PLAY_CLOCK_DECR_inform ( delta, PLAY_CLOCK );

          OK = true;
        }
      }
    }
  }

  return (OK);
}

/*
 * Execucao do comando "p+XX": 
 * Incrementa o "Play Clock" em "XX" unidades.
 *  @param BYTE_PTR CMD_data Buffer com dados do comando 
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  pXX_INCR_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  BYTE delta;
  int new_PCLOCK;
  bool OK = false;

  if ( BCD_8_NEXT_get ( CMD_data, &index, &delta ) )
  {
    if ( END_CMD_CHK ( CMD_data, index ) )
    {
      delta = BCD8_to_HEX8 ( delta );

      if ( PCLOCK_delta_value_CHK ( delta ) )
      {
        new_PCLOCK = PLAY_CLOCK + delta;

        if ( PLAY_CLOCK_value_CHK ( new_PCLOCK ) )
        {
          PLAY_CLOCK = new_PCLOCK;

          PLAY_CLOCK_INCR_inform ( delta, PLAY_CLOCK );

          OK = true;
        }
      }
    }
  }

  return (OK);
}

/*
 * Execucao do comando "pXX": 
 * Especifica o "Play Clock".
 *  @param BYTE_PTR CMD_data Buffer com dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  pXX_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  BYTE VAL_1;
  bool OK = false;

  if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
  {
    if ( END_CMD_CHK ( CMD_data, index ) )
    {
      VAL_1 = BCD8_to_HEX8 ( VAL_1 );

      if ( PLAY_CLOCK_value_CHK ( VAL_1 ) )
      {
        PLAY_CLOCK = VAL_1;

        PLAY_CLOCK_inform ( PLAY_CLOCK );

        OK = true;
      }
    }
  }

  return (OK);
}

/*
 * Execucao de comando "p": 
 * Seta o "Play Clock" em "40".
 */
bool  p_CMD ()
{
  PLAY_CLOCK = 40;

  PLAY_CLOCK_inform ( PLAY_CLOCK );

  return (true);
}

/*
 * Comando "pb": 
 * Alterna o modo "swap" entre "Play Clock" e "Ball".
 *  @param BYTE_PTR CMD_data Buffer com dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  pb_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  if ( END_CMD_CHK ( CMD_data, index ) )
  {
    PLAY_CLOCK_swap = !PLAY_CLOCK_swap;

    PLAY_CLOCK_swap_inform ( PLAY_CLOCK_swap );

    OK = true;
  }

  return (OK);
}

/*
 * Escreve na serial de comando o
 * estado de pausa do PLAY CLOCK
 */
void  PLAY_CLOCK_stop_inform ()
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_LF_print ("PLAY CLOCK stopped !!!");
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_LF_print ("#pp");
  }
}

/*
 * Comando "pp": 
 * Pausa/Interrompe a contagem do "Play Clock".
 *  @param BYTE_PTR CMD_data Buffer com dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  pp_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  if ( END_CMD_CHK ( CMD_data, index ) )
  {
    PLAY_CLOCK_stop ();

    PLAY_CLOCK_stop_inform ();

    OK = true;
  }

  return (OK);
}

/*
 * Escreve na serial de comando o estado
 * de execucao/reinicializado do PLAY CLOCK
 */
void  PLAY_CLOCK_run_inform ()
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_LF_print ("PLAY CLOCK running !!!");
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_LF_print ("#pi");
  }
}

/*
 * Execucao do comando "pi": 
 * Inicia/Reinicializa a contagem do "Play Clock".
 *  @param BYTE_PTR CMD_data Buffer com dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  pi_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  if ( END_CMD_CHK ( CMD_data, index ) )
  {
    if ( PLAY_CLOCK != 0 )
    {
      PLAY_CLOCK_run ();

      PLAY_CLOCK_run_inform ();
    }

    OK = true;
  }

  return (OK);
}


/*
 * Escreve na serial de comando um valor
 * de BALL POSITION
 *  @param BYTE position Valor do BALL POSITION
 */
void  BALL_POSITION_inform ( BYTE position )
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_print ("BALL on: ");
    Serial.println ( position, DEC );
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_print ("#bb");
    Serial.println ( position, DEC );
  }
}

/*
 * Execucao do comando "bbXX": 
 * Especifica a posicao da bola.
 *  @param BYTE_PTR CMD_data Buffer com dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  bbXX_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  BYTE VAL_1;
  bool OK = false;

  if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
  {
    if ( END_CMD_CHK ( CMD_data, index ) )
    {
      VAL_1 = BCD8_to_HEX8 ( VAL_1 );

      if ( BALL_POS_value_CHK ( VAL_1 ) )
      {
        BALL_position = VAL_1;

        BALL_POSITION_inform ( BALL_position );

        OK = true;
      }
    }
  }

  return (OK);
}

/*
 * Escreve na serial de comando um 
 * valor para o BALL TO GO
 *  @param BYTE to_GO Valor do BALL TO GO
 */
void  to_GO_inform ( BYTE to_GO )
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_print ("TO GO = ");
    Serial.println ( to_GO, DEC );
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_print ("#bg");
    Serial.println ( to_GO, DEC );
  }
}

/*
 * Execucao do comando "bgXX": 
 * Especifica o "to GO" atual.
 *  @param BYTE_PTR CMD_data Buffer com dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  bgXX_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  BYTE VAL_1;
  bool OK = false;

  if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
  {
    if ( END_CMD_CHK ( CMD_data, index ) )
    {
      VAL_1 = BCD8_to_HEX8 ( VAL_1 );

      if ( to_GO_value_CHK ( VAL_1 ) )
      {
        BALL_to_go = VAL_1;

        to_GO_inform ( BALL_to_go );

        OK = true;
      }
    }
  }

  return (OK);
}

/*
 * Escreve na serial de comando um valor 
 * de BALL DOWN
 *  @param BYTE down Valor do BALL DOWN
 */
void  BALL_down_inform ( BYTE down )
{
  if ( CMD_reply == CMD_full_reply )  // modo resposta "completa":
  {
    ROM_SERIAL_print ("DOWN = ");
    Serial.println ( down, DEC );
  }
  else if ( CMD_reply == CMD_code_reply ) // modo resposta "codificada":
  {
    ROM_SERIAL_print ("#bd");
    Serial.println ( down, DEC );
  }
}

/*
 * Execucao do Comando "bdX": 
 * Especifica o "down" atual.
 *  @param BYTE_PTR CMD_data Buffer com dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  bdX_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  BYTE VAL_1;
  bool OK = false;

  if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
  {
    if ( END_CMD_CHK ( CMD_data, index ) )
    {
      VAL_1 = BCD8_to_HEX8 ( VAL_1 );

      if ( DOWN_value_CHK ( VAL_1 ) )
      {
        BALL_down = VAL_1;

        BALL_down_inform ( BALL_down );

        OK = true;
      }
    }
  }

  return (OK);
}

/*
 * Execucao do comando "bXX,Y,ZZ": especifica a posicao da bola, o "down",
 * e "to GO".
 *  @param BYTE_PTR CMD_data Buffer com dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  bXX_Y_ZZ_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  BYTE VAL_1, VAL_2, VAL_3;
  bool OK = false;

  if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
  {
    if ( CMD_data[index] == ',' )
    {
      index++;

      VAL_1 = BCD8_to_HEX8 ( VAL_1 );

      OK = BALL_POS_value_CHK ( VAL_1 );
    }
  }

  if ( OK && BCD_8_NEXT_get ( CMD_data, &index, &VAL_2 ) )
  {
    OK = false;

    if ( CMD_data[index] == ',' )
    {
      index++;

      VAL_2 = BCD8_to_HEX8 ( VAL_2 );

      OK = DOWN_value_CHK ( VAL_2 );
    }
  }
  else  OK = false;

  if ( OK && BCD_8_NEXT_get ( CMD_data, &index, &VAL_3 ) )
  {
    OK = false;

    if ( END_CMD_CHK ( CMD_data, index ) )
    {
      VAL_3 = BCD8_to_HEX8 ( VAL_3 );

      OK = to_GO_value_CHK ( VAL_3 );
    }
  }
  else  OK = false;

  if ( OK )
  {
    BALL_position = VAL_1;
    BALL_POSITION_inform ( BALL_position );

    BALL_down = VAL_2;
    BALL_down_inform ( BALL_down );

    BALL_to_go = VAL_3;
    to_GO_inform ( BALL_to_go );
  }

  return (OK);
}

/*
 * Controla a execucao de comandos recebidos 
 * pela interface serial de comandos para controle
 * de VANTAGEM CASA
 *  @param BYTE_PTR CMD_data Buffer lido da interface serial de comandos
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao de um comando interpretado seja bem sucedida
 *                False caso contrario
 */
bool  L_MAIN_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  switch ( CMD_data [index] )
  {

    case  'c':
      index++;
      OK = lc_CMD ( CMD_data, index );
      break;

    case  'v':
      index++;
      OK = lv_CMD ( CMD_data, index );
      break;

    case  't':
      index++;
      OK = lt_CMD ( CMD_data, index );
      break;

    default:
      break;
  }

  return (OK);
}

/*
 * Execucao do comando "@0": 
 * Desliga o Display.
 *  @param BYTE_PTR CMD_data Buffer com dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  Display_OFF_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  if ( END_CMD_CHK ( CMD_data, index ) )
  {
    DISPLAY_view_SET ( VIEW_OFF );
    OK = true;
  }

  return (OK);
}

/*
 * Execucao do comando "@1": 
 * Liga o Display.
 *  @param BYTE_PTR CMD_data Buffer com dados do comando 
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  Display_ON_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  if ( END_CMD_CHK ( CMD_data, index ) )
  {
    DISPLAY_view_SET ( VIEW_ON );
    OK = true;
  }

  return (OK);
}

/*
 * Execucao do comando "$c": 
 * Seleciona resposta "codificada" aos comandos
 * recebidos.
 *  @param BYTE_PTR CMD_data Buffer com dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao do comando seja bem sucedida
 *                False caso contrario
 */
bool  code_reply_SET_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  if ( END_CMD_CHK ( CMD_data, index ) )
  {
    CMD_reply = CMD_code_reply;
    OK = true;
  }

  return (OK);
}

/*
 * Execucao do comando "$f": 
 * Seleciona resposta "completa" aos comandos
 * recebidos.
 *  @param BYTE_PTR CMD_data Buffer com dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  full_reply_SET_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  if ( END_CMD_CHK ( CMD_data, index ) )
  {
    CMD_reply = CMD_full_reply;

    OK = true;
  }

  return (OK);
}

/*
 * Execucao do comando "$n": 
 * Seleciona sem resposta aos comandos recebidos.
 *  @param BYTE_PTR CMD_data Buffer com dados do comando
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao seja bem sucedida
 *                False caso contrario
 */
bool  no_reply_SET_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  if ( END_CMD_CHK ( CMD_data, index ) )
  {
    CMD_reply = CMD_no_reply;
    OK = true;
  }

  return (OK);
}
/*
 * Inicialiazacao da placa 
 * com os modos de operacao
 * dos pinos e estado inicial
 * dos mesmos
 */
void  SYS_HARDWARE_init (){
  Serial.begin(9600);
}

/*
 * Inicializacao do SISTEMA 
 */
void  SYS_init ()
{
  //inicializacao de pinos
  SYS_HARDWARE_init ();

  //inicializacao de cronometros de cadencia
  SYS_100ms_CRONO_startup ();
  SYS_100ms_CRONO_run ();

  //estado inicial dos displays
  DISPLAY_view_SET ( VIEW_OFF );

  //inicializacao de cronometros de cadencia dos displays
  DISPLAY_refresh_startup ();
  DISPLAY_refresh_run ();
}

/*
 * Controla a cadencia configurada do
 * SISTEMA
 */
void  SYS_cadence ()
{
  
  SYS_100ms_CRONO_cadence ();

  DISPLAY_refresh_cadence ();
  DISPLAY_refresh_make ();
  
}

/*
 * Configura o estado de inicializacao 
 * do PLACAR
 */
void  PLACAR_SETTING_init ()
{
  GAME_TIME_min = 12; // seta os minutos do tempo de jogo (00..59).
  GAME_TIME_seg = 00; // seta os segundos do tempo de jogo (00..59).


  BALL_position = 0;  // seta a posicao atual da Bola (00..99).

  BALL_down = 1;    // seta o "down" da Bola (0..9).

  BALL_to_go = 00;  // seta o "to go" da Bola (00..99).


  SCORE_home = 00;  // seta a pontuacao da "casa" (00..99).

  TMOUT_home = 3;   // seta o "timeout" da "casa" (0..3).


  SCORE_guest = 00; // seta a pontuacao do "visitante" (00..99).

  TMOUT_guest = 3;  // seta o "timeout" do "visitante" (0..3).


  GAME_VANTAGE = HOME_VANTAGE;  // seta a vantagem atual no jogo 
                                // ( HOME_VANTAGE ou GUEST_VANTAGE ).


  GAME_quarter = 1; // seta o "quarter" do jogo (1, 2, 3, ou 4).


  PLAY_CLOCK = 40;  // seta o "Play Clock" atual do jogo (00..99).
}

void rxSetup(){

  SYS_init ();    //  <==== setar Pullups ou OUTs p/ pinos nao usados.
                  //    (checar padrao de startup do Arduino)
  
  PLACAR_SETTING_init ();
  
  DISPLAY_refresh_force = true;
  DISPLAY_refresh_make ();
  
  
  PLAY_CLOCK_startup ();    
  PLAY_CLOCK_notify_ON ( PLAY_CLOCK_cadence_inform );
  
  GAME_TIME_startup ();
  GAME_TIME_notify_ON ( GAME_TIME_cadence_inform );

  ROM_SERIAL_LF_print("Inicializando controladora do placar...");
  Serial.println();

}

void rxLoop(){

  COMMAND_proc ( (CMD_LOC_type*) CMD_list );

  PLAY_CLOCK_cadence ();

  GAME_TIME_cadence ();

  SYS_cadence ();

}

/*
 * Controla a execucao de comandos recebidos 
 * pela interface serial de comandos para controle
 * de POSICAO DA BOLA
 *  @param BYTE_PTR CMD_data Buffer lido da interface serial de comandos
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao de um comando interpretado seja bem sucedida
 *                False caso contrario
 */
bool  B_MAIN_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  switch ( CMD_data [index] )
  {

    case  'b':
      index++;
      OK = bbXX_CMD ( CMD_data, index );
      break;

    case  'd':
      index++;
      OK = bdX_CMD ( CMD_data, index );
      break;

    case  'g':
      index++;
      OK = bgXX_CMD ( CMD_data, index );
      break;

    default:
      OK = bXX_Y_ZZ_CMD ( CMD_data, index );
      break;
  }

  return (OK);
}
/*
 * Controla a execucao de comandos recebidos 
 * pela interface serial de comandos para controle
 * de TIMEOUT CASA
 *  @param BYTE_PTR CMD_data Buffer lido da interface serial de comandos
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao de um comando interpretado seja bem sucedida
 *                False caso contrario
 */
bool  T_MAIN_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  switch ( CMD_data [index] )
  {

    case  'c':
      index++;
      if ( CMD_data [index] == '-')
      {
        index++;

        OK = tc_DECR_CMD ( CMD_data, index );
      }
      else  OK = tcX_CMD ( CMD_data, index );
      break;

    case  'v':
      index++;
      if ( CMD_data [index] == '-')
      {
        index++;

        OK = tv_DECR_CMD ( CMD_data, index );
      }
      else  OK = tvX_CMD ( CMD_data, index );
      break;

    default:
      break;
  }

  return (OK);
}
/*
 * Controla a execucao de comandos recebidos 
 * pela interface serial de comandos para controle
 * do PLACAR CASA
 *  @param BYTE_PTR CMD_data Buffer lido da interface serial de comandos
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao de um comando interpretado seja bem sucedida
 *                False caso contrario
 */
bool  C_MAIN_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  switch ( CMD_data [index] )
  {

    case  '+':
      index++;
      OK = cXX_INCR_CMD ( CMD_data, index );
      break;

    case  '-':
      index++;
      OK = cXX_DECR_CMD ( CMD_data, index );
      break;

    default:
      OK = cXX_CMD ( CMD_data, index );
      break;
      
  }

  return (OK);
}
/*
 * Controla a execucao de comandos recebidos 
 * pela interface serial de comandos para controle
 * do PLACAR VISITANTE
 *  @param BYTE_PTR CMD_data Buffer lido da interface serial de comandos
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao de um comando interpretado seja bem sucedida
 *                False caso contrario
 */

bool  V_MAIN_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  switch ( CMD_data [index] )
  {

    case  '+':
      index++;
      OK = vXX_INCR_CMD ( CMD_data, index );
      break;

    case  '-':
      index++;
      OK = vXX_DECR_CMD ( CMD_data, index );
      break;

    default:
      OK = vXX_CMD ( CMD_data, index );
      break;
      
  }

  return (OK);
}
/*
 * Controla a execucao de comandos recebidos 
 * pela interface serial de comandos para controle
 * de GAME QUARTER
 *  @param BYTE_PTR CMD_data Buffer lido da interface serial de comandos
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao de um comando interpretado seja bem sucedida
 *                False caso contrario
 */
bool  Q_MAIN_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  switch ( CMD_data [index] )
  {
    case  '*':
      index++;
//      OK = yyy_CMD ( CMD_data, index );
      break;

    case  '/':
      index++;
//      OK = yyy_CMD ( CMD_data, index );
      break;

    default:
      OK = qX_CMD ( CMD_data, index );
      break;
  }

  return (OK);
}
/*
 * Controla a execucao de comandos recebidos 
 * pela interface serial de comandos para controle
 * de GAME TIME
 *  @param BYTE_PTR CMD_data Buffer lido da interface serial de comandos
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao de um comando interpretado seja bem sucedida
 *                False caso contrario
 */
bool  G_MAIN_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  switch ( CMD_data [index] )
  {
    case  'i':
      index++;
      OK = gi_CMD ( CMD_data, index );
      break;

    case  'p':
      index++;
      OK = gp_CMD ( CMD_data, index );
      break;

    default:
      OK = gXX_YY_CMD ( CMD_data, index );
      break;
  }

  return (OK);
}
/*
 * Controla a execucao de comandos recebidos 
 * pela interface serial de comandos para controle
 * de PLAY CLOCK
 *  @param BYTE_PTR CMD_data Buffer lido da interface serial de comandos
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao de um comando interpretado seja bem sucedida
 *                False caso contrario
 */
bool  P_MAIN_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  switch ( CMD_data [index] )
  {
  
    case  'i':
      index++;
      OK = pi_CMD ( CMD_data, index );
      break;
      
    case  'p':
      index++;
      OK = pp_CMD ( CMD_data, index );
      break;
      
    case  '+':
      index++;
      OK = pXX_INCR_CMD ( CMD_data, index );
      break;
      
    case  '-':
      index++;
      OK = pXX_DECR_CMD ( CMD_data, index );
      break;
    case  'b':
      index++;
      OK = pb_CMD ( CMD_data, index );
      break;
      
    default:
      if ( END_CMD_CHK ( CMD_data, index ) )
      {
        OK = p_CMD ();
      }
      else  OK = pXX_CMD ( CMD_data, index );
      break;
      
  }

  return (OK);
}
/*
 * Controla a execucao de comandos recebidos 
 * pela interface serial de comandos para controle
 * de ESTADO DO DISPLAY
 *  @param BYTE_PTR CMD_data Buffer lido da interface serial de comandos
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao de um comando interpretado seja bem sucedida
 *                False caso contrario
 */
bool  Display_ON_OFF_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  switch ( CMD_data [index] )
  {

    case  '0':  // comando para desligar o Display.
      index++;
      OK = Display_OFF_CMD ( CMD_data, index );
      break;

    case  '1':  // comando para ligar o Display.
      index++;
      OK = Display_ON_CMD ( CMD_data, index );
      break;

    default:
      break;
  }

  return (OK);
}
/*
 * Controla a execucao de comandos recebidos 
 * pela interface serial de comandos para controle
 * do REPLY MODE (Modo de resposta)
 *  @param BYTE_PTR CMD_data Buffer lido da interface serial de comandos
 *  @param BYTE index Posicao de leitura do buffer
 *  @returns bool True caso a execucao de um comando interpretado seja bem sucedida
 *                False caso contrario
 */
bool  REPLY_mode_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  bool OK = false;

  switch ( CMD_data [index] )
  {

    case  'n':  // especifica "sem resposta" aos comandos recebidos.
      index++;
      OK = no_reply_SET_CMD ( CMD_data, index );
      break;

    case  'f':  // especifica resposta "completa" aos comandos recebidos.
      index++;
      OK = full_reply_SET_CMD ( CMD_data, index );
      break;

    case  'c':  // especifica resposta "codificada" aos comandos recebidos.
      index++;
      OK = code_reply_SET_CMD ( CMD_data, index );
      break;

    default:
      break;
  }

  return (OK);
}
/**
 * Dummy command
 * Emulacao de comando
 */
bool  DUMMY_CMD ( BYTE_PTR CMD_data, BYTE index )
{
  return (true);
}
