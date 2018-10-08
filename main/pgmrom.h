/*
  pgmrom.h - Biblioteca para definicoes de tipos e
             funcoes utilitarias de acesso a memoria de programa
             do sistema
  @Criador  <Incluir dados de quem escreveu para Leandro>
            Leandro Morozowski <lmorozowski@hotmail.com>, 
            Renato Albani dos Santos <renato.utfpr@gmail.com>, 
            2018
  @Atualizado Renato Albani dos Santos <renato.utfpr@gmail.com>, Outubro 2018
 */
#ifndef pgmrom_h
#define pgmrom_h

#include <stdio.h>  // "stdio" nativo do C++, para Arduino (CPU AVR).
#include <stdlib.h> // "stdlib" nativo do C++, para Arduino (CPU AVR).
#include <avr/pgmspace.h> // para acesso à FLASH da CPU.
#include "main.h"

/**
 * Definicao de tipos e funcoes para acesso à Memória de Programa:
 **/
#define  ROM_set const PROGMEM       // definicao de dados na Flash da CPU.

#define ROM_BYTE_ptr  const char *   // definicao de um Ponteiro para acesso
                                     // à Bytes armazenados na Flash.

#define ROM_get8(var) pgm_read_byte_near(var) // funcao para leitura de um
                                              // Byte armazenado na Flash.

#define ROM_get16(var)  pgm_read_word_near(var) // funcao para leitura de uma
                                                // Word armazenada na Flash.

#define ROM_PTR_get(var) pgm_read_ptr_near(var) // funcao para leitura de um
                                                // Ponteiro armazenado na Flash.
/**
 * Definicao de funcoes para especificar armazenamento de strings
 * na memória de Programa, em metodos de "printagem" serial:
 **/
#define ROM_SERIAL_print(s) Serial.print(F(s))
#define ROM_SERIAL_LF_print(s)  Serial.println(F(s))

/**
 * Tabela para traducao de digitos BCD para os respectivos padroes
 * numericos codificados em 7 segmentos, para o Placar "Main"
 **/
ROM_set  BYTE Seven_SEG_code_M [] =
{        
  Seven_SEG_0_patt_M, // 0: padrao do "número 0", em 7 segmentos.
  Seven_SEG_1_patt_M, // 1: padrao do "número 1", em 7 segmentos.
  Seven_SEG_2_patt_M, // 2: padrao do "número 2", em 7 segmentos.
  Seven_SEG_3_patt_M, // 3: padrao do "número 3", em 7 segmentos.
  Seven_SEG_4_patt_M, // 4: padrao do "número 4", em 7 segmentos.
  Seven_SEG_5_patt_M, // 5: padrao do "número 5", em 7 segmentos.
  Seven_SEG_6_patt_M, // 6: padrao do "número 6", em 7 segmentos.
  Seven_SEG_7_patt_M, // 7: padrao do "número 7", em 7 segmentos.
  Seven_SEG_8_patt_M, // 8: padrao do "número 8", em 7 segmentos.
  Seven_SEG_9_patt_M  // 9: padrao do "número 9", em 7 segmentos.
};

/**
 * Tabela para traducao de digitos BCD para os respectivos padroes
 * numericos codificados em 7 segmentos, para o Placar "Piloto":
 **/
ROM_set  BYTE Seven_SEG_code_P [] =
{       
  Seven_SEG_0_patt_P, // 0: padrao do "número 0", em 7 segmentos.
  Seven_SEG_1_patt_P, // 1: padrao do "número 1", em 7 segmentos.
  Seven_SEG_2_patt_P, // 2: padrao do "número 2", em 7 segmentos.
  Seven_SEG_3_patt_P, // 3: padrao do "número 3", em 7 segmentos.
  Seven_SEG_4_patt_P, // 4: padrao do "número 4", em 7 segmentos.
  Seven_SEG_5_patt_P, // 5: padrao do "número 5", em 7 segmentos.
  Seven_SEG_6_patt_P, // 6: padrao do "número 6", em 7 segmentos.
  Seven_SEG_7_patt_P, // 7: padrao do "número 7", em 7 segmentos.
  Seven_SEG_8_patt_P, // 8: padrao do "número 8", em 7 segmentos.
  Seven_SEG_9_patt_P  // 9: padrao do "número 9", em 7 segmentos.
};

/**
 * Tabela de codificacao dos segmentos de timeout,
 * para o Placar "Main"
 **/
ROM_set  BYTE TMO_code_TAB_M [] =
{            
  HOME_TMO_0_patt_M, 
  GUEST_TMO_0_patt_M, 
  
  HOME_TMO_1_patt_M, 
  GUEST_TMO_1_patt_M, 
  
  HOME_TMO_2_patt_M, 
  GUEST_TMO_2_patt_M,
  
  HOME_TMO_3_patt_M, 
  GUEST_TMO_3_patt_M, 

  HOME_TMO_mask_M, 
  GUEST_TMO_mask_M
};            

/**
 * Tabela de codificacao dos segmentos de timeout
 * para o placar "Piloto"
 **/
ROM_set  BYTE TMO_code_TAB_P [] =
{           
  HOME_TMO_0_patt_P, 
  GUEST_TMO_0_patt_P,
  
  HOME_TMO_1_patt_P, 
  GUEST_TMO_1_patt_P,
 
  HOME_TMO_2_patt_P, 
  GUEST_TMO_2_patt_P,
 
  HOME_TMO_3_patt_P, 
  GUEST_TMO_3_patt_P,

  HOME_TMO_mask_P, 
  GUEST_TMO_mask_P
};

/**
 * Tabela de codificacao dos segmentos de "vantagem"
 * para o placar "Main"
 */
ROM_set  BYTE VANT_code_TAB_M [] =
{        
  HOME_VANT_patt_M, // 0: padrao para "Home Vantage".
  GUEST_VANT_patt_M,  // 1: padrao para "Guest Vantage".
  VANT_PATT_mask_M  // 2: mascara para os bits "Vantage".
};

/**
 * Tabela de codificacao dos segmentos de "vantagem"
 * para o placar "Piloto"
 */
ROM_set  BYTE VANT_code_TAB_P [] =
{       
  HOME_VANT_patt_P, // 0: padrao para "Home Vantage".
  GUEST_VANT_patt_P,  // 1: padrao para "Guest Vantage".
  VANT_PATT_mask_P  // 2: mascara para os bits "Vantage".
};

#endif
