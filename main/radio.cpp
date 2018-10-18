#include <Arduino.h>

/*
 * Bibliotecas para controle  
 * do RF transceiver nRF24L01
 */
#include <RF24_config.h> 
#include <RF24.h> 
#include <nRF24L01.h>

#include "main.h"
#include "pins.h"
#include "types.h"

/*
 * Instancia o driver de radio
 */
RF24 _radio(RF24_CE_PIN, RF24_CSN_PIN);
BYTE ADDR[] = {PIPEADDR};

void initRadio(){
  _radio.begin();
  _radio.enableDynamicPayloads();
  #if (RADIO_MODE == TX_RADIO)
  _radio.openWritingPipe(ADDR);
  #endif
  #if (RADIO_MODE == RX_RADIO)
  _radio.openReadingPipe(RF24_PIPE1, ADDR);
  _radio.startListening();
  #endif
}

bool writeRadioPkg(void *buf, uint len){
  return _radio.write(buf, len);  
}

uint readRadioPkg(BYTE_PTR readBuf, uint bufSize){
  if(!_radio.available()){
     return 0;
  }
  uint dynamicPayloadSize = _radio.getDynamicPayloadSize();
  _radio.read(readBuf, bufSize);
  return dynamicPayloadSize;
}
