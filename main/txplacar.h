/*
  txplacar.h - Biblioteca de funcoes e definicoes
               relacionados a transmissao de comandos
               para o controle do placar utilizando
               o modulo nRF24L01
    @Criado Renato Albani dos Santos <renato.utfpr@gmail.com>, Outubro 2018
 */
#ifndef TXPLACAR_H
#define TXPLACAR_H

#include <Arduino.h>
#include "types.h"

/*
 * Definicao do tamanho
 * do buffer de leitura
 * de comandos
 */
#define CMD_size 10 
/*
 * Inicializacao das configuracoes
 * do radio para transmissao
 */
extern void txSetup();
/*
 * Controle de estado de transmissao 
 * no sistema. Chamado no loop principal
 */
extern void txLoop();

#endif
