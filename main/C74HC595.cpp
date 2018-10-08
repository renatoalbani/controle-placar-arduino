/*
   C74HC595.cpp - Implementacao de Classe
                  para controle de registradores
                  de 74HC595 via serial
   @Criado Renato Albani dos Santos <renato.utfpr@gmail.com>, Outubro 2018
 */
#include <Arduino.h>
#include "types.h"
#include "C74HC595.h"
C74HC595::C74HC595(int SCLK_PIN, int SOUT_PIN, int LCLK_PIN){
  _SCLK_PIN = SCLK_PIN;
  _SOUT_PIN = SOUT_PIN;
  _LCLK_PIN = LCLK_PIN;  
  
  pinMode ( _SOUT_PIN, OUTPUT );
  digitalWrite ( _SOUT_PIN, LOW );

  pinMode ( _SCLK_PIN, OUTPUT );
  digitalWrite ( _SCLK_PIN, LOW );
  
  pinMode ( _LCLK_PIN, OUTPUT );
  digitalWrite ( _LCLK_PIN, HIGH );
  
}
/*
 * Funcao para serializacao de um BYTE 
 * para os registradores 74HC595
 *  @param BYTE DATA_8 Dado a ser serializado
 */
void C74HC595::writeData(BYTE DATA_8){
  BYTE  bit_n;    // bit atual sendo enviado ao ShiftRegister.
  BYTE  bit_MASK; // mascara binaria para o bit atual.

  bit_MASK = BIT_7;

  for ( bit_n = 0; bit_n <=7; bit_n++ )
  {
    if ( DATA_8 & bit_MASK )
    {
      digitalWrite (_SOUT_PIN, HIGH );

    }
    else
    {
      digitalWrite (_SOUT_PIN, LOW );
    }

    digitalWrite (_SCLK_PIN, LOW );

    delayMicroseconds(1);

    digitalWrite (_SCLK_PIN, HIGH );

    delayMicroseconds(1);

    bit_MASK = ( bit_MASK >> 1);
  }

  delayMicroseconds(1);
  digitalWrite ( _SOUT_PIN, HIGH );
  digitalWrite ( _SCLK_PIN, LOW );
}
/**
 * Funcao para transicao de clock do LATCH
 * de C74HC595 e memorizacao do estado 
 * atual dos registradores de deslocamento
 */
void C74HC595::latch(){
  digitalWrite ( _LCLK_PIN, HIGH );

  delayMicroseconds(1);

  digitalWrite ( _LCLK_PIN, LOW );
}
