/*
    txplaclar.cpp - Implementacao de funcoes
                    para o controle de envio
                    de comandos via radio
    @Criado Renato Albani dos Santos <renato.utfpr@gmail.com>, Outubro 2018
 */
#include <Arduino.h>

/*
 * Bibliotecas para controle  
 * do RF transceiver nRF24L01
 */
#include <RF24_config.h> 
#include <RF24.h> 
#include <printf.h>
#include <nRF24L01.h>

#include "main.h"
#include "types.h"
#include "pgmrom.h"
#include "txplacar.h"
/*
 * Instancia o driver de radio
 */
RF24 _radio(RF24_CE_PIN, RF24_CSN_PIN);
BYTE _PIPEADDR[] = { "ADDR1" }; //endereco logico para transmissao de mensagens
char _commandBuffer[CMD_size]; //buffer de commandos lidos pela serial
/*
 * Limpa o buffer a direita a partir de 
 * uma posicao
 *  @param size_t readSize Posicao de inicio
 */
void bufferClearRight(size_t readSize){
  if(!readSize || readSize == CMD_size){
    return;
 }
 for(char i = readSize; i < CMD_size; i++){
    _commandBuffer[i] = 0x00;
  }
}
/*
 * Escreve o comando memorizado
 * na serial
 */
void printCommand(){
  ROM_SERIAL_print("Command lido: ");
  Serial.println(_commandBuffer);
}
/*
 * Envia o comando via radio
 */
bool txSendCommand() {
  return _radio.write(_commandBuffer, sizeof(_commandBuffer));
}

/*
 * Le os comandos via serial
 */
bool txReadCommand() {
 if(Serial.available() <= 0){
   return false;
 }
 size_t readSize = Serial.readBytesUntil(CMD_end, _commandBuffer, CMD_size);
 if(0 == readSize){
    return false;    
 }
 bufferClearRight(readSize);
 printCommand();
 return true;   
}

/*
 * Controle de estado de transmissao 
 * no sistema. Chamado no loop principal
 */
void txLoop(){
  if(!txReadCommand()){
    return;
  }
  if(!txSendCommand()){
    ROM_SERIAL_LF_print("ERRO - Falha ao enviar comando via radio!!!");
  }
}
/*
 * Inicializacao das configuracoes
 * do radio para transmissao
 */
void txSetup(){
  Serial.begin (9600);
  _radio.begin();
  _radio.openWritingPipe(_PIPEADDR);
  ROM_SERIAL_LF_print("Inicializando transmissor de comandos...");
  Serial.println (); 
}
