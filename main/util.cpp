/*
  Implementacao de funcoes utilitarias usadas no sistema
    @Criador <Incluir dados de quem escreveu para Leandro>
              Leandro Morozowski <lmorozowski@hotmail.com>, 
              Renato Albani dos Santos <renato.utfpr@gmail.com>, 
              2018
    @Atualizado Renato Albani dos Santos <renato.utfpr@gmail.com>, Outubro 2018
 */
#include <Arduino.h>
#include "types.h"
#include "util.h"
/*
 * Converte um dado booleano para BYTE 
 */
BYTE  BOOL_extent ( bool STS )
{
  return ( STS? 0xFF : 0 );
}
/*
 * Converte um byte em base hexadecimal 
 * para o padrao BCD
 */
BYTE  HEX8_to_BCD8 ( BYTE HEX_8 )
{

  BYTE  BCD_result;
  BYTE  i;

  BCD_result = 0;

  if ( HEX_8 < 100 )
  {
    i = 0;

    while ( HEX_8 >= 10 )
    {
      BCD_result += ( ( HEX_8 % 10 ) << i );

      HEX_8 /= 10;

      i += 4;
    }

    BCD_result += ( HEX_8 << i );
  }

  return ( BCD_result );
}
/*
 * Converte um byte no padrao BCD
 * para a base hexadecimal
 */
BYTE  BCD8_to_HEX8 ( BYTE BCD_8 )
{
  return ( 10*( ( BCD_8 & 0xF0 ) >> 4 ) + ( BCD_8 & 0x0F ) );
}
/*
 * Converte uma palavra no padrao BCD
 * para a base hexadecimal
 */
WORD  BCD16_to_HEX16 ( WORD BCD_16 )
{
  WORD  HEX_16;

  HEX_16 = ( BCD_16 & 0x000F );
  HEX_16 += 10*( ( BCD_16 & 0x00F0 ) >> 4 );
  HEX_16 += 100*( ( BCD_16 & 0x0F00 ) >> 8 );
  HEX_16 += 1000*( ( BCD_16 & 0xF000 ) >> 12 );

  return ( HEX_16 );
}
/*
 * Converte um caracter ASCII maiusculo
 * para minusculo
 */
BYTE  LOW_CASE_conv ( BYTE ASCII_char )
{
  if ( ( ASCII_char >= 'A' ) && ( ASCII_char <= 'Z' ) )
  {
    ASCII_char += 0x20;
  }

  return (ASCII_char);
}
/*
 * Converte um byte ASCII para o padrao BCD
 *  @returns bool True caso seja realizada a conversao
 *                False caso contrario
 */
bool  ASCII_to_BCD_8 ( BYTE *code )
{
  bool is_BCD = false;

  if ( ( *code >= 0x30 ) && ( *code <= 0x39 ) )
  {
    *code = *code - 0x30;

    is_BCD = true;
  }

  return (is_BCD);
}

/*
 * Le o proximo byte BCD nos dados de um comando
 *  @param BYTE_PTR CMD_data referencia para os dados do comando
 *  @param BYTE *index referencia para indice de inicio da verificacao 
 *  @param BYTE *BCD_8 referencia (buffer) para armazenar o byte encontrado 
 *  @returns bool True caso exista um byte BCD nos dados
 *                False caso contrario
 */
bool  BCD_8_NEXT_get ( BYTE_PTR CMD_data, BYTE *index, BYTE *BCD_8 )
{
  BYTE VAL_1, VAL_2;
  bool is_BCD = false;

  VAL_1 = CMD_data [*index];

  if ( ASCII_to_BCD_8 (&VAL_1) )
  {
    is_BCD = true;

    (*index)++;

    VAL_2 = CMD_data [*index];

    if ( ASCII_to_BCD_8 (&VAL_2) )
    {
      (*index)++;

      VAL_1 = ( VAL_1 << 4 ) + VAL_2;
    }

    *BCD_8 = VAL_1;
  }

  return (is_BCD);
}
