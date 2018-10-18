#include <Arduino.h>
#include "types.h"
#include "main.h"
#include "pins.h"
#include "cmdbuffer.h"

#include "radio.h"


/*
 * Buffers 
 * SYS_CMD_BUFFER Buffer principal
 * SERIAL_CMD_BUFFER Buffer auxiliar para interpretacao
 *                   de comandos recebidos pela serial
 */
BYTE SYS_CMD_BUFFER [BUFF_SIZE];
BYTE SERIAL_CMD_BUFFER[CMD_MAX_SIZE];

/*
 * Referencia de escrita para o buffer serial
 */
BYTE_PTR serialWriteRef = SERIAL_CMD_BUFFER;

/**
 * Referencias de leitura e escrita do 
 * buffer principal
 */
BYTE_PTR writeRef = SYS_CMD_BUFFER;
BYTE_PTR readRef = SYS_CMD_BUFFER;

/*
 * Verifica se existe algum dado
 * enfileirado no buffer principal
 * @returns bool Flag indicando a existencia
 *               de um dado enfileirado
 */
bool hasData(){
  return readRef != writeRef; 
}

/**
 * Verifica se a posicao de uma referencia e
 * equivalente a ultima posicao do buffer de
 * commandos
 * @param BYTE_PTR ref Referencia a ser verificada
 * @returns bool Flag que indica se a referencia
 *               aponta para a ultima posicao do
 *               buffer
 */
bool endIsReached(BYTE_PTR ref){
  return ref >= (SYS_CMD_BUFFER + BUFF_SIZE);
}

/*
 * Verifica se o comprimento de escrita desejado
 * nao ultrapassa os limites de buffer de commandos
 * @param uint writeSize Comprimento de escrita
 * @returns bool Flag que indica se o comprimento esta
 *               fora dos limites
 */
bool isWriteOutOfBound(uint writeSize){
  if(writeRef >= readRef){
    return writeSize >= ( (readRef - SYS_CMD_BUFFER) + (((SYS_CMD_BUFFER + BUFF_SIZE) - readRef ) - (writeRef - readRef)) );
  }
  return (readRef - writeRef) <= writeSize;
}

/*
 * Restaura a referencia de leitura
 * do buffer principal para a posicao
 * inicial
 */
void readReset(){
  readRef = SYS_CMD_BUFFER;
}
/*
 * Restaura a referencia de escrita
 * do buffer principal para a posicao
 * inicial
 */
void writeReset(){
  writeRef = SYS_CMD_BUFFER;
}

/*
 * Escreve dados no buffer principal
 * @param BYTE_PTR data Referencia para os dados
 * @param uint writeSize Quantidade de bytes de dados 
 *                       a serem copiados
 * @returns bool Flag que indica se a escrita
 *               foi bem sucedida
 */
bool writeBuffer(BYTE_PTR data, uint writeSize){
  if(isWriteOutOfBound(writeSize)){
    return false;
  }
  
  BYTE_PTR dataAux;
  dataAux = data;
  
  while(writeSize){
    if(endIsReached(writeRef)){
      writeReset();
    }
    *writeRef = *dataAux;
    ++dataAux;
    ++writeRef;
    --writeSize;    
  }
  return true;
}
/*
 * Escreve na serial os dado buffer principal.
 * Mostra tambem a posicao de leitura
 * e posicao de escrita
 */
void printBufferData(){
  Serial.println("Buffer data: ");
  for(int i=0; i < sizeof(SYS_CMD_BUFFER); i++){
    if(readRef == &SYS_CMD_BUFFER[i]){
      Serial.print(" RD> ");
    }else if(writeRef == &SYS_CMD_BUFFER[i]){
      Serial.print(" WR> ");
    }
    Serial.print(SYS_CMD_BUFFER[i]);
  }
  Serial.println();
  Serial.println("End buffer");
}
/*
 * Le um byte da interface serial
 * escrevendo o comando no buffer principal
 * caso seja detectado que um comando
 * completo foi lido
 */
void readSerialInterface(){
  if(0 >= Serial.available()){
    return;
  }
  if(serialWriteRef >= (SERIAL_CMD_BUFFER + CMD_MAX_SIZE)){
    serialWriteRef = SYS_CMD_BUFFER;
  }
  *serialWriteRef = Serial.read();
  ++serialWriteRef;
  if(CMD_end != *(serialWriteRef - 1)){    
    return;
  }
  Serial.println("Write serial read command");
  writeBuffer(SERIAL_CMD_BUFFER, (serialWriteRef - SERIAL_CMD_BUFFER));
  serialWriteRef = SERIAL_CMD_BUFFER;
}
/*
 * Le comandos da interface de radio
 * do sistema
 */
void readRadioInterface(){
  BYTE RBUFF[CMD_MAX_SIZE];
  uint readLen = readRadioPkg(RBUFF, sizeof(RBUFF));
  if(readLen <= 0){
    return;
  }
  writeBuffer(RBUFF, sizeof(RBUFF));
}
/*
 * Le as distintas interfaces de recebimento
 * de comandos e adiciona os comandos a fila
 */
void readCommandInterfaces(){
  readSerialInterface();
  #if (RX_RADIO_ENABLED == RX_RADIO_MODE)
  readRadioInterface();
  #endif
}
/*
 * Verifica se existe um comando enfileirado
 *  
 *  @returns bool Flag que indica a existencia
 *                de um comando para interpretacao
 *                do sistema
 */
bool existsCommand(){
  BYTE_PTR startRef;
  startRef = readRef;
  while(startRef != writeRef){
    if(CMD_end == *startRef){
      return true;
    }
    ++startRef;
  }
  
  return false;
}
/*
 *  Copia o comando do buffer para um buffer de leitura
 *    
 *    @param BYTE_PTR writeBuffer Referencia para um buffer de leitura
 *    @param uint bufferSize Tamanho do buffer de leitura
 */
void readCommandBuffer(BYTE_PTR writeBuffer, uint bufferSize){
  uint count = 0;
  if(!hasData()){
    return;
  }
  while(bufferSize){
    if(endIsReached(readRef)){
      readReset();
    }
    *(writeBuffer + count) = *readRef;
    ++count;
    ++readRef;
    if(CMD_end == *(readRef - 1)){
      break;  
    }
    --bufferSize;
  }
}
/*
 * Descarta um comando enfileirado
 */
void flipCommand(){
  while(CMD_end != *readRef){
    ++readRef;
  }
  ++readRef;
}

/*
 * Inicializacao das distintas 
 * interfaces de recebimento de
 * comandos
 */
void initCommandInterfaces(){
  //TODO: Verificar a posibilidade de inicializar
  //      a interface serial atraves desta
  //      chamada
  #if (RX_RADIO_ENABLED == RX_RADIO_MODE) 
  initRadio();
  #endif
}
