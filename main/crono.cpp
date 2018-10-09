#include <Arduino.h>
#include "crono.h"
#include "types.h"
/*
 * NOOP
 */
void  CRONO_DUMMY_notify (){}

/*
 * Inicializa um cronometro
 *  @param CRONO_type *CRONO_PTR Referencia para a estrutura inicializada
 *  @param uint periodo_ms Cadencia do cronometro
 */
void  CRONO_init ( struct CRONO_type *CRONO_PTR, uint periodo_ms )
{
  CRONO_PTR->periodo_ms = periodo_ms; // seta o periodo de disparo do cronometro.

  CRONO_PTR->trigger = false;   // reseta indicador de evento.

  CRONO_PTR->run = false;   // mantem cronometro parado.

  CRONO_PTR->registro_ms = millis();  // reseta referencia para
            // atualizacao do cronometro.

  CRONO_PTR->CRONO_notify = CRONO_DUMMY_notify; // "desliga" informe da cadencia.
}

/*
 * Atribui uma funcao para notificacao de cadencia
 * do cronometro
 *  @param CRONO_type *CRONO_PRT Referencia do cronometro
 *  @param (*CRONO_notify)() Funcao a ser invocada para notificacao 
 */
void  CRONO_notify_ON ( struct CRONO_type *CRONO_PTR, void (*CRONO_notify) () )
{
  CRONO_PTR->CRONO_notify = CRONO_notify; // seta funcao para
              // informe da cadencia.
}

/*
 * Atribui uma funcao NOOP para simular o desligamento
 * de notificacoes de cadencia do cronometro
 *  @param CRONO_type *CRONO_PTR Referencia para o cronometro
 */
void  CRONO_notify_OFF ( struct CRONO_type *CRONO_PTR )
{
  CRONO_PTR->CRONO_notify = CRONO_DUMMY_notify; // "desliga" informe
                                                  // da cadencia.
}

/*
 * Coloca o cronometro no estado de pausa
 *  @param CRONO_type *CRONO_PTR Referencia para o cronometro
 */
void  CRONO_stop ( struct CRONO_type *CRONO_PTR )
{
  CRONO_PTR->run = false; // mantem cronometro parado.

  CRONO_PTR->trigger = false; // reseta indicador de evento.
}

/*
 * Coloca o cronometro no estado de execucao
 *  @param CRONO_type *CRONO_PTR Referencia para o cronometro
 */
void  CRONO_run ( struct CRONO_type *CRONO_PTR )
{
  CRONO_PTR->run = true;  // libera o cronometro.

  CRONO_PTR->trigger = false; // reseta indicador de evento.

  CRONO_PTR->registro_ms = millis();  // reseta referencia para
                                        // atualizacao do cronometro.
}

/*
 * Verifica o estado da execucao do cronometro
 *  @param CRONO_type *CRONO_PTR Referencia para o cronometro
 *  @returns True caso esteja em execucao False caso contrario
 */
bool  CRONO_running_CHECK ( struct CRONO_type *CRONO_PTR )
{
  return CRONO_PTR->run;
}

/*
 * Notifica uma cadencia do cronometro
 *  @param CRONO_type *CRONO_PTR Referencia para o cronometro
 */
void  CRONO_detected ( struct CRONO_type *CRONO_PTR )
{
  CRONO_PTR->trigger = false; // reseta indicador de evento.

  CRONO_PTR->CRONO_notify (); // informa a cadencia do cronometro.
}

/*
 * Verica a flag de controle de disparo do cronometro
 *  @param CRONO_type *CRONO_PTR Referencia para o cronometro
 */
bool  CRONO_trigger_CHECK ( struct CRONO_type *CRONO_PTR )
{
  return CRONO_PTR->trigger;
}

/*
 * Atualiza o estado de um cronometro
 * de acordo com o relogio da placa
 *  @param CRONO_type *CRONO_PTR Referencia para um cronometro
 */
void  CRONO_atualize ( struct CRONO_type *CRONO_PTR )
{
  ulong atual_Millis;

  if ( CRONO_PTR->run )
  {
    atual_Millis = millis();  // obtem valor atual do "millis".

    if ( ( atual_Millis - CRONO_PTR->registro_ms ) >= CRONO_PTR->periodo_ms )
    {
      CRONO_PTR->trigger = true;  // se ocorreu um "trigger", indica isso.

      CRONO_PTR->registro_ms = atual_Millis;  // seta nova referencia para
    }                                           // próxima atualizacao.
  }
  else
  {
    CRONO_PTR->trigger = false;   // reseta indicador de evento.

    CRONO_PTR->registro_ms = millis();  // reseta referencia para
  }                                       // atualizacao do cronometro.
}
