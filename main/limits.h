/*
  limits.h - Definicoes de limites do sistema
             e funcoes de verificacao
  @Criado Renato Albani dos Santos <renato.utfpr@gmail.com> Outubro 2018           
 */
#ifndef limits_h
#define limits_h
#include <Arduino.h>
#include "types.h"
/*
 * Definicao dos limites 
 * relacionados a QUARTER
 */
#define QUARTER_MIN  1
#define QUARTER_MAX 4
/*
 * Definicao dos limites para TIMEOUT
 */
#define Timeout_MIN  0
#define Timeout_MAX 3
/*
 * Define os limites de GAME SCORE
 */
#define GAME_SCORE_MIN  0
#define GAME_SCORE_MAX  99

/*
 * Define os limites do SCORE Delta 
 */
#define SCORE_delta_MIN 0
#define SCORE_delta_MAX 10
/*
 * Definicao de limites
 * para o PLAY CLOCK
 */
#define PLAY_CLOCK_MIN  0
#define PLAY_CLOCK_MAX  40

/*
 * Definicao de limites 
 * para o decremento de
 * PLAY CLOCK
 */
#define PCLOCK_delta_MIN  0
#define PCLOCK_delta_MAX  20
/*
 * Define os limites  
 * para o BALL POSITION
 */
#define BALL_pos_MIN  0
#define BALL_pos_MAX  50
/*
 * Definicao do limites  
 * para o BALL TO GO
 */
#define to_GO_MIN  0
#define to_GO_MAX 99
/*
 * Definicao dos limites
 * para BALL DOWN
 */
#define down_MIN  1
#define down_MAX  4
/*
 * Definicao de limites
 * para GAME TIME
 */
#define Game_Time_MAX_minutos  15
#define Game_Time_MAX_seg 60*Game_Time_MAX_minutos
/*
 * Verifica se uma valor esta dentro
 * dos limites de QUARTER
 *  @param BYTE value Valor a ser testado
 *  @returns bool True caso esteja dentro dos limites
 *                False caso contrario
 */
extern bool  QUARTER_value_CHK ( BYTE value );
/*
 * Verifica se um valor esta dentro
 * dos limites de TIMEOUT
 *  @param BYTE value Valor a ser verificado
 *  @returns bool True caso o valor esteja dentro dos limites
 *                False caso contrario
 */
extern bool  Timeout_value_CHK ( BYTE value );
/*
 * Verifica se um valor esta dentro dos limites
 * de SCORE Delta
 *  @param BYTE value Valor a ser verificado
 *  @returns bool True caso o valor esteja dentro dos limites
 *                False caso contrario
 */
extern bool  SCORE_delta_value_CHK ( BYTE value );

/*
 * Verifica se um valor esta dentro dos limites
 * de GAME SCORE
 *  @param int value Valor a ser verificado
 *  @returns bool True caso o valor esteja dentro dos limites
 *                False caso contrario
 */
extern bool  GAME_SCORE_value_CHK ( int value );
/*
 * Verifica se um valor obedece aos 
 * limites do decremento de PLAY CLOCK
 *  @param BYTE value Valor de decremento
 *  @returns bool True caso o valor obedeca aos limites
 *                False caso contrario
 */
extern bool  PCLOCK_delta_value_CHK ( BYTE value );
/*
 * Verifica se um valor obedece aos limites 
 * de PLAY CLOCK
 *  @param int value Valor do PLAY CLOCK
 *  @returns bool True caso o valor obedeca aos limites
 *                False caso contrario
 */
extern bool  PLAY_CLOCK_value_CHK ( int value );

/*
 * Verificao se um valor obedece os 
 * limites para o BALL POSITION
 *  @param BYTE value Valor do BALL POSITION
 *  @returns bool True caso o valor obedeca aos limites
 *                False caso contrario
 */
extern bool  BALL_POS_value_CHK ( BYTE value );
/*
 * Verifica se um valor obedece aos
 * limites do BALL TO GO
 *  @param BYTE value Valor do BALL TO GO
 *  @returns bool True caso o valor obedeca aos limites
 *                False caso contrario
 */
extern bool  to_GO_value_CHK ( BYTE value );
/*
 * Verifica se um valor obedece aos limites
 * para BALL DOWN
 *  @param BYTE value Valor de BALL DOWN
 *  @returns bool True caso o valor obedeca aos limites
 *                False caso contrario
 */
extern bool  DOWN_value_CHK ( BYTE value );/*
 * Verifica se valores de min e seg 
 * estao dentro dos limites para o GAME TIME
 *  @param BYTE min Valor dos minutos
 *  @param BYTE seg Valor dos segundos
 *  @returns bool True caso os valores obedecam aos limites
 *                False caso contrario
 */
extern bool Game_Time_value_CHK ( BYTE min, BYTE seg );
#endif
