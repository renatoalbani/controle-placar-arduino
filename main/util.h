/*
  util.h - Funcoes utilitarias usadas no sistema
  @Criador <Incluir dados de quem escreveu para Leandro>
            Leandro Morozowski <lmorozowski@hotmail.com>, 
            Renato Albani dos Santos <renato.utfpr@gmail.com>, 
            2018
    @Atualizado Renato Albani dos Santos <renato.utfpr@gmail.com>, Outubro 2018
 */
#ifndef util_h
#define util_h
#include <Arduino.h>
#include "types.h"
/*
 * Converte um dado booleano para BYTE 
 */
extern BYTE  BOOL_extent ( bool STS );
/*
 * Converte um byte em base hexadecimal 
 * para o padrao BCD
 */
extern BYTE  HEX8_to_BCD8 ( BYTE HEX_8 );
/*
 * Converte um byte no padrao BCD
 * para a base hexadecimal
 */
extern BYTE  BCD8_to_HEX8 ( BYTE BCD_8 );
/*
 * Converte uma palavra no padrao BCD
 * para a base hexadecimal
 */
extern WORD  BCD16_to_HEX16 ( WORD BCD_16 );
/*
 * Converte um caracter ASCII maiusculo
 * para minusculo
 */
extern BYTE  LOW_CASE_conv ( BYTE ASCII_char );
/*
 * Converte um byte ASCII para o padrao BCD
 *  @returns bool True caso seja realizada a conversao
 *                False caso contrario
 */
extern bool  ASCII_to_BCD_8 ( BYTE *code );
#endif
