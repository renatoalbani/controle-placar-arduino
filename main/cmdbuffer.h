/*
   cmdbuffer.h Biblioteca com definicoes relacionadas a
               enfileiramento de comandos
    @Criado Renato Albani dos Santos <renato.utfpr@gmail.com>, Outubro 2018
 */
#ifndef cmdbuffer_h
#define cmdbuffer_h

#include <Arduino.h>
#include "types.h"

/*
 * Dimensionamento do buffer de comandos
 */
#define CMD_MAX_QTY 10 //numero maximo de comandos no buffer
#define CMD_MAX_SIZE 10 //tamanho maximo do comando
#define BUFF_SIZE (CMD_MAX_QTY * CMD_MAX_SIZE) //tamanho do buffer em bytes

/*
 * Le as distintas interfaces de recebimento
 * de comandos e adiciona os comandos a fila
 */
extern void readCommandInterfaces();
/*
 * Verifica se existe um comando enfileirado
 *  
 *  @returns bool Flag que indica a existencia
 *                de um comando para interpretacao
 *                do sistema
 */
extern bool existsCommand();
/*
 *  Copia o comando do buffer para um buffer de leitura
 *    
 *    @param BYTE_PTR writeBuffer Referencia para um buffer de leitura
 *    @param uint bufferSize Tamanho do buffer de leitura
 */
extern void readCommandBuffer(BYTE_PTR writeBuffer, uint bufferSize);
/*
 * Descarta um comando enfileirado
 */
extern void flipCommand();
/*
 * Escreve na serial os dado buffer principal.
 * Mostra tambem a posicao de leitura
 * e posicao de escrita
 */
extern void printBufferData();

#endif
