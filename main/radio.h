/*
   radio.h Biblioteca para envio e recebimento
           de comandos via radio
   @Criado Renato Albani dos Santos <renato.utfpr@gmail.com>, Outubro 2018
 */
#ifndef radio_h
#define radio_h
#include <Arduino.h>
#include "types.h"
/*
 * Inicializacao da controladora
 * de radio
 */
extern void initRadio();
/*
 * Envia um pacote via radio 
 * @param BYTE_PTR buf Referencia para o buffer de dados
 * @param uint len Tamanho do buffer de dados
 * @returns bool Flag que indica se o envio foi bem sucedido
 */
extern bool writeRadioPkg(void *buf, uint len);
/*
 * Leitura de pacote recebido via radio
 * @param BYTE_PTR readBuf Referencia para o buffer de leitura dos dados
 * @param uint bufSize Tamanho do buffer de leitura
 * @returns uint Numero de bytes recebidos
 */
extern uint readRadioPkg(BYTE_PTR readBuf, uint bufSize);
#endif
