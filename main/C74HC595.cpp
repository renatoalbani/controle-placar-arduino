/*
   C74HC595.cpp - Implementacao de Classe
                  para controle de registradores
                  de 74HC595 via serial
   @Criado Renato Albani dos Santos <renato.utfpr@gmail.com>, Outubro 2018
 */
#include <Arduino.h>
#include "types.h"
#include "C74HC595.h"
C74HC595::C74HC595(BYTE SCLK_PIN, BYTE SOUT_PIN, BYTE LCLK_PIN){
 
  _SCLK_PIN = SCLK_PIN;
  _SOUT_PIN = SOUT_PIN;
  _LCLK_PIN = LCLK_PIN;  
  
  digitalWrite ( _SOUT_PIN, LOW );
  pinMode ( _SOUT_PIN, OUTPUT );
  
  digitalWrite ( _SCLK_PIN, LOW );
  pinMode ( _SCLK_PIN, OUTPUT );
    
  digitalWrite ( _LCLK_PIN, HIGH );
  pinMode ( _LCLK_PIN, OUTPUT );
  
}
/*
 * Funcao para serializacao de um BYTE 
 * para os registradores 74HC595
 *  @param BYTE DATA_8 Dado a ser serializado
 */
void C74HC595::writeData(BYTE DATA_8){

  shiftOut(_SOUT_PIN, _SCLK_PIN, MSBFIRST, DATA_8);
 
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
