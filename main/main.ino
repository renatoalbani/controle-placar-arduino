
/*
  Skecth principal
  @Criador <Incluir dados de quem escreveu para Leandro>
           Leandro Morozowski <lmorozowski@hotmail.com>,  
           Renato Albani dos Santos <renato.utfpr@gmail.com>,
           2018
  @Atualiado Renato Albani dos Santos <renato.utfpr@gmail.com> Outubro, 2018
 */

/**
 *	Bibliotecas utilizadas neste Sistema:
 **/

#include "types.h" // definicoes de tipos utilizados pelo sistema
#include "util.h" // funcoes utilitarias e definicoes de tipos
#include "main.h" // biblioteca com definicoes do sistema

#if (RADIO_MODE == TX_RADIO)
  #include "txplacar.h" //funcoes de leitura e envio de comandos pelo modulo nRF24L01
#endif
#if (RADIO_MODE == RX_RADIO) 
  #include "rxplacar.h" //funcoes de controle dos displays e recebimento de comandos
#endif
                                                            

/*
 * Interceptador de inicializacao
 * do Arduino
 */
void	setup ()
{

  #if (RADIO_MODE == TX_RADIO)
    txSetup();
  #endif

  #if (RADIO_MODE == RX_RADIO)
    rxSetup();
  #endif
  
  //TODO: Verificar necessidade
  //Aguarda 10 ms
	delay (10);
}

/*
 * Loop principal Arduino
 * de acordo com o clock do sistema
 */
void	loop ()
{

  #if (RADIO_MODE == TX_RADIO)
    txLoop();
  #endif
  
  #if (RADIO_MODE == RX_RADIO)
    rxLoop();
  #endif
  
  //TODO: Verificar necessidade
  //Aguarda 10 ms
	delay (10);
 
}
