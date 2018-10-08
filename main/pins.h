/*
  pins.h - Definicao de pinos utilizados no sistema
    @Criador <Incluir dados de quem escreveu para Leandro>
            Leandro Morozowski <lmorozowski@hotmail.com>, 
            Renato Albani dos Santos <renato.utfpr@gmail.com>, 
            2018
    @Atualizado Renato Albani dos Santos <renato.utfpr@gmail.com>, Outubro 2018
 */
#ifndef pins_h
#define pins_h
#include "main.h"
/**
 * Definicao da pinagem relativa aos sinais de Controle de
 * Hardware, quando a placa usada e o Arduino Mega 2560
 * (MODO NaO SIMULADO (REAL))
 **/
#if  ( ( HW_Board == MEGA_2560 ) && ( SIMUL == 0 ) ) // opcao para Arduino

  #define SDOUT_piloto  2 // pino da placa para o sinal "SDOUT" do "Piloto".
  #define SCLK_piloto 6 // pino da placa para o sinal "SCLK" do "Piloto".
  #define PCLK_piloto 4 // pino da placa para o sinal "PCLK" do "Piloto".
    
  #define SDOUT_main  8 // pino da placa para o sinal "SDOUT" do "Main".
  #define SCLK_main 12  // pino da placa para o sinal "SCLK" do "Main".
  #define PCLK_main 10  // pino da placa para o sinal "PCLK" do "Main".

#endif

/**
 * Definicao da pinagem relativa aos sinais de Controle de
 * Hardware, quando a placa usada e o Arduino Mega 2560
 * (MODO SIMULADO)
 **/
#if ( ( HW_Board == MEGA_2560 ) && ( SIMUL == 1 ) )

  #define SDOUT_piloto  5 // pino da placa para o sinal "SDOUT" do "Piloto".
  #define SCLK_piloto 6 // pino da placa para o sinal "SCLK" do "Piloto".
  #define PCLK_piloto 50  // pino da placa para o sinal "PCLK" do "Piloto".

  #define SDOUT_main  5 // pino da placa para o sinal "SDOUT" do "Main".
  #define SCLK_main 6 // pino da placa para o sinal "SCLK" do "Main".
  #define PCLK_main 7 // pino da placa para o sinal "PCLK" do "Main".

#endif

/**
 * Definicao da pinagem relativa aos sinais de Controle de
 * Hardware, quando a placa usada e o Arduino UNO rev 3:
 **/
#if ( HW_Board == UNO_rev3 )

  #define SDOUT_piloto  5 // pino da placa para o sinal "SDOUT" do "Piloto".
  #define SCLK_piloto 6 // pino da placa para o sinal "SCLK" do "Piloto".
  #define PCLK_piloto 4 // pino da placa para o sinal "PCLK" do "Piloto".

  #define SDOUT_main  5 // pino da placa para o sinal "SDOUT" do "Main".
  #define SCLK_main 6 // pino da placa para o sinal "SCLK" do "Main".
  #define PCLK_main 7 // pino da placa para o sinal "PCLK" do "Main".

#endif
#endif