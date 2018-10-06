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

#endif
