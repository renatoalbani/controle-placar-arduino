/*
  C74HC595.h - Classe para controle
               de registradores 74HC595
               interfaceado via serial
  @Criado Renato Albani dos Santos <renato.utfpr@gmail.com> Outubro, 2018
  @Atualizado Renato Albani dos Santos <renato.utfpr@gmail.com> Outubro, 2018
 */
#ifndef C74HC595_h
#define C74HC595_h
#include <Arduino.h>
#include "types.h"
class C74HC595 {
  public:
    /*
     * Construtor para instancias de C74HC595
     *  @param int SCLK_PIN Pino de clock da serial
     *  @param int SOUT_PIN Pino de dados da serial
     *  @param int LCLK_PIN Pino de clock do latch
     */
    C74HC595(BYTE SCLK_PIN, BYTE SOUT_PIN, BYTE LCLK_PIN);
    /*
     * Funcao para serializacao de um BYTE 
     * para os registradores 74HC595
     *  @param BYTE DATA_8 Dado a ser serializado
     */
    void writeData(BYTE DATA_8);
    /**
     * Funcao para transicao de clock do LATCH
     * de C74HC595 e memorizacao do estado 
     * atual dos registradores de deslocamento
     */
    void latch();
  private:
    BYTE _SCLK_PIN; //pino de clock da serial
    BYTE _SOUT_PIN; //pino de dados da serial
    BYTE _LCLK_PIN; //pino de clock do latch
};
#endif
