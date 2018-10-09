/*
  limits.cpp - Definicoes de limites do sistema
               e funcoes de verificacao
  @Criado Renato Albani dos Santos <renato.utfpr@gmail.com> Outubro 2018           
 */
#include "limits.h"
#include <Arduino.h>
#include "types.h"
/*
 * Verifica se uma valor esta dentro
 * dos limites de QUARTER
 *  @param BYTE value Valor a ser testado
 *  @returns bool True caso esteja dentro dos limites
 *                False caso contrario
 */
bool  QUARTER_value_CHK ( BYTE value )
{
  return ( value >= QUARTER_MIN ) && ( value <= QUARTER_MAX );
}
/*
 * Verifica se um valor esta dentro
 * dos limites de TIMEOUT
 *  @param BYTE value Valor a ser verificado
 *  @returns bool True caso o valor esteja dentro dos limites
 *                False caso contrario
 */
bool  Timeout_value_CHK ( BYTE value )
{
  return ( value >= Timeout_MIN ) && ( value <= Timeout_MAX );
}
/*
 * Verifica se um valor esta dentro dos limites
 * de SCORE Delta
 *  @param BYTE value Valor a ser verificado
 *  @returns bool True caso o valor esteja dentro dos limites
 *                False caso contrario
 */
bool  SCORE_delta_value_CHK ( BYTE value )
{
  return ( value >= SCORE_delta_MIN ) && ( value <= SCORE_delta_MAX );
}
/*
 * Verifica se um valor esta dentro dos limites
 * de GAME SCORE
 *  @param int value Valor a ser verificado
 *  @returns bool True caso o valor esteja dentro dos limites
 *                False caso contrario
 */
bool  GAME_SCORE_value_CHK ( int value )
{
  return ( value >= GAME_SCORE_MIN ) && ( value <= GAME_SCORE_MAX );
}
/*
 * Verifica se um valor obedece aos 
 * limites do decremento de PLAY CLOCK
 *  @param BYTE value Valor de decremento
 *  @returns bool True caso o valor obedeca aos limites
 *                False caso contrario
 */
bool  PCLOCK_delta_value_CHK ( BYTE value )
{
  return ( value >= PCLOCK_delta_MIN ) && ( value <= PCLOCK_delta_MAX );
}

/*
 * Verifica se um valor obedece aos limites 
 * de PLAY CLOCK
 *  @param int value Valor do PLAY CLOCK
 *  @returns bool True caso o valor obedeca aos limites
 *                False caso contrario
 */
bool  PLAY_CLOCK_value_CHK ( int value )
{
  return ( value >= PLAY_CLOCK_MIN ) && ( value <= PLAY_CLOCK_MAX );
}

/*
 * Verificao se um valor obedece os 
 * limites para o BALL POSITION
 *  @param BYTE value Valor do BALL POSITION
 *  @returns bool True caso o valor obedeca aos limites
 *                False caso contrario
 */
bool  BALL_POS_value_CHK ( BYTE value )
{
  return ( value >= BALL_pos_MIN ) && ( value <= BALL_pos_MAX );
}

/*
 * Verifica se um valor obedece aos
 * limites do BALL TO GO
 *  @param BYTE value Valor do BALL TO GO
 *  @returns bool True caso o valor obedeca aos limites
 *                False caso contrario
 */
bool  to_GO_value_CHK ( BYTE value )
{
  return ( value >= to_GO_MIN ) && ( value <= to_GO_MAX );
}
/*
 * Verifica se um valor obedece aos limites
 * para BALL DOWN
 *  @param BYTE value Valor de BALL DOWN
 *  @returns bool True caso o valor obedeca aos limites
 *                False caso contrario
 */
bool  DOWN_value_CHK ( BYTE value )
{
 return ( value >= down_MIN ) && ( value <= down_MAX );
}
/*
 * Verifica se valores de min e seg 
 * estao dentro dos limites para o GAME TIME
 *  @param BYTE min Valor dos minutos
 *  @param BYTE seg Valor dos segundos
 *  @returns bool True caso os valores obedecam aos limites
 *                False caso contrario
 */
bool  Game_Time_value_CHK ( BYTE min, BYTE seg )
{
  WORD  seg_TOTAL = ( 60*min + seg );
  return ( seg_TOTAL <= Game_Time_MAX_seg );
}
