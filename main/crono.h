#ifndef crono_h
#define crono_h
#include "types.h"
/*
 * NOOP
 */
extern void  CRONO_DUMMY_notify ();

/*
 * Inicializa um cronometro
 *  @param CRONO_type *CRONO_PTR Referencia para a estrutura inicializada
 *  @param uint periodo_ms Cadencia do cronometro
 */
extern void  CRONO_init ( struct CRONO_type *CRONO_PTR, uint periodo_ms );

/*
 * Atribui uma funcao para notificacao de cadencia
 * do cronometro
 *  @param CRONO_type *CRONO_PRT Referencia do cronometro
 *  @param (*CRONO_notify)() Funcao a ser invocada para notificacao 
 */
extern void  CRONO_notify_ON ( struct CRONO_type *CRONO_PTR, void (*CRONO_notify) () );
/*
 * Atribui uma funcao NOOP para simular o desligamento
 * de notificacoes de cadencia do cronometro
 *  @param CRONO_type *CRONO_PTR Referencia para o cronometro
 */
extern void  CRONO_notify_OFF ( struct CRONO_type *CRONO_PTR );
/*
 * Coloca o cronometro no estado de pausa
 *  @param CRONO_type *CRONO_PTR Referencia para o cronometro
 */
extern void  CRONO_stop ( struct CRONO_type *CRONO_PTR );
/*
 * Coloca o cronometro no estado de execucao
 *  @param CRONO_type *CRONO_PTR Referencia para o cronometro
 */
extern void  CRONO_run ( struct CRONO_type *CRONO_PTR );
/*
 * Verifica o estado da execucao do cronometro
 *  @param CRONO_type *CRONO_PTR Referencia para o cronometro
 *  @returns True caso esteja em execucao False caso contrario
 */
extern bool  CRONO_running_CHECK ( struct CRONO_type *CRONO_PTR );
/*
 * Notifica uma cadencia do cronometro
 *  @param CRONO_type *CRONO_PTR Referencia para o cronometro
 */
extern void  CRONO_detected ( struct CRONO_type *CRONO_PTR );
/*
 * Verica a flag de controle de disparo do cronometro
 *  @param CRONO_type *CRONO_PTR Referencia para o cronometro
 */
extern bool  CRONO_trigger_CHECK ( struct CRONO_type *CRONO_PTR );
/*
 * Atualiza o estado de um cronometro
 * de acordo com o relogio da placa
 *  @param CRONO_type *CRONO_PTR Referencia para um cronometro
 */
extern void  CRONO_atualize ( struct CRONO_type *CRONO_PTR );
#endif
