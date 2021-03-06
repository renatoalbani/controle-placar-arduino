
//***********************************************************************
//	Skecth básico para...
//
//
//***********************************************************************




//***********************************************************************
//	Definição dos modelos (ou tipos) de placas da Plataforma
//	Arduino, suportados por este Sistema:
//=======================================================================

//#define	UNO_rev3	1	// placa modelo "Arduino UNO rev 3".
#define	MEGA_2560	2	// placa modelo "Arduino Mega 2560".

//***********************************************************************




//***********************************************************************
//	Opção para a placa de Hardware utilizada no Sistema:
//
//	UNO = opção para utilização da placa Arduino UNO.
//	MEGA_2560 = opção para utilização da placa Arduino Mega 2560.
//
//=======================================================================

#define	HW_Board 	MEGA_2560	// seleciona a placa utilizada.
//#define	HW_Board 	UNO_rev3	// seleciona a placa utilizada.

//***********************************************************************




//***********************************************************************
//	Opção para execução "real" ou "simulada":
//
//	0 = execução "real" via placa física.
//	1 = execução "simulada" via placa virtual.
//
//=======================================================================

#define	SIMUL	1	// seleciona execução "real" ou "simulada".

//***********************************************************************




//***********************************************************************
//	Opção para disponibilidade dos Placares "Main" e "Piloto":
//
//	0 = Placar ausente.
//	1 = Placar presente.
//
//=======================================================================

#define	Main_DISPLAY_ON		1	// indica se o Placar "Main" está
					// presente no Sistema.
//-----------------------------------------------------------------------
#define	Pilot_DISPLAY_ON	1	// indica se o Placar "Piloto" está
					// presente no Sistema.
//***********************************************************************




//***********************************************************************
//	Bibliotecas utilizadas neste Sistema:
//=======================================================================

#include <stdio.h>	// "stdio" nativo do C++, para Arduino (CPU AVR).
#include <stdlib.h>	// "stdlib" nativo do C++, para Arduino (CPU AVR).

#include <avr/pgmspace.h>	// para acesso à FLASH da CPU.

//***********************************************************************




//***********************************************************************
//	Definição de tipos para facilitar codificação do programa:
//=======================================================================

#define	uint	unsigned int	// tipo inteiros positivos de 16 bits.

#define	ulong	unsigned long	// tipo inteiros positivos de 32 bits.

//-----------------------------------------------------------------------

typedef	unsigned char	BYTE;	// tipo 8 bits, não sinalizado.
typedef	unsigned short	WORD;	// tipo 16 bits, não sinalizado.
typedef	unsigned long	DWORD;	// tipo 32 bits, não sinalizado.

typedef	signed char	sBYTE;	// tipo 8 bits, sinalizado.
typedef	signed short	sWORD;	// tipo 16 bits, sinalizado.
typedef	signed long	sDWORD;	// tipo 32 bits, sinalizado.

//-----------------------------------------------------------------------

typedef	BYTE*	BYTE_PTR;	// tipo Ponteiro para acesso a Bytes.

//***********************************************************************




//***********************************************************************
//	Definição de tipos e funções para acesso à Memória de Programa:
//=======================================================================

#define	ROM_set	const PROGMEM	// definição de dados na Flash da CPU.

#define	ROM_BYTE_ptr	const char *	// definição de um Ponteiro para acesso
					// à Bytes armazenados na Flash.
//-----------------------------------------------------------------------

#define ROM_get8(var)	pgm_read_byte_near(var)	// função para leitura de um
						// Byte armazenado na Flash.

#define ROM_get16(var)	pgm_read_word_near(var)	// função para leitura de uma
						// Word armazenada na Flash.

#define ROM_PTR_get(var) pgm_read_ptr_near(var)	// função para leitura de um
						// Ponteiro armazenado na Flash.
//***********************************************************************




//***********************************************************************
//	Definição de funções para especificar armazenamento de strings
//	na memória de Programa, em métodos de "printagem" serial:
//=======================================================================

#define	ROM_SERIAL_print(s)	Serial.print(F(s))

#define	ROM_SERIAL_LF_print(s)	Serial.println(F(s))

//***********************************************************************




//***********************************************************************
//	Definição de padrões de bit para facilitar codificação do
//	Programa em acessos individuais aos respectivos bits:
//-----------------------------------------------------------------------

#define	BIT_0	0x01	// padrão binário correspondente ao "bit 0".
#define	BIT_1	0x02	// padrão binário correspondente ao "bit 1".
#define	BIT_2	0x04	// padrão binário correspondente ao "bit 2".
#define	BIT_3	0x08	// padrão binário correspondente ao "bit 3".
#define	BIT_4	0x10	// padrão binário correspondente ao "bit 4".
#define	BIT_5	0x20	// padrão binário correspondente ao "bit 5".
#define	BIT_6	0x40	// padrão binário correspondente ao "bit 6".
#define	BIT_7	0x80	// padrão binário correspondente ao "bit 7".

#define	BIT_blank	0x00	// padrão para todos os bits em "0".

//***********************************************************************




//***********************************************************************
//	Definição de uma estrutura para acesso aos padrões de bits dos
//	segmentos de LEDs do Display, e especificação dos pinos de HW
//	para controle do Placar físico:
//=======================================================================

struct	DISPLAY_desc
{
	BYTE_PTR SEGM_code_TAB;	// Tabela de codificação dos segmentos do Display.

	BYTE_PTR TMO_code_TAB;	// Tabela com codificação do Timeout "Home/Guest".

	BYTE_PTR VANT_code_TAB;	// Tabela com codificação do "Vantage".

	bool	polarity;	// polaridade do Display (anodo/katodo comum).

// - - - - - - - - - - - - - - - -

	BYTE SDOUT_pin;		// pino correspondente ao sinal "SDOUT".

	BYTE SCLK_pin;		// pino correspondente ao sinal "SCLK".

	BYTE PCLK_pin;		// pino correspondente ao sinal "PCLK".

} DISPLAY_info;

//***********************************************************************




//***********************************************************************
//	Definição da pinagem relativa aos sinais de Controle de
//	Hardware, quando a placa usada é o Arduino Mega 2560 "real":
//=======================================================================

#if	( ( HW_Board == MEGA_2560 ) && ( SIMUL == 0 ) )	// opção para Arduino
//__________________________________________________	// Mega 2560 "real":

#define	SDOUT_piloto	2	// pino da placa para o sinal "SDOUT" do "Piloto".

#define	SCLK_piloto	6	// pino da placa para o sinal "SCLK" do "Piloto".

#define	PCLK_piloto	4	// pino da placa para o sinal "PCLK" do "Piloto".

// - - - - - - - - - - - - - - - -

#define	SDOUT_main	8	// pino da placa para o sinal "SDOUT" do "Main".

#define	SCLK_main	12	// pino da placa para o sinal "SCLK" do "Main".

#define	PCLK_main	10	// pino da placa para o sinal "PCLK" do "Main".
//__________________________________________________

#endif

//***********************************************************************




//***********************************************************************
//	Definição da pinagem relativa aos sinais de Controle de
//	Hardware, quando a placa usada é o Arduino Mega 2560 "simulado":
//=======================================================================

#if	( ( HW_Board == MEGA_2560 ) && ( SIMUL == 1 ) )	// opção para Arduino
//__________________________________________________	// Mega 2560 simulado:

#define	SDOUT_piloto	5	// pino da placa para o sinal "SDOUT" do "Piloto".

#define	SCLK_piloto	6	// pino da placa para o sinal "SCLK" do "Piloto".

#define	PCLK_piloto	50	// pino da placa para o sinal "PCLK" do "Piloto".

// - - - - - - - - - - - - - - - -

#define	SDOUT_main	5	// pino da placa para o sinal "SDOUT" do "Main".

#define	SCLK_main	6	// pino da placa para o sinal "SCLK" do "Main".

#define	PCLK_main	7	// pino da placa para o sinal "PCLK" do "Main".
//__________________________________________________

#endif

//***********************************************************************




//***********************************************************************
//	Definição da pinagem relativa aos sinais de Controle de
//	Hardware, quando a placa usada é o Arduino UNO rev 3:
//=======================================================================

#if	( HW_Board == UNO_rev3 )	// opção para Arduino UNO rev 3:
//__________________________________________________

#define	SDOUT_piloto	5	// pino da placa para o sinal "SDOUT" do "Piloto".

#define	SCLK_piloto	6	// pino da placa para o sinal "SCLK" do "Piloto".

#define	PCLK_piloto	4	// pino da placa para o sinal "PCLK" do "Piloto".

// - - - - - - - - - - - - - - - -

#define	SDOUT_main	5	// pino da placa para o sinal "SDOUT" do "Main".

#define	SCLK_main	6	// pino da placa para o sinal "SCLK" do "Main".

#define	PCLK_main	7	// pino da placa para o sinal "PCLK" do "Main".
//__________________________________________________

#endif

//***********************************************************************




//***********************************************************************
//
//=======================================================================

#define	ANODO_comum	0	// indica Display tipo "anodo comum".

#define	KATODO_comum	1	// indica Display tipo "katodo comum".

//***********************************************************************




//***********************************************************************
//	Mapeamento dos segmentos do Display no Placar "Main":
//=======================================================================

#define	SEG_aM	BIT_7	// bit correspondente ao Segmento "a".
#define	SEG_bM	BIT_1	// bit correspondente ao Segmento "b".
#define	SEG_cM	BIT_2	// bit correspondente ao Segmento "c".
#define	SEG_dM	BIT_3	// bit correspondente ao Segmento "d".
#define	SEG_eM	BIT_4	// bit correspondente ao Segmento "e".
#define	SEG_fM	BIT_5	// bit correspondente ao Segmento "f".
#define	SEG_gM	BIT_6	// bit correspondente ao Segmento "g".

//***********************************************************************




//***********************************************************************
//	Mapeamento dos segmentos do Display no Placar "Piloto":
//=======================================================================

#define	SEG_aP	BIT_2	// bit correspondente ao Segmento "a".
#define	SEG_bP	BIT_1	// bit correspondente ao Segmento "b".
#define	SEG_cP	BIT_5	// bit correspondente ao Segmento "c".
#define	SEG_dP	BIT_6	// bit correspondente ao Segmento "d".
#define	SEG_eP	BIT_7	// bit correspondente ao Segmento "e".
#define	SEG_fP	BIT_3	// bit correspondente ao Segmento "f".
#define	SEG_gP	BIT_4	// bit correspondente ao Segmento "g".

//***********************************************************************




//***********************************************************************
//	Codificação dos padrões numéricos BCD em 7 segmentos, para o
//	Placar "Main".
//=======================================================================

#define	Seven_SEG_0_patt_M	( SEG_aM + SEG_bM + SEG_cM + SEG_dM + SEG_eM + SEG_fM )
#define	Seven_SEG_1_patt_M	( SEG_bM + SEG_cM )
#define	Seven_SEG_2_patt_M	( SEG_aM + SEG_bM + SEG_dM + SEG_eM + SEG_gM )
#define	Seven_SEG_3_patt_M	( SEG_aM + SEG_bM + SEG_cM + SEG_dM + SEG_gM )
#define	Seven_SEG_4_patt_M	( SEG_bM + SEG_cM + SEG_fM + SEG_gM )
#define	Seven_SEG_5_patt_M	( SEG_aM + SEG_cM + SEG_dM + SEG_fM + SEG_gM )
#define	Seven_SEG_6_patt_M	( SEG_aM + SEG_cM + SEG_dM + SEG_eM + SEG_fM + SEG_gM )
#define	Seven_SEG_7_patt_M	( SEG_aM + SEG_bM + SEG_cM )
#define	Seven_SEG_8_patt_M	( SEG_aM + SEG_bM + SEG_cM + SEG_dM + SEG_eM + SEG_fM + SEG_gM )
#define	Seven_SEG_9_patt_M	( SEG_aM + SEG_bM + SEG_cM + SEG_dM + SEG_fM + SEG_gM )

//***********************************************************************




//***********************************************************************
//	Definição do tipo do Display (Anodo ou Katodo Comum), para o
//	Placar "Main":
//
//	KATODO_comum = opção para Display tipo "Katodo Comum".
//	ANODO_comum  = opção para Display tipo "Anodo Comum".
//
//=======================================================================

#if	( SIMUL == 0 )	// opção para Arduino Mega 2560 "real":

#define Main_DISPLAY_pol	KATODO_comum

#else		// opção para Arduino Mega 2560 "simulado":

#define Main_DISPLAY_pol	KATODO_comum

#endif

//***********************************************************************




//***********************************************************************
//	Codificação dos padrões numéricos BCD em 7 segmentos, para o
//	Placar "Piloto".
//=======================================================================

#define	Seven_SEG_0_patt_P	( SEG_aP + SEG_bP + SEG_cP + SEG_dP + SEG_eP + SEG_fP )
#define	Seven_SEG_1_patt_P	( SEG_bP + SEG_cP )
#define	Seven_SEG_2_patt_P	( SEG_aP + SEG_bP + SEG_dP + SEG_eP + SEG_gP )
#define	Seven_SEG_3_patt_P	( SEG_aP + SEG_bP + SEG_cP + SEG_dP + SEG_gP )
#define	Seven_SEG_4_patt_P	( SEG_bP + SEG_cP + SEG_fP + SEG_gP )
#define	Seven_SEG_5_patt_P	( SEG_aP + SEG_cP + SEG_dP + SEG_fP + SEG_gP )
#define	Seven_SEG_6_patt_P	( SEG_aP + SEG_cP + SEG_dP + SEG_eP + SEG_fP + SEG_gP )
#define	Seven_SEG_7_patt_P	( SEG_aP + SEG_bP + SEG_cP )
#define	Seven_SEG_8_patt_P	( SEG_aP + SEG_bP + SEG_cP + SEG_dP + SEG_eP + SEG_fP + SEG_gP )
#define	Seven_SEG_9_patt_P	( SEG_aP + SEG_bP + SEG_cP + SEG_dP + SEG_fP + SEG_gP )

//***********************************************************************




//***********************************************************************
//	Definição do tipo do Display (Anodo ou Katodo Comum), para o
//	Placar "Piloto":
//
//	KATODO_comum = opção para Display tipo "Katodo Comum".
//	ANODO_comum  = opção para Display tipo "Anodo Comum".
//
//=======================================================================

#if	( SIMUL == 0 )	// opção para Arduino Mega 2560 "real":

#define Pilot_DISPLAY_pol	ANODO_comum

#else		// opção para Arduino Mega 2560 "simulado":

#define Pilot_DISPLAY_pol	KATODO_comum

#endif

//***********************************************************************




//***********************************************************************
//	Tabela para tradução de dígitos BCD para os respectivos padrões
//	numéricos codificados em 7 segmentos, para o Placar "Main":
//=======================================================================

ROM_set  BYTE Seven_SEG_code_M [] =
{				// index:
	Seven_SEG_0_patt_M,	// 0: padrão do "número 0", em 7 segmentos.
	Seven_SEG_1_patt_M,	// 1: padrão do "número 1", em 7 segmentos.
	Seven_SEG_2_patt_M,	// 2: padrão do "número 2", em 7 segmentos.
	Seven_SEG_3_patt_M,	// 3: padrão do "número 3", em 7 segmentos.
	Seven_SEG_4_patt_M,	// 4: padrão do "número 4", em 7 segmentos.
	Seven_SEG_5_patt_M,	// 5: padrão do "número 5", em 7 segmentos.
	Seven_SEG_6_patt_M,	// 6: padrão do "número 6", em 7 segmentos.
	Seven_SEG_7_patt_M,	// 7: padrão do "número 7", em 7 segmentos.
	Seven_SEG_8_patt_M,	// 8: padrão do "número 8", em 7 segmentos.
	Seven_SEG_9_patt_M	// 9: padrão do "número 9", em 7 segmentos.
};
//***********************************************************************




//***********************************************************************
//	Tabela para tradução de dígitos BCD para os respectivos padrões
//	numéricos codificados em 7 segmentos, para o Placar "Piloto":
//=======================================================================

ROM_set  BYTE Seven_SEG_code_P [] =
{				// index:
	Seven_SEG_0_patt_P,	// 0: padrão do "número 0", em 7 segmentos.
	Seven_SEG_1_patt_P,	// 1: padrão do "número 1", em 7 segmentos.
	Seven_SEG_2_patt_P,	// 2: padrão do "número 2", em 7 segmentos.
	Seven_SEG_3_patt_P,	// 3: padrão do "número 3", em 7 segmentos.
	Seven_SEG_4_patt_P,	// 4: padrão do "número 4", em 7 segmentos.
	Seven_SEG_5_patt_P,	// 5: padrão do "número 5", em 7 segmentos.
	Seven_SEG_6_patt_P,	// 6: padrão do "número 6", em 7 segmentos.
	Seven_SEG_7_patt_P,	// 7: padrão do "número 7", em 7 segmentos.
	Seven_SEG_8_patt_P,	// 8: padrão do "número 8", em 7 segmentos.
	Seven_SEG_9_patt_P	// 9: padrão do "número 9", em 7 segmentos.
};
//***********************************************************************




//***********************************************************************
//	Codificação dos padrões dos segmentos de "timeout", para o
//	Placar "Main":
//=======================================================================

#define	HOME_TMO_0_patt_M	( BIT_blank )
#define	HOME_TMO_1_patt_M	( BIT_3 )
#define	HOME_TMO_2_patt_M	( BIT_3 + BIT_2 )
#define	HOME_TMO_3_patt_M	( BIT_3 + BIT_2 + BIT_1 )

#define	HOME_TMO_mask_M		( BIT_3 + BIT_2 + BIT_1 )

//-----------------------------------------------------------------------

#define	GUEST_TMO_0_patt_M	( BIT_blank )
#define	GUEST_TMO_1_patt_M	( BIT_6 )
#define	GUEST_TMO_2_patt_M	( BIT_6 + BIT_5 )
#define	GUEST_TMO_3_patt_M	( BIT_6 + BIT_5 + BIT_4 )

#define	GUEST_TMO_mask_M	( BIT_6 + BIT_5 + BIT_4 )

//***********************************************************************




//***********************************************************************
//	Codificação dos padrões dos segmentos de "timeout", para o
//	Placar "Piloto":
//=======================================================================

#define	HOME_TMO_0_patt_P	( BIT_blank )
#define	HOME_TMO_1_patt_P	( BIT_3 )
#define	HOME_TMO_2_patt_P	( BIT_3 + BIT_2 )
#define	HOME_TMO_3_patt_P	( BIT_3 + BIT_2 + BIT_1 )

#define	HOME_TMO_mask_P		( BIT_3 + BIT_2 + BIT_1 )

//-----------------------------------------------------------------------

#define	GUEST_TMO_0_patt_P	( BIT_blank )
#define	GUEST_TMO_1_patt_P	( BIT_6 )
#define	GUEST_TMO_2_patt_P	( BIT_6 + BIT_5 )
#define	GUEST_TMO_3_patt_P	( BIT_6 + BIT_5 + BIT_4 )

#define	GUEST_TMO_mask_P	( BIT_6 + BIT_5 + BIT_4 )

//***********************************************************************




//***********************************************************************
//
//=======================================================================

ROM_set  BYTE TMO_code_TAB_M [] =
{						// index:
	HOME_TMO_0_patt_M, GUEST_TMO_0_patt_M,	// 0:
	HOME_TMO_1_patt_M, GUEST_TMO_1_patt_M,	// 1:
	HOME_TMO_2_patt_M, GUEST_TMO_2_patt_M,	// 2:
	HOME_TMO_3_patt_M, GUEST_TMO_3_patt_M,	// 3:

	HOME_TMO_mask_M, GUEST_TMO_mask_M	// 4: máscara para os bits
};						// de timeout.
//***********************************************************************




//***********************************************************************
//
//=======================================================================

ROM_set  BYTE TMO_code_TAB_P [] =
{						// index:
	HOME_TMO_0_patt_P, GUEST_TMO_0_patt_P,	// 0:
	HOME_TMO_1_patt_P, GUEST_TMO_1_patt_P,	// 1:
	HOME_TMO_2_patt_P, GUEST_TMO_2_patt_P,	// 2:
	HOME_TMO_3_patt_P, GUEST_TMO_3_patt_P,	// 3:

	HOME_TMO_mask_P, GUEST_TMO_mask_P	// 4: máscara para os bits
};						// de timeout.
//***********************************************************************




//***********************************************************************
//
//=======================================================================

enum { HOME_VANTAGE, GUEST_VANTAGE };

//***********************************************************************




//***********************************************************************
//	Codificação dos padrões dos segmentos de "vantagem", para o
//	Placar "Main":
//=======================================================================

#define	HOME_VANT_patt_M	BIT_7

#define	GUEST_VANT_patt_M	BIT_0

//-----------------------------------------------------------------------

#define	VANT_PATT_mask_M	( HOME_VANT_patt_M + GUEST_VANT_patt_M )

//***********************************************************************




//***********************************************************************
//	Codificação dos padrões dos segmentos de "vantagem", para o
//	Placar "Piloto":
//=======================================================================

#define	HOME_VANT_patt_P	BIT_7

#define	GUEST_VANT_patt_P	BIT_0

//-----------------------------------------------------------------------

#define	VANT_PATT_mask_P	( HOME_VANT_patt_P + GUEST_VANT_patt_P )

//***********************************************************************




//***********************************************************************
//
//=======================================================================

ROM_set  BYTE VANT_code_TAB_M [] =
{				// index:
	HOME_VANT_patt_M,	// 0: padrão para "Home Vantage".
	GUEST_VANT_patt_M,	// 1: padrão para "Guest Vantage".
	VANT_PATT_mask_M	// 2: máscara para os bits "Vantage".
};
//***********************************************************************




//***********************************************************************
//
//=======================================================================

ROM_set  BYTE VANT_code_TAB_P [] =
{				// index:
	HOME_VANT_patt_P,	// 0: padrão para "Home Vantage".
	GUEST_VANT_patt_P,	// 1: padrão para "Guest Vantage".
	VANT_PATT_mask_P	// 2: máscara para os bits "Vantage".
};
//***********************************************************************




//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//
//
//
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\




//***********************************************************************
//
//=======================================================================

#define	VIEW_OFF	0

#define	VIEW_ON		1

//***********************************************************************




//***********************************************************************
//
//=======================================================================

typedef union
{
	WORD	VIEW_STS;	// "view status" geral do Display.

	struct			// flags individuais de "view status":
	{
		WORD GAME_TIME	:1;	// "view status" do indicador...
		WORD GAME_quarter:1;	// "view status" do indicador...
//__________________________________________________

		WORD BALL_pos	:1;	// "view status" do indicador...
		WORD BALL_down	:1;	// "view status" do indicador...
		WORD BALL_to_go	:1;	// "view status" do indicador...
//__________________________________________________

		WORD SCORE_home	:1;	// "view status" do indicador...
		WORD VANT_home	:1;	// "view status" do indicador...
		WORD TMOUT_home	:1;	// "view status" do indicador...
//__________________________________________________

		WORD SCORE_guest:1;	// "view status" do indicador...
		WORD VANT_guest	:1;	// "view status" do indicador...
		WORD TMOUT_guest:1;	// "view status" do indicador...
//__________________________________________________

		WORD PLAY_CLOCK	:1;	// // "view status" do indicador...
	};

} DISPLAY_VIEW_type;

//-----------------------------------------------------------------------

DISPLAY_VIEW_type	DISPLAY_VIEW_STS;

//***********************************************************************




//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//
//
//
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\




//***********************************************************************
//
//=======================================================================

BYTE	TMO_VANT_REG = 0;	// espelho do Registro de HW com bits do
				// "timeout" e "vantage".
//***********************************************************************




//***********************************************************************
//	Definição dos elementos referentes ao tempo do jogo:
//=======================================================================

BYTE	GAME_TIME_min;	// indica os minutos do tempo de jogo (00..59).

BYTE	GAME_TIME_seg;	// indica os segundos do tempo de jogo (00..59).

//***********************************************************************




//***********************************************************************
//	Definição dos elementos referentes ao "spot de bola":
//=======================================================================

BYTE	BALL_position;	// indica a posição atual da Bola (00..99).

BYTE	BALL_down;	// indica o "down" da Bola (0..9).

BYTE	BALL_to_go;	// indica o "to go" da Bola (00..99).

//***********************************************************************




//***********************************************************************
//	Definição dos elementos referentes à Pontuação e Timeout da 
//	"casa" e dos "visitantes", da "vantagem" atual, e do "quarter"
//	atual do jogo.
//=======================================================================

BYTE	SCORE_home;	// indica a pontuação da "casa" (00..99).

BYTE	TMOUT_home;	// indica o "timeout" da "casa" (0..3).

//-----------------------------------------------------------------------

BYTE	SCORE_guest;	// indica a pontuação do "visitante" (00..99).

BYTE	TMOUT_guest;	// indica o "timeout" do "visitante" (0..3).

//-----------------------------------------------------------------------

BYTE	GAME_VANTAGE;	// indica a vantagem atual no jogo.

//-----------------------------------------------------------------------

BYTE	GAME_quarter;	// indica o "quarter" do jogo (1, 2, 3, ou 4).

//***********************************************************************




//***********************************************************************
//	Definição do elemento referente ao "Play Clock" atual do jogo:
//=======================================================================

BYTE	PLAY_CLOCK;	// indica o "Play Clock" atual do jogo (00..99).

//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	PLAY_CLOCK_swap = false;	// indica se o "smart swap" está
					// ativo para o "Play Clock".
//***********************************************************************




//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//
//
//
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\




//***********************************************************************
//
//=======================================================================

void	HW_SERIAL_OUT_8 ( BYTE DATA_8 )
{
//--------------------------------------------------
BYTE	bit_n;	// bit atual sendo enviado ao ShiftRegister.

BYTE	bit_MASK;	// máscara binária para o bit atual.
//--------------------------------------------------

	bit_MASK = BIT_7;

	for ( bit_n = 0; bit_n <=7; bit_n++ )
	{
		if ( DATA_8 & bit_MASK )
		{
			digitalWrite ( DISPLAY_info.SDOUT_pin, HIGH );

		}
		else
		{
			digitalWrite ( DISPLAY_info.SDOUT_pin, LOW );
		}

		digitalWrite ( DISPLAY_info.SCLK_pin, LOW );

		delayMicroseconds(1);

		digitalWrite ( DISPLAY_info.SCLK_pin, HIGH );

		delayMicroseconds(1);

		bit_MASK = ( bit_MASK >> 1);
	}
//..................................................

	delayMicroseconds(1);
	digitalWrite ( DISPLAY_info.SDOUT_pin, HIGH );
	digitalWrite ( DISPLAY_info.SCLK_pin, LOW );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	HW_DISPLAY_REG_actualize ()
{
	digitalWrite ( DISPLAY_info.PCLK_pin, HIGH );

	delayMicroseconds(1);

	digitalWrite ( DISPLAY_info.PCLK_pin, LOW );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	DISPLAY_PATTERN_send ( BYTE PATTERN )
{
	if ( DISPLAY_info.polarity == 0 ) PATTERN = ~PATTERN;

	HW_SERIAL_OUT_8 ( PATTERN );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	DISPLAY_DIGIT_send ( BYTE DATA )
{
//--------------------------------------------------
BYTE	LSB;
BYTE	PATTERN;
//--------------------------------------------------

	LSB = ( DATA & 0x0F );

	PATTERN = ROM_get8 ( DISPLAY_info.SEGM_code_TAB + LSB );

	if ( DISPLAY_info.polarity == 0 ) PATTERN = ~PATTERN;

	HW_SERIAL_OUT_8 ( PATTERN );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	DISPLAY_BCD_BYTE_send ( BYTE DATA )
{
//--------------------------------------------------
BYTE	LSB;
BYTE	MSB;
BYTE	PATTERN;
//--------------------------------------------------

	LSB = ( DATA & 0x0F );
	MSB = ( DATA & 0xF0 ) >> 4;
//..................................................

	PATTERN = ROM_get8 ( DISPLAY_info.SEGM_code_TAB + LSB );

	if ( DISPLAY_info.polarity == 0 ) PATTERN = ~PATTERN;

	HW_SERIAL_OUT_8 ( PATTERN );
//..................................................

	PATTERN = ROM_get8 ( DISPLAY_info.SEGM_code_TAB + MSB );

	if ( DISPLAY_info.polarity == 0 ) PATTERN = ~PATTERN;

	HW_SERIAL_OUT_8 ( PATTERN );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

BYTE	BOOL_extent ( bool STS )
{
	return ( STS? 0xFF : 0 );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

BYTE	HEX8_to_BCD8 ( BYTE HEX_8 )
{
//--------------------------------------------------
BYTE	BCD_result;
BYTE	i;
//--------------------------------------------------

	BCD_result = 0;

	if ( HEX_8 < 100 )
	{
		i = 0;

		while ( HEX_8 >= 10 )
		{
			BCD_result += ( ( HEX_8 % 10 ) << i );

			HEX_8 /= 10;

			i += 4;
		}

		BCD_result += ( HEX_8 << i );
	}

	return ( BCD_result );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

BYTE	BCD8_to_HEX8 ( BYTE BCD_8 )
{
	return ( 10*( ( BCD_8 & 0xF0 ) >> 4 ) + ( BCD_8 & 0x0F ) );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

WORD	BCD16_to_HEX16 ( WORD BCD_16 )
{
//--------------------------------------------------
WORD	HEX_16;
//--------------------------------------------------

	HEX_16 = ( BCD_16 & 0x000F );
	HEX_16 += 10*( ( BCD_16 & 0x00F0 ) >> 4 );
	HEX_16 += 100*( ( BCD_16 & 0x0F00 ) >> 8 );
	HEX_16 += 1000*( ( BCD_16 & 0xF000 ) >> 12 );

	return ( HEX_16 );
}
//***********************************************************************




//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//
//
//
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\




//***********************************************************************
//
//=======================================================================

void	BLANK_PATTERN_send ()
{
	DISPLAY_PATTERN_send (0);
}
//***********************************************************************




//***********************************************************************

void	BLANK_PATTERN_DOUBLE_send ()
{
	DISPLAY_PATTERN_send (0);
	DISPLAY_PATTERN_send (0);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	GAME_TIME_send ( BYTE minutos, BYTE segundos )
{
	if ( DISPLAY_VIEW_STS.GAME_TIME )
	{
		DISPLAY_BCD_BYTE_send ( HEX8_to_BCD8 ( segundos ) );
		DISPLAY_BCD_BYTE_send ( HEX8_to_BCD8 ( minutos ) );
	}
	else
	{
		BLANK_PATTERN_DOUBLE_send ();
		BLANK_PATTERN_DOUBLE_send ();
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	BALL_POSITION_send ( BYTE BALL_pos )
{
	if ( DISPLAY_VIEW_STS.BALL_pos )
	{
		DISPLAY_BCD_BYTE_send ( HEX8_to_BCD8 ( BALL_pos ) );
	}
	else	BLANK_PATTERN_DOUBLE_send ();
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	BALL_DOWN_send ( BYTE BALL_down )
{
	if ( DISPLAY_VIEW_STS.BALL_down )
	{
		DISPLAY_DIGIT_send ( BALL_down );
	}
	else	BLANK_PATTERN_send ();
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	BALL_to_GO_send ( BYTE BALL_to_go )
{
	if ( DISPLAY_VIEW_STS.BALL_to_go )
	{
		DISPLAY_BCD_BYTE_send ( HEX8_to_BCD8 ( BALL_to_go ) );
	}
	else	BLANK_PATTERN_DOUBLE_send ();
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	SCORE_HOME_send ( BYTE SCORE_home )
{
	if ( DISPLAY_VIEW_STS.SCORE_home )
	{
		DISPLAY_BCD_BYTE_send ( HEX8_to_BCD8 ( SCORE_home ) );
	}
	else	BLANK_PATTERN_DOUBLE_send ();
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	SCORE_GUEST_send ( BYTE SCORE_guest )
{
	if ( DISPLAY_VIEW_STS.SCORE_guest )
	{
		DISPLAY_BCD_BYTE_send ( HEX8_to_BCD8 ( SCORE_guest ) );
	}
	else	BLANK_PATTERN_DOUBLE_send ();
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	GAME_QUARTER_send ( BYTE GAME_quarter )
{
	if ( DISPLAY_VIEW_STS.GAME_quarter )
	{
		DISPLAY_DIGIT_send ( GAME_quarter );
	}
	else	BLANK_PATTERN_send ();
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLAY_CLOCK_send ( BYTE PLAY_CLOCK )
{
	if ( DISPLAY_VIEW_STS.PLAY_CLOCK )
	{
		DISPLAY_BCD_BYTE_send ( HEX8_to_BCD8 ( PLAY_CLOCK ) );
	}
	else	BLANK_PATTERN_DOUBLE_send ();
}
//***********************************************************************




//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//
//
//
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\




//***********************************************************************
//
//=======================================================================

void	TMO_VANT_REG_send ()
{
	DISPLAY_PATTERN_send ( TMO_VANT_REG );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	VANTAGE_actualize ( BYTE vantage )
{
//--------------------------------------------------
BYTE	VANT_patt;
//--------------------------------------------------

	if ( vantage == HOME_VANTAGE )
	{
		VANT_patt = ROM_get8 ( DISPLAY_info.VANT_code_TAB + 0 );	// obtém padrão de bits.

		VANT_patt &= BOOL_extent( DISPLAY_VIEW_STS.VANT_home );
	}
	else
	{
		VANT_patt = ROM_get8 ( DISPLAY_info.VANT_code_TAB + 1 );	// obtém padrão de bits.

		VANT_patt &= BOOL_extent( DISPLAY_VIEW_STS.VANT_guest );
	}
//..................................................

	TMO_VANT_REG &= (~ROM_get8 ( DISPLAY_info.VANT_code_TAB + 2 ) );	// aplica máscara.

	TMO_VANT_REG |= VANT_patt;
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	HOME_TIMEOUT_actualize ( BYTE timeout )
{
//--------------------------------------------------
BYTE	TMO_patt;
BYTE	index;
//--------------------------------------------------

	index = 2*timeout;

	TMO_patt = ROM_get8 ( DISPLAY_info.TMO_code_TAB + index );	// obtém padrão de bits.

	TMO_patt &= BOOL_extent( DISPLAY_VIEW_STS.TMOUT_home );
//..................................................

	TMO_VANT_REG &= ( ~ROM_get8 ( DISPLAY_info.TMO_code_TAB + 2*4 ) );	// aplica máscara.

	TMO_VANT_REG |= TMO_patt;
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	GUEST_TIMEOUT_actualize ( BYTE timeout )
{
//--------------------------------------------------
BYTE	TMO_patt;
BYTE	index;
//--------------------------------------------------

	index = 2*timeout +1;

	TMO_patt = ROM_get8 ( DISPLAY_info.TMO_code_TAB + index );	// obtém padrão de bits.

	TMO_patt &= BOOL_extent( DISPLAY_VIEW_STS.TMOUT_guest );
//..................................................

	TMO_VANT_REG &= ( ~ROM_get8 ( DISPLAY_info.TMO_code_TAB + 2*4 +1 ) );	// aplica máscara.

	TMO_VANT_REG |= TMO_patt;
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	TIMEOUT_VANTAGE_send ( BYTE HOME_tmo, BYTE GUEST_tmo, BYTE vantage )
{
	HOME_TIMEOUT_actualize ( HOME_tmo );

	GUEST_TIMEOUT_actualize ( GUEST_tmo );

	VANTAGE_actualize ( vantage );

	TMO_VANT_REG_send ();
}
//***********************************************************************




//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//
//
//
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\




//***********************************************************************
//ORDEM DOS PLACARES
//=======================================================================

void	PLACAR_DISPLAY_normal_serialize ()
{
  PLAY_CLOCK_send ( PLAY_CLOCK );

  GAME_TIME_send ( GAME_TIME_min, GAME_TIME_seg );

  TIMEOUT_VANTAGE_send ( TMOUT_home, TMOUT_guest, GAME_VANTAGE );

  GAME_QUARTER_send ( GAME_quarter );

  SCORE_GUEST_send ( SCORE_guest );

  SCORE_HOME_send ( SCORE_home );



  BALL_to_GO_send ( BALL_to_go );

  BALL_DOWN_send ( BALL_down );

  BALL_POSITION_send ( BALL_position );

  GAME_TIME_send ( GAME_TIME_min, GAME_TIME_seg );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLACAR_DISPLAY_swap_serialize ()
{
  PLAY_CLOCK_send ( PLAY_CLOCK );

  GAME_TIME_send ( GAME_TIME_min, GAME_TIME_seg );

  TIMEOUT_VANTAGE_send ( TMOUT_home, TMOUT_guest, GAME_VANTAGE );

  GAME_QUARTER_send ( GAME_quarter );

  SCORE_GUEST_send ( SCORE_guest );

  SCORE_HOME_send ( SCORE_home );

 

  BALL_to_GO_send ( BALL_to_go );

  BALL_DOWN_send ( BALL_down );

  BALL_POSITION_send ( PLAY_CLOCK );  // overlap "Ball" com o "Play Clock".

  GAME_TIME_send ( GAME_TIME_min, GAME_TIME_seg );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLACAR_DISPLAY_refresh ()
{
	if ( !( PLAY_CLOCK_swap && PLAY_CLOCK_running_CHECK() ) )

		PLACAR_DISPLAY_normal_serialize();

	else	PLACAR_DISPLAY_swap_serialize();
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLACAR_MAIN_DISPLAY_refresh ()
{
#if	( Main_DISPLAY_ON == 1 )

	DISPLAY_info.SEGM_code_TAB = (BYTE_PTR) Seven_SEG_code_M;	// Tabela de codificação dos segmentos do Display.

	DISPLAY_info.TMO_code_TAB = (BYTE_PTR) TMO_code_TAB_M;		// Tabela com codificação do Timeout "Home/Guest".

	DISPLAY_info.VANT_code_TAB = (BYTE_PTR) VANT_code_TAB_M;	// Tabela com codificação do "Vantage".

	DISPLAY_info.polarity = Main_DISPLAY_pol;	// polaridade do Display (anodo/katodo comum).

// - - - - - - - - - - - - - - - -

	DISPLAY_info.SDOUT_pin = SDOUT_main;	// especifica pino "SDOUT" para o "Main".

	DISPLAY_info.SCLK_pin = SCLK_main;	// especifica pino "SCLK" para o "Main".

	DISPLAY_info.PCLK_pin = PCLK_main;	// especifica pino "PCLK" para o "Main".

//..................................................

	PLACAR_DISPLAY_refresh ();

	delayMicroseconds(10);

	HW_DISPLAY_REG_actualize ();

#endif
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLACAR_PILOT_DISPLAY_refresh ()
{
#if	( Pilot_DISPLAY_ON == 1 )

	DISPLAY_info.SEGM_code_TAB = (BYTE_PTR) Seven_SEG_code_P;	// Tabela de codificação dos segmentos do Display.

	DISPLAY_info.TMO_code_TAB = (BYTE_PTR) TMO_code_TAB_P;		// Tabela com codificação do Timeout "Home/Guest".

	DISPLAY_info.VANT_code_TAB = (BYTE_PTR) VANT_code_TAB_P;	// Tabela com codificação do "Vantage".

	DISPLAY_info.polarity = Pilot_DISPLAY_pol;	// polaridade do Display (anodo/katodo comum).

// - - - - - - - - - - - - - - - -

	DISPLAY_info.SDOUT_pin = SDOUT_piloto;	// especifica pino "SDOUT" para o "Piloto".

	DISPLAY_info.SCLK_pin = SCLK_piloto;	// especifica pino "SCLK" para o "Piloto".

	DISPLAY_info.PCLK_pin = PCLK_piloto;	// especifica pino "PCLK" para o "Piloto".

//..................................................

	PLACAR_DISPLAY_refresh ();

	delayMicroseconds(10);

	HW_DISPLAY_REG_actualize ();

#endif
}
//***********************************************************************




//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//
//
//
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\




//***********************************************************************
//
//=======================================================================

void	DISPLAY_view_SET ( bool view )
{
	if (view)
	{
		DISPLAY_VIEW_STS.VIEW_STS = 0xFFFF;
	}
	else
	{
		DISPLAY_VIEW_STS.VIEW_STS = 0;
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	GAME_TIME_view_SET ( bool view )
{
	DISPLAY_VIEW_STS.GAME_TIME = view;
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	GAME_QUARTER_view_SET ( bool view )
{
	DISPLAY_VIEW_STS.GAME_quarter = view;
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	BALL_POSITION_view_SET ( bool view )
{
	DISPLAY_VIEW_STS.BALL_pos = view;
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	BALL_DOWN_view_SET ( bool view )
{
	DISPLAY_VIEW_STS.BALL_down = view;
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	BALL_TO_GO_view_SET ( bool view )
{
	DISPLAY_VIEW_STS.BALL_to_go = view;
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	SCORE_HOME_view_SET ( bool view )
{
	DISPLAY_VIEW_STS.SCORE_home = view;
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	VANT_HOME_view_SET ( bool view )
{
	DISPLAY_VIEW_STS.VANT_home = view;
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	TMOUT_HOME_view_SET ( bool view )
{
	DISPLAY_VIEW_STS.TMOUT_home = view;
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	SCORE_GUEST_view_SET ( bool view )
{
	DISPLAY_VIEW_STS.SCORE_guest = view;
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	VANT_GUEST_view_SET ( bool view )
{
	DISPLAY_VIEW_STS.VANT_guest = view;
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	TMOUT_GUEST_view_SET ( bool view )
{
	DISPLAY_VIEW_STS.TMOUT_guest = view;
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLAY_CLOCK_view_SET ( bool view )
{
	DISPLAY_VIEW_STS.PLAY_CLOCK = view;
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	DISPLAY_BLANK_refresh ()
{


}
//***********************************************************************




//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//
//
//
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\




//***********************************************************************
//
//=======================================================================

#if	( SIMUL == 0 )

#define	Second_ms	1000

#else

#define	Second_ms	50

#endif

//***********************************************************************




//***********************************************************************
//
//=======================================================================

struct	CRONO_type
{
	uint periodo_ms;	// período de disparo do cronômetro.

	bool trigger;		// indica que ocorreu um evento do cronômetro.

	bool run;		// habilita cadência do cronômetro.

	void (*CRONO_notify) ();	// função para informe da cadência.

	ulong registro_ms;	// uso interno para detecção de evento.
};
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	CRONO_DUMMY_notify ()
{
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	CRONO_init ( struct CRONO_type *CRONO_PTR, uint periodo_ms )
{
	(*CRONO_PTR).periodo_ms = periodo_ms;	// seta o período de disparo do cronômetro.

	(*CRONO_PTR).trigger = false;		// reseta indicador de evento.

	(*CRONO_PTR).run = false;		// mantém cronômetro parado.

	(*CRONO_PTR).registro_ms = millis();	// reseta referência para
						// atualização do cronômetro.

	(*CRONO_PTR).CRONO_notify = CRONO_DUMMY_notify;	// "desliga" informe da cadência.
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	CRONO_notify_ON ( struct CRONO_type *CRONO_PTR, void (*CRONO_notify) () )
{
	(*CRONO_PTR).CRONO_notify = CRONO_notify;	// seta função para
							// informe da cadência.
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	CRONO_notify_OFF ( struct CRONO_type *CRONO_PTR )
{
	(*CRONO_PTR).CRONO_notify = CRONO_DUMMY_notify;	// "desliga" informe
							// da cadência.
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	CRONO_stop ( struct CRONO_type *CRONO_PTR )
{
	(*CRONO_PTR).run = false;	// mantém cronômetro parado.

	(*CRONO_PTR).trigger = false;	// reseta indicador de evento.
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	CRONO_run ( struct CRONO_type *CRONO_PTR )
{
	(*CRONO_PTR).run = true;	// libera o cronômetro.

	(*CRONO_PTR).trigger = false;	// reseta indicador de evento.

	(*CRONO_PTR).registro_ms = millis();	// reseta referência para
						// atualização do cronômetro.
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	CRONO_running_CHECK ( struct CRONO_type *CRONO_PTR )
{
	return ( (*CRONO_PTR).run );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	CRONO_detected ( struct CRONO_type *CRONO_PTR )
{
	(*CRONO_PTR).trigger = false;	// reseta indicador de evento.

	(*CRONO_PTR).CRONO_notify ();	// informa a cadência do cronômetro.
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	CRONO_trigger_CHECK ( struct CRONO_type *CRONO_PTR )
{
	return ( (*CRONO_PTR).trigger );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	CRONO_atualize ( struct CRONO_type *CRONO_PTR )
{
//--------------------------------------------------
ulong atual_Millis;
//--------------------------------------------------

	if ( (*CRONO_PTR).run )
	{
		atual_Millis = millis();	// obtém valor atual do "millis".

		if ( ( atual_Millis - (*CRONO_PTR).registro_ms ) >= (*CRONO_PTR).periodo_ms )
		{
			(*CRONO_PTR).trigger = true;	// se ocorreu um "trigger", indica isso.

			(*CRONO_PTR).registro_ms = atual_Millis;	// seta nova referência para
		}							// próxima atualização.
	}
	else
	{
		(*CRONO_PTR).trigger = false;		// reseta indicador de evento.

		(*CRONO_PTR).registro_ms = millis();	// reseta referência para
	}						// atualização do cronômetro.
}
//***********************************************************************




//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//
//
//
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\




//***********************************************************************
//	Definição de um contador para controle do cadenciamento de
//	operações "HMI" (Interface Homem/Máquina) neste Sistema:
//=======================================================================

WORD	SYS_COUNTER_100ms = 0;

//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	SYS_COUNTER_100ms_increment ()
{
	SYS_COUNTER_100ms++;

	if ( SYS_COUNTER_100ms > 1000 )
	{
		SYS_COUNTER_100ms = 0;
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

#if	( SIMUL == 0 )

#define	SYS_100ms_CRONO_RATE	( Second_ms/10 )

#else

#define	SYS_100ms_CRONO_RATE	( Second_ms/10 )

#endif

//***********************************************************************




//***********************************************************************
//	Definição de um cronômetro de 100ms para cadenciamento de
//	operações "HMI" (Interface Homem/Máquina) neste Sistema:
//=======================================================================

CRONO_type	SYS_100ms_CRONO;	// cronômetro para cadenciamento
					// de operações "HMI" no Sistema.
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	SYS_100ms_CRONO_startup ()
{
	CRONO_init ( &SYS_100ms_CRONO, SYS_100ms_CRONO_RATE );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	SYS_100ms_CRONO_run ()
{
	CRONO_run ( &SYS_100ms_CRONO );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	SYS_100ms_CRONO_cadence ()
{
	CRONO_atualize ( &SYS_100ms_CRONO );

	if ( CRONO_trigger_CHECK ( &SYS_100ms_CRONO ) )
	{
		SYS_COUNTER_100ms_increment ();

		CRONO_detected ( &SYS_100ms_CRONO );
	}
}
//***********************************************************************




//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//
//
//
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\




//***********************************************************************
//
//=======================================================================

#if	( SIMUL == 0 )

#define	DISPLAY_refresh_RATE	( Second_ms/4 )

#else

#define	DISPLAY_refresh_RATE	( Second_ms/4 )

#endif

//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	DISPLAY_refresh_force = false;

//***********************************************************************




//***********************************************************************
//	Definição de um cronômetro para a taxa de atualização do
//	Display.
//=======================================================================

CRONO_type	DISPLAY_refresh_CRONO;	// cronômetro para temporização
					// da atualização do Display.
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	DISPLAY_refresh_startup ()
{
	CRONO_init ( &DISPLAY_refresh_CRONO, DISPLAY_refresh_RATE );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	DISPLAY_refresh_stop ()
{
	CRONO_stop ( &DISPLAY_refresh_CRONO );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	DISPLAY_refresh_run ()
{
	CRONO_run ( &DISPLAY_refresh_CRONO );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	DISPLAY_refresh_cadence ()
{
	CRONO_atualize ( &DISPLAY_refresh_CRONO );

	if ( CRONO_trigger_CHECK ( &DISPLAY_refresh_CRONO ) )
	{
		DISPLAY_refresh_force = true;

		CRONO_detected ( &DISPLAY_refresh_CRONO );
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	DISPLAY_refresh_make ()
{
	if ( DISPLAY_refresh_force )
	{
		DISPLAY_refresh_force = false;

		PLACAR_PILOT_DISPLAY_refresh ();

		delayMicroseconds(50);

		PLACAR_MAIN_DISPLAY_refresh ();
	}
}
//***********************************************************************




//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//
//
//
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\




//***********************************************************************
//	Definição de um cronômetro para a cadência do "Play Clock":
//=======================================================================

CRONO_type	PLAY_CLOCK_CRONO;	// cronômetro para cadência do
					// "Play Clock".
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLAY_CLOCK_startup ()
{
	CRONO_init ( &PLAY_CLOCK_CRONO, Second_ms );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLAY_CLOCK_notify_ON ( void (*PLAY_CLOCK_notify) () )
{
	CRONO_notify_ON ( &PLAY_CLOCK_CRONO, PLAY_CLOCK_notify );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLAY_CLOCK_notify_OFF ()
{
	CRONO_notify_OFF ( &PLAY_CLOCK_CRONO );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLAY_CLOCK_stop ()
{
	CRONO_stop ( &PLAY_CLOCK_CRONO );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLAY_CLOCK_run ()
{
	CRONO_run ( &PLAY_CLOCK_CRONO );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	PLAY_CLOCK_running_CHECK ()
{
	return ( CRONO_running_CHECK ( &PLAY_CLOCK_CRONO ) );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLAY_CLOCK_cadence ()
{
	if ( PLAY_CLOCK != 0 )
	{
		CRONO_atualize ( &PLAY_CLOCK_CRONO );

		if ( CRONO_trigger_CHECK ( &PLAY_CLOCK_CRONO ) )
		{
			PLAY_CLOCK--;

			CRONO_detected ( &PLAY_CLOCK_CRONO );
		}
	}
	else	CRONO_stop ( &PLAY_CLOCK_CRONO );
}
//***********************************************************************




//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//
//
//
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\




//***********************************************************************
//	Definição de um cronômetro para a cadência do "Game Time":
//=======================================================================

CRONO_type	GAME_TIME_CRONO;	// cronômetro para cadência do
					// "Game Time".
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	GAME_TIME_startup ()
{
	CRONO_init ( &GAME_TIME_CRONO, Second_ms );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	GAME_TIME_notify_ON ( void (*GAME_TIME_notify) () )
{
	CRONO_notify_ON ( &GAME_TIME_CRONO, GAME_TIME_notify );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	GAME_TIME_notify_OFF ()
{
	CRONO_notify_OFF ( &GAME_TIME_CRONO );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	GAME_TIME_stop ()
{
	CRONO_stop ( &GAME_TIME_CRONO );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	GAME_TIME_run ()
{
	CRONO_run ( &GAME_TIME_CRONO );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	GAME_TIME_running_CHECK ()
{
	return ( CRONO_running_CHECK ( &GAME_TIME_CRONO ) );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	GAME_TIME_cadence ()
{
	if ( ( GAME_TIME_seg != 0 ) || ( GAME_TIME_min != 0 ) )
	{
		CRONO_atualize ( &GAME_TIME_CRONO );

		if ( CRONO_trigger_CHECK ( &GAME_TIME_CRONO ) )
		{
			if ( GAME_TIME_seg > 0 )
			{
				GAME_TIME_seg--;
			}
			else
			{
				GAME_TIME_seg = 59;

				GAME_TIME_min--;
			}

			CRONO_detected ( &GAME_TIME_CRONO );
		}
	}
	else	CRONO_stop ( &GAME_TIME_CRONO );
}
//***********************************************************************




//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//
//
//
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\




//***********************************************************************
//	Definição do caracter de finalização de um comando:
//=======================================================================

#if	( SIMUL == 0 )

#define	CMD_end	0x0A	// caracter de finalização de comando = "line feed".

#else

#define	CMD_end	'.'	// caracter de finalização de comando = "ponto".

#endif

//***********************************************************************




//***********************************************************************
//
//=======================================================================

#define	CMD_no_reply	0	// sem resposta aos comandos recebidos.

#define	CMD_full_reply	1	// resposta "completa" aos comandos recebidos.

#define	CMD_code_reply	2	// resposta "codificada" aos comandos recebidos.

//-----------------------------------------------------------------------

//BYTE	CMD_reply = CMD_no_reply;	// indica o tipo de resposta aos
BYTE	CMD_reply = CMD_full_reply;	// indica o tipo de resposta aos
					// comandos recebidos.
//***********************************************************************




//***********************************************************************
//
//=======================================================================

typedef	bool (*CMD_func_PTR) ( BYTE_PTR CMD_data, BYTE index );

//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	DUMMY_CMD ( BYTE_PTR CMD_data, BYTE index )
{
	return (true);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

#define	CMD_size 10	// máximo de caracteres em um comando.

BYTE	CMD_BUFF [CMD_size];	// Buffer de Comandos recebidos.

BYTE	CMD_BUFF_idx;	// posição atual no Buffer de Comandos.

//-----------------------------------------------------------------------

CMD_func_PTR SYS_CMD_PTR;	// Ponteiro para o Comando a ser executado.

//***********************************************************************




//***********************************************************************
//
//=======================================================================

struct	CMD_LOC_info
{
	char CMD_code;

	CMD_func_PTR	CMD_func;
};

//-----------------------------------------------------------------------

typedef	struct	CMD_LOC_info	CMD_LOC_type;

//-----------------------------------------------------------------------

#define	CMD_LIST_end_mark	1	// marcador do fim de uma lista
					// de comandos.
//***********************************************************************




//***********************************************************************
//
//=======================================================================

BYTE	LOW_CASE_conv ( BYTE ASCII_char )
{
	if ( ( ASCII_char >= 'A' ) && ( ASCII_char <= 'Z' ) )
	{
		ASCII_char += 0x20;
	}

	return (ASCII_char);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	CMD_BUFF_reset ()
{
//--------------------------------------------------
BYTE i;
//--------------------------------------------------

	CMD_BUFF_idx = 0;

	for ( i=0; i< CMD_size; i++ )
	{
		CMD_BUFF [i] = 0;
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	CMD_BUFF_ins ( BYTE new_char )
{
	if ( CMD_BUFF_idx < CMD_size )
	{
		CMD_BUFF [CMD_BUFF_idx] = LOW_CASE_conv (new_char);

		CMD_BUFF_idx++;
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	COMMAND_get ()
{
//--------------------------------------------------
bool CMD_sts;
BYTE rcv_char;
//--------------------------------------------------

	CMD_sts = false;

	if ( Serial.available() > 0 )
	{
		rcv_char = Serial.read();

		if ( rcv_char == CMD_end )
		{
			CMD_sts = true;
		}
		else
		{
			CMD_BUFF_ins (rcv_char);
		}
	}

	return (CMD_sts);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	COMMAND_decode ( struct CMD_LOC_info *CMD_list_PTR )
{
//--------------------------------------------------
char CMD_code, LOC_code;
BYTE CMD_idx;
bool locate;
//--------------------------------------------------

	CMD_code = CMD_BUFF [0];

	locate = false;

	CMD_idx = 0;

	do
	{
		LOC_code = ROM_get8( &CMD_list_PTR [CMD_idx].CMD_code );

		if ( LOC_code == CMD_code )
		{
			SYS_CMD_PTR = (CMD_func_PTR) ROM_PTR_get( &CMD_list_PTR [CMD_idx].CMD_func );

			locate = true;
		}
		else
		{
			CMD_idx++;
		}

	} while ( !locate && ( LOC_code != CMD_LIST_end_mark ) );

	return (locate);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	SERIAL_newline_send ( BYTE n )
{
	for ( ; n > 0; n-- ) Serial.println ();
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	COMMAND_FAIL_reply ()
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_LF_print ("comando ignorado!!!");
		SERIAL_newline_send (1);
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_LF_print ("#0");
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	COMMAND_OK_reply ()
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_LF_print ("ok");
		SERIAL_newline_send (1);
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_LF_print ("#1");
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	COMMAND_exec ()
{
	if ( SYS_CMD_PTR ( CMD_BUFF, 1 ) )	// executa o comando.

		COMMAND_OK_reply ();		// réplica para sucesso.

	else	COMMAND_FAIL_reply ();		// réplica para erro.
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	COMMAND_proc ( struct CMD_LOC_info *CMD_list_PTR )
{
	if ( COMMAND_get () )
	{
		if ( COMMAND_decode ( CMD_list_PTR ) )

			COMMAND_exec ();

		else	COMMAND_FAIL_reply ();

		CMD_BUFF_reset ();
	}
}
//***********************************************************************




//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//
//
//
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\




//***********************************************************************
//
//=======================================================================

bool	ASCII_to_BCD_8 ( BYTE *code )
{
//--------------------------------------------------
bool is_BCD = false;
//--------------------------------------------------

	if ( ( *code >= 0x30 ) && ( *code <= 0x39 ) )
	{
		*code = *code - 0x30;

		is_BCD = true;
	}

	return (is_BCD);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	BCD_8_NEXT_get ( BYTE_PTR CMD_data, BYTE *index, BYTE *BCD_8 )
{
//--------------------------------------------------
BYTE VAL_1, VAL_2;
bool is_BCD = false;
//--------------------------------------------------

	VAL_1 = CMD_data [*index];

	if ( ASCII_to_BCD_8 (&VAL_1) )
	{
		is_BCD = true;

		(*index)++;

		VAL_2 = CMD_data [*index];

		if ( ASCII_to_BCD_8 (&VAL_2) )
		{
			(*index)++;

			VAL_1 = ( VAL_1 << 4 ) + VAL_2;
		}

		*BCD_8 = VAL_1;
	}

	return (is_BCD);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	END_CMD_CHK ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
BYTE VAL_1;
bool OK = false;
//--------------------------------------------------

	VAL_1 = CMD_data [index];

	if ( ( VAL_1 == 0 ) || ( VAL_1 == 0x0D ) )
	{
		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

#define QUARTER_MIN	1

#define QUARTER_MAX	4

//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	QUARTER_value_CHK ( BYTE value )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( ( value >= QUARTER_MIN ) && ( value <= QUARTER_MAX ) )
	{
		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	QUARTER_inform ( BYTE value )
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_print ("GAME QUARTER = ");
		Serial.println ( value, DEC );
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_print ("#q");
		Serial.println ( value, DEC );
	}
}
//***********************************************************************




//***********************************************************************
//	Comando "qX": especifica o "QUARTER" atual.
//=======================================================================

bool	qX_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
BYTE VAL_1;
bool OK = false;
//--------------------------------------------------

	if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
	{
		if ( END_CMD_CHK ( CMD_data, index ) )
		{
			VAL_1 =	BCD8_to_HEX8 ( VAL_1 );

			if ( QUARTER_value_CHK ( VAL_1 ) )
			{
				GAME_quarter = VAL_1;

				QUARTER_inform ( GAME_quarter );

				OK = true;
			}
		}
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	GUEST_VANTAGE_inform ()
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_LF_print ("GUEST vantage!");
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_LF_print ("#lv");
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	HOME_VANTAGE_inform ()
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_LF_print ("HOME vantage!");
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_LF_print ("#lc");
	}
}
//***********************************************************************




//***********************************************************************
//	Comando "lt": troca a Vantagem atual.
//=======================================================================

bool	lt_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( END_CMD_CHK ( CMD_data, index ) )
	{
		if ( GAME_VANTAGE == GUEST_VANTAGE )
		{
			GAME_VANTAGE = HOME_VANTAGE;

			HOME_VANTAGE_inform ();
		}
		else
		{
			GAME_VANTAGE = GUEST_VANTAGE;

			GUEST_VANTAGE_inform ();
		}

		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//	Comando "lv": seleciona a "Vantagem visitante".
//=======================================================================

bool	lv_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( END_CMD_CHK ( CMD_data, index ) )
	{
		GAME_VANTAGE = GUEST_VANTAGE;

		GUEST_VANTAGE_inform ();

		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//	Comando "lc": seleciona a "Vantagem casa".
//=======================================================================

bool	lc_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( END_CMD_CHK ( CMD_data, index ) )
	{
		GAME_VANTAGE = HOME_VANTAGE;

		HOME_VANTAGE_inform ();

		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

#define Timeout_MIN	0

#define Timeout_MAX	3

//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	Timeout_value_CHK ( BYTE value )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( ( value >= Timeout_MIN ) && ( value <= Timeout_MAX ) )
	{
		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	TIMEOUT_guest_inform ( BYTE value )
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_print ("TIMEOUT GUEST = ");
		Serial.println ( value, DEC );
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_print ("#tv");
		Serial.println ( value, DEC );
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	TIMEOUT_home_inform ( BYTE value )
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_print ("TIMEOUT HOME = ");
		Serial.println ( value, DEC );
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_print ("#tc");
		Serial.println ( value, DEC );
	}
}
//***********************************************************************




//***********************************************************************
//	Comando "tc-": decrementa o "TIMEOUT visitante".
//=======================================================================

bool	tv_DECR_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( END_CMD_CHK ( CMD_data, index ) )
	{
		if ( TMOUT_guest != 0 )
		{
			TMOUT_guest--;

			TIMEOUT_guest_inform ( TMOUT_guest );

			OK = true;
		}
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//	Comando "tc-": decrementa o "TIMEOUT casa".
//=======================================================================

bool	tc_DECR_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( END_CMD_CHK ( CMD_data, index ) )
	{
		if ( TMOUT_home != 0 )
		{
			TMOUT_home--;

			TIMEOUT_home_inform ( TMOUT_home );

			OK = true;
		}
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//	Comando "tvXX": especifica o "TIMEOUT visitante".
//=======================================================================

bool	tvX_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
BYTE VAL_1;
bool OK = false;
//--------------------------------------------------

	if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
	{
		if ( END_CMD_CHK ( CMD_data, index ) )
		{
			VAL_1 =	BCD8_to_HEX8 ( VAL_1 );

			if ( Timeout_value_CHK ( VAL_1 ) )
			{
				TMOUT_guest = VAL_1;

				TIMEOUT_guest_inform ( TMOUT_guest );

				OK = true;
			}
		}
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//	Comando "tcXX": especifica o "TIMEOUT casa".
//=======================================================================

bool	tcX_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
BYTE VAL_1;
bool OK = false;
//--------------------------------------------------

	if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
	{
		if ( END_CMD_CHK ( CMD_data, index ) )
		{
			VAL_1 =	BCD8_to_HEX8 ( VAL_1 );

			if ( Timeout_value_CHK ( VAL_1 ) )
			{
				TMOUT_home = VAL_1;

				TIMEOUT_home_inform ( TMOUT_home );

				OK = true;
			}
		}
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

#define GAME_SCORE_MIN	0

#define GAME_SCORE_MAX	99

//-----------------------------------------------------------------------

#define SCORE_delta_MIN	0

#define SCORE_delta_MAX	10

//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	SCORE_delta_value_CHK ( BYTE value )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( ( value >= SCORE_delta_MIN ) && ( value <= SCORE_delta_MAX ) )
	{
		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	GAME_SCORE_value_CHK ( int value )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( ( value >= GAME_SCORE_MIN ) && ( value <= GAME_SCORE_MAX ) )
	{
		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	SCORE_guest_DECR_inform ( BYTE delta, BYTE score )
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_print ("SCORE GUEST decremented by ");
		Serial.println ( delta, DEC );
		ROM_SERIAL_print ("new SCORE GUEST = ");
		Serial.println ( score, DEC );
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_print ("#v");
		Serial.println ( score, DEC );
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	SCORE_guest_INCR_inform ( BYTE delta, BYTE score )
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_print ("SCORE GUEST incremented by ");
		Serial.println ( delta, DEC );
		ROM_SERIAL_print ("new SCORE GUEST = ");
		Serial.println ( score, DEC );
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_print ("#v");
		Serial.println ( score, DEC );
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	SCORE_guest_inform ( BYTE score )
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_print ("SCORE GUEST = ");
		Serial.println ( score, DEC );
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_print ("#v");
		Serial.println ( score, DEC );
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	SCORE_home_DECR_inform ( BYTE delta, BYTE score )
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_print ("SCORE HOME decremented by ");
		Serial.println ( delta, DEC );
		ROM_SERIAL_print ("new SCORE HOME = ");
		Serial.println ( score, DEC );
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_print ("#c");
		Serial.println ( score, DEC );
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	SCORE_home_INCR_inform ( BYTE delta, BYTE score )
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_print ("SCORE HOME incremented by ");
		Serial.println ( delta, DEC );
		ROM_SERIAL_print ("new SCORE HOME = ");
		Serial.println ( score, DEC );
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_print ("#c");
		Serial.println ( score, DEC );
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	SCORE_home_inform ( BYTE score )
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_print ("SCORE HOME = ");
		Serial.println ( score, DEC );
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_print ("#c");
		Serial.println ( score, DEC );
	}
}
//***********************************************************************




//***********************************************************************
//	Comando "v-XX": decrementa o "Placar visitante" em "XX" unidades.
//=======================================================================

bool	vXX_DECR_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
BYTE delta;
int new_score;
bool OK = false;
//--------------------------------------------------

	if ( BCD_8_NEXT_get ( CMD_data, &index, &delta ) )
	{
		if ( END_CMD_CHK ( CMD_data, index ) )
		{
			delta =	BCD8_to_HEX8 ( delta );

			if ( SCORE_delta_value_CHK ( delta ) )
			{
				new_score = SCORE_guest - delta;

				if ( GAME_SCORE_value_CHK ( new_score ) )
				{
					SCORE_guest = new_score;

					SCORE_guest_DECR_inform ( delta, SCORE_guest );

					OK = true;
				}
			}
		}
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//	Comando "v+XX": incrementa o "Placar visitante" em "XX" unidades.
//=======================================================================

bool	vXX_INCR_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
BYTE delta;
int new_score;
bool OK = false;
//--------------------------------------------------

	if ( BCD_8_NEXT_get ( CMD_data, &index, &delta ) )
	{
		if ( END_CMD_CHK ( CMD_data, index ) )
		{
			delta =	BCD8_to_HEX8 ( delta );

			if ( SCORE_delta_value_CHK ( delta ) )
			{
				new_score = SCORE_guest + delta;

				if ( GAME_SCORE_value_CHK ( new_score ) )
				{
					SCORE_guest = new_score;

					SCORE_guest_INCR_inform ( delta, SCORE_guest );

					OK = true;
				}
			}
		}
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//	Comando "vXX": especifica o "Placar visitante".
//=======================================================================

bool	vXX_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
BYTE VAL_1;
bool OK = false;
//--------------------------------------------------

	if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
	{
		if ( END_CMD_CHK ( CMD_data, index ) )
		{
			VAL_1 =	BCD8_to_HEX8 ( VAL_1 );

			if ( GAME_SCORE_value_CHK ( VAL_1 ) )
			{
				SCORE_guest = VAL_1;

				SCORE_guest_inform ( SCORE_guest );

				OK = true;
			}
		}
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//	Comando "c-XX": decrementa o "Placar casa" em "XX" unidades.
//=======================================================================

bool	cXX_DECR_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
BYTE delta;
int new_score;
bool OK = false;
//--------------------------------------------------

	if ( BCD_8_NEXT_get ( CMD_data, &index, &delta ) )
	{
		if ( END_CMD_CHK ( CMD_data, index ) )
		{
			delta =	BCD8_to_HEX8 ( delta );

			if ( SCORE_delta_value_CHK ( delta ) )
			{
				new_score = SCORE_home - delta;

				if ( GAME_SCORE_value_CHK ( new_score ) )
				{
					SCORE_home = new_score;

					SCORE_home_DECR_inform ( delta, SCORE_home );

					OK = true;
				}
			}
		}
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//	Comando "c+XX": incrementa o "Placar casa" em "XX" unidades.
//=======================================================================

bool	cXX_INCR_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
BYTE delta;
int new_score;
bool OK = false;
//--------------------------------------------------

	if ( BCD_8_NEXT_get ( CMD_data, &index, &delta ) )
	{
		if ( END_CMD_CHK ( CMD_data, index ) )
		{
			delta =	BCD8_to_HEX8 ( delta );

			if ( SCORE_delta_value_CHK ( delta ) )
			{
				new_score = SCORE_home + delta;

				if ( GAME_SCORE_value_CHK ( new_score ) )
				{
					SCORE_home = new_score;

					SCORE_home_INCR_inform ( delta, SCORE_home );

					OK = true;
				}
			}
		}
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//	Comando "cXX": especifica o "Placar casa".
//=======================================================================

bool	cXX_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
BYTE VAL_1;
bool OK = false;
//--------------------------------------------------

	if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
	{
		if ( END_CMD_CHK ( CMD_data, index ) )
		{
			VAL_1 =	BCD8_to_HEX8 ( VAL_1 );

			if ( GAME_SCORE_value_CHK ( VAL_1 ) )
			{
				SCORE_home = VAL_1;

				SCORE_home_inform ( SCORE_home );

				OK = true;
			}
		}
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

#define Game_Time_MAX_minutos	15

#define Game_Time_MAX_seg	60*Game_Time_MAX_minutos

//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	Game_Time_value_CHK ( BYTE min, BYTE seg )
{
//--------------------------------------------------
WORD	seg_TOTAL;
//--------------------------------------------------

	seg_TOTAL = ( 60*min + seg );

	return ( seg_TOTAL <= Game_Time_MAX_seg );
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	GAME_TIME_cadence_inform ()
{
	if ( CMD_reply == CMD_code_reply )	// se modo resposta "codificada":
	{
		ROM_SERIAL_print ("#gm");
		Serial.println ( GAME_TIME_min, DEC );
		ROM_SERIAL_print ("#gs");
		Serial.println ( GAME_TIME_seg, DEC );

		if ( ( GAME_TIME_seg == 0 ) && ( GAME_TIME_min == 0 ) )
		{
			ROM_SERIAL_LF_print ("#gp");
		}
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	GAME_TIME_inform ( BYTE min, BYTE seg )
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_print ("Game Time = ");

		if ( min < 10 ) ROM_SERIAL_print ("0");
		Serial.print ( min, DEC );

		ROM_SERIAL_print (":");

		if ( seg < 10 ) ROM_SERIAL_print ("0");
		Serial.println ( seg, DEC );
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_print ("#gm");
		Serial.println ( min, DEC );
		ROM_SERIAL_print ("#gs");
		Serial.println ( seg, DEC );
	}
}
//***********************************************************************




//***********************************************************************
//	Comando "gXX:YY": especifica o "Game Time" atual.
//=======================================================================

bool	gXX_YY_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
BYTE VAL_1, VAL_2;
bool OK = false;
//--------------------------------------------------

	if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
	{
		if ( CMD_data[index] == ':' )
		{
			index++;

			VAL_1 =	BCD8_to_HEX8 ( VAL_1 );

			OK = true;
		}
	}
//..................................................

	if ( OK && BCD_8_NEXT_get ( CMD_data, &index, &VAL_2 ) )
	{
		OK = false;

		if ( END_CMD_CHK ( CMD_data, index ) )
		{
			VAL_2 =	BCD8_to_HEX8 ( VAL_2 );

			OK = Game_Time_value_CHK ( VAL_1, VAL_2 );
		}
	}
	else	OK = false;
//..................................................

	if ( OK )
	{
		GAME_TIME_min = VAL_1;
		GAME_TIME_seg = VAL_2;

		GAME_TIME_inform ( GAME_TIME_min, GAME_TIME_seg );
	}
//..................................................

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	GAME_TIME_stop_inform ()
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_LF_print ("Game Time stopped !!!");
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_LF_print ("#gp");
	}
}
//***********************************************************************




//***********************************************************************
//	Comando "gp": pára a contagem do "Game Time".
//=======================================================================

bool	gp_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( END_CMD_CHK ( CMD_data, index ) )
	{
		GAME_TIME_stop ();

		GAME_TIME_stop_inform ();

		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	GAME_TIME_run_inform ()
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_LF_print ("Game Time running !!!");
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_LF_print ("#gi");
	}
}
//***********************************************************************




//***********************************************************************
//	Comando "gi": inicia a contagem do "Game Time".
//=======================================================================

bool	gi_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( END_CMD_CHK ( CMD_data, index ) )
	{
		if ( ( GAME_TIME_seg != 0 ) || ( GAME_TIME_min != 0 ) )
		{
			GAME_TIME_run ();

			GAME_TIME_run_inform ();
		}

		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

#define PLAY_CLOCK_MIN	0

#define PLAY_CLOCK_MAX	40

//-----------------------------------------------------------------------

#define PCLOCK_delta_MIN	0

#define PCLOCK_delta_MAX	20

//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	PCLOCK_delta_value_CHK ( BYTE value )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( ( value >= PCLOCK_delta_MIN ) && ( value <= PCLOCK_delta_MAX ) )
	{
		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	PLAY_CLOCK_value_CHK ( int value )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( ( value >= PLAY_CLOCK_MIN ) && ( value <= PLAY_CLOCK_MAX ) )
	{
		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLAY_CLOCK_cadence_inform ()
{
	if ( CMD_reply == CMD_code_reply )	// se modo resposta "codificada":
	{
		ROM_SERIAL_print ("#p");
		Serial.println ( PLAY_CLOCK, DEC );

		if ( PLAY_CLOCK == 0 )	ROM_SERIAL_LF_print ("#pp");
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLAY_CLOCK_swap_OFF_inform ()
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_LF_print ("PLAY CLOCK on BALL = OFF");
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_LF_print ("#pb0");
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLAY_CLOCK_swap_ON_inform ()
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_LF_print ("PLAY CLOCK on BALL = ON");
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_LF_print ("#pb1");
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLAY_CLOCK_swap_inform ( bool swap )
{
	if ( swap )

		PLAY_CLOCK_swap_ON_inform ();

	else	PLAY_CLOCK_swap_OFF_inform ();
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLAY_CLOCK_DECR_inform ( BYTE delta, BYTE value )
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_print ("PLAY CLOCK decremented by ");
		Serial.println ( delta, DEC );
		ROM_SERIAL_print ("new PLAY CLOCK = ");
		Serial.println ( value, DEC );
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_print ("#p");
		Serial.println ( value, DEC );
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLAY_CLOCK_INCR_inform ( BYTE delta, BYTE value )
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_print ("PLAY CLOCK incremented by ");
		Serial.println ( delta, DEC );
		ROM_SERIAL_print ("new PLAY CLOCK = ");
		Serial.println ( value, DEC );
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_print ("#p");
		Serial.println ( value, DEC );
	}
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLAY_CLOCK_inform ( BYTE value )
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_print ("PLAY CLOCK = ");
		Serial.println ( value, DEC );
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_print ("#p");
		Serial.println ( value, DEC );
	}
}
//***********************************************************************




//***********************************************************************
//	Comando "p-XX": decrementa o "Play Clock" em "XX" unidades.
//=======================================================================

bool	pXX_DECR_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
BYTE delta;
int new_PCLOCK;
bool OK = false;
//--------------------------------------------------

	if ( BCD_8_NEXT_get ( CMD_data, &index, &delta ) )
	{
		if ( END_CMD_CHK ( CMD_data, index ) )
		{
			delta =	BCD8_to_HEX8 ( delta );

			if ( PCLOCK_delta_value_CHK ( delta ) )
			{
				new_PCLOCK = PLAY_CLOCK - delta;

				if ( PLAY_CLOCK_value_CHK ( new_PCLOCK ) )
				{
					PLAY_CLOCK = new_PCLOCK;

					PLAY_CLOCK_DECR_inform ( delta, PLAY_CLOCK );

					OK = true;
				}
			}
		}
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//	Comando "p+XX": incrementa o "Play Clock" em "XX" unidades.
//=======================================================================

bool	pXX_INCR_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
BYTE delta;
int new_PCLOCK;
bool OK = false;
//--------------------------------------------------

	if ( BCD_8_NEXT_get ( CMD_data, &index, &delta ) )
	{
		if ( END_CMD_CHK ( CMD_data, index ) )
		{
			delta =	BCD8_to_HEX8 ( delta );

			if ( PCLOCK_delta_value_CHK ( delta ) )
			{
				new_PCLOCK = PLAY_CLOCK + delta;

				if ( PLAY_CLOCK_value_CHK ( new_PCLOCK ) )
				{
					PLAY_CLOCK = new_PCLOCK;

					PLAY_CLOCK_INCR_inform ( delta, PLAY_CLOCK );

					OK = true;
				}
			}
		}
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//	Comando "pXX": especifica o "Play Clock".
//=======================================================================

bool	pXX_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
BYTE VAL_1;
bool OK = false;
//--------------------------------------------------

	if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
	{
		if ( END_CMD_CHK ( CMD_data, index ) )
		{
			VAL_1 =	BCD8_to_HEX8 ( VAL_1 );

			if ( PLAY_CLOCK_value_CHK ( VAL_1 ) )
			{
				PLAY_CLOCK = VAL_1;

				PLAY_CLOCK_inform ( PLAY_CLOCK );

				OK = true;
			}
		}
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//	Comando "p": seta o "Play Clock" em "40".
//=======================================================================

bool	p_CMD ()
{
	PLAY_CLOCK = 40;

	PLAY_CLOCK_inform ( PLAY_CLOCK );

	return (true);
}
//***********************************************************************




//***********************************************************************
//	Comando "pb": toggle modo "swap" entre "Play Clock" e "Ball".
//=======================================================================

bool	pb_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( END_CMD_CHK ( CMD_data, index ) )
	{
		PLAY_CLOCK_swap = !PLAY_CLOCK_swap;

		PLAY_CLOCK_swap_inform ( PLAY_CLOCK_swap );

		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLAY_CLOCK_stop_inform ()
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_LF_print ("PLAY CLOCK stopped !!!");
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_LF_print ("#pp");
	}
}
//***********************************************************************




//***********************************************************************
//	Comando "pp": pára a contagem do "Play Clock".
//=======================================================================

bool	pp_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( END_CMD_CHK ( CMD_data, index ) )
	{
		PLAY_CLOCK_stop ();

		PLAY_CLOCK_stop_inform ();

		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	PLAY_CLOCK_run_inform ()
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_LF_print ("PLAY CLOCK running !!!");
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_LF_print ("#pi");
	}
}
//***********************************************************************




//***********************************************************************
//	Comando "pi": inicia a contagem do "Play Clock".
//=======================================================================

bool	pi_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( END_CMD_CHK ( CMD_data, index ) )
	{
		if ( PLAY_CLOCK != 0 )
		{
			PLAY_CLOCK_run ();

			PLAY_CLOCK_run_inform ();
		}

		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

#define BALL_pos_MIN	0

#define BALL_pos_MAX	50

//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	BALL_POS_value_CHK ( BYTE value )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( ( value >= BALL_pos_MIN ) && ( value <= BALL_pos_MAX ) )
	{
		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	BALL_POSITION_inform ( BYTE position )
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_print ("BALL on: ");
		Serial.println ( position, DEC );
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_print ("#bb");
		Serial.println ( position, DEC );
	}
}
//***********************************************************************




//***********************************************************************
//	Comando "bbXX": especifica a posição da bola.
//=======================================================================

bool	bbXX_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
BYTE VAL_1;
bool OK = false;
//--------------------------------------------------

	if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
	{
		if ( END_CMD_CHK ( CMD_data, index ) )
		{
			VAL_1 =	BCD8_to_HEX8 ( VAL_1 );

			if ( BALL_POS_value_CHK ( VAL_1 ) )
			{
				BALL_position = VAL_1;

				BALL_POSITION_inform ( BALL_position );

				OK = true;
			}
		}
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

#define to_GO_MIN	0

#define to_GO_MAX	99

//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	to_GO_value_CHK ( BYTE value )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( ( value >= to_GO_MIN ) && ( value <= to_GO_MAX ) )
	{
		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	to_GO_inform ( BYTE to_GO )
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_print ("TO GO = ");
		Serial.println ( to_GO, DEC );
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_print ("#bg");
		Serial.println ( to_GO, DEC );
	}
}
//***********************************************************************




//***********************************************************************
//	Comando "bgXX": especifica o "to GO" atual.
//=======================================================================

bool	bgXX_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
BYTE VAL_1;
bool OK = false;
//--------------------------------------------------

	if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
	{
		if ( END_CMD_CHK ( CMD_data, index ) )
		{
			VAL_1 =	BCD8_to_HEX8 ( VAL_1 );

			if ( to_GO_value_CHK ( VAL_1 ) )
			{
				BALL_to_go = VAL_1;

				to_GO_inform ( BALL_to_go );

				OK = true;
			}
		}
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

#define down_MIN	1

#define down_MAX	4

//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	DOWN_value_CHK ( BYTE value )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( ( value >= down_MIN ) && ( value <= down_MAX ) )
	{
		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	BALL_down_inform ( BYTE down )
{
	if ( CMD_reply == CMD_full_reply )	// modo resposta "completa":
	{
		ROM_SERIAL_print ("DOWN = ");
		Serial.println ( down, DEC );
	}
	else if ( CMD_reply == CMD_code_reply )	// modo resposta "codificada":
	{
		ROM_SERIAL_print ("#bd");
		Serial.println ( down, DEC );
	}
}
//***********************************************************************




//***********************************************************************
//	Comando "bdX": especifica o "down" atual.
//=======================================================================

bool	bdX_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
BYTE VAL_1;
bool OK = false;
//--------------------------------------------------

	if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
	{
		if ( END_CMD_CHK ( CMD_data, index ) )
		{
			VAL_1 =	BCD8_to_HEX8 ( VAL_1 );

			if ( DOWN_value_CHK ( VAL_1 ) )
			{
				BALL_down = VAL_1;

				BALL_down_inform ( BALL_down );

				OK = true;
			}
		}
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//	Comando "bXX,Y,ZZ": especifica a posição da bola, o "down",
//	e "to GO".
//=======================================================================

bool	bXX_Y_ZZ_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
BYTE VAL_1, VAL_2, VAL_3;
bool OK = false;
//--------------------------------------------------

	if ( BCD_8_NEXT_get ( CMD_data, &index, &VAL_1 ) )
	{
		if ( CMD_data[index] == ',' )
		{
			index++;

			VAL_1 =	BCD8_to_HEX8 ( VAL_1 );

			OK = BALL_POS_value_CHK ( VAL_1 );
		}
	}
//..................................................

	if ( OK && BCD_8_NEXT_get ( CMD_data, &index, &VAL_2 ) )
	{
		OK = false;

		if ( CMD_data[index] == ',' )
		{
			index++;

			VAL_2 =	BCD8_to_HEX8 ( VAL_2 );

			OK = DOWN_value_CHK ( VAL_2 );
		}
	}
	else	OK = false;
//..................................................

	if ( OK && BCD_8_NEXT_get ( CMD_data, &index, &VAL_3 ) )
	{
		OK = false;

		if ( END_CMD_CHK ( CMD_data, index ) )
		{
			VAL_3 =	BCD8_to_HEX8 ( VAL_3 );

			OK = to_GO_value_CHK ( VAL_3 );
		}
	}
	else	OK = false;
//..................................................

	if ( OK )
	{
		BALL_position = VAL_1;
		BALL_POSITION_inform ( BALL_position );

		BALL_down = VAL_2;
		BALL_down_inform ( BALL_down );

		BALL_to_go = VAL_3;
		to_GO_inform ( BALL_to_go );
	}
//..................................................

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	P_MAIN_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	switch ( CMD_data [index] )
	{
//..................................................
		case	'i':

			index++;

			OK = pi_CMD ( CMD_data, index );

		break;
//..................................................

		case	'p':

			index++;

			OK = pp_CMD ( CMD_data, index );

		break;
//..................................................
		case	'+':

			index++;

			OK = pXX_INCR_CMD ( CMD_data, index );

		break;
//..................................................

		case	'-':

			index++;

			OK = pXX_DECR_CMD ( CMD_data, index );

		break;
//..................................................

		case	'b':

			index++;

			OK = pb_CMD ( CMD_data, index );

		break;
//..................................................

		default:
			if ( END_CMD_CHK ( CMD_data, index ) )
			{
				OK = p_CMD ();
			}
			else	OK = pXX_CMD ( CMD_data, index );
//..................................................
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	G_MAIN_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	switch ( CMD_data [index] )
	{
//..................................................
		case	'i':

			index++;

			OK = gi_CMD ( CMD_data, index );

		break;
//..................................................

		case	'p':

			index++;

			OK = gp_CMD ( CMD_data, index );

		break;
//..................................................

		default:
			OK = gXX_YY_CMD ( CMD_data, index );
//..................................................
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	Q_MAIN_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	switch ( CMD_data [index] )
	{
//..................................................
		case	'*':

			index++;

//			OK = yyy_CMD ( CMD_data, index );

		break;
//..................................................

		case	'/':

			index++;

//			OK = yyy_CMD ( CMD_data, index );

		break;
//..................................................

		default:
			OK = qX_CMD ( CMD_data, index );
//..................................................
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	V_MAIN_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	switch ( CMD_data [index] )
	{
//..................................................
		case	'+':

			index++;

			OK = vXX_INCR_CMD ( CMD_data, index );

		break;
//..................................................

		case	'-':

			index++;

			OK = vXX_DECR_CMD ( CMD_data, index );

		break;
//..................................................

		default:
			OK = vXX_CMD ( CMD_data, index );
//..................................................
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	C_MAIN_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	switch ( CMD_data [index] )
	{
//..................................................
		case	'+':

			index++;

			OK = cXX_INCR_CMD ( CMD_data, index );

		break;
//..................................................

		case	'-':

			index++;

			OK = cXX_DECR_CMD ( CMD_data, index );

		break;
//..................................................

		default:
			OK = cXX_CMD ( CMD_data, index );
//..................................................
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	L_MAIN_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	switch ( CMD_data [index] )
	{
//..................................................
		case	'c':

			index++;

			OK = lc_CMD ( CMD_data, index );

		break;
//..................................................

		case	'v':

			index++;

			OK = lv_CMD ( CMD_data, index );

		break;
//..................................................

		case	't':

			index++;

			OK = lt_CMD ( CMD_data, index );

		break;
//..................................................

		default:
			OK = false;
//..................................................
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	T_MAIN_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	switch ( CMD_data [index] )
	{
//..................................................
		case	'c':

			index++;

			if ( CMD_data [index] == '-')
			{
				index++;

				OK = tc_DECR_CMD ( CMD_data, index );
			}
			else	OK = tcX_CMD ( CMD_data, index );

		break;
//..................................................

		case	'v':

			index++;

			if ( CMD_data [index] == '-')
			{
				index++;

				OK = tv_DECR_CMD ( CMD_data, index );
			}
			else	OK = tvX_CMD ( CMD_data, index );

		break;
//..................................................

		default:
			OK = false;
//..................................................
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	B_MAIN_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	switch ( CMD_data [index] )
	{
//..................................................
		case	'b':

			index++;

			OK = bbXX_CMD ( CMD_data, index );

		break;
//..................................................

		case	'd':

			index++;

			OK = bdX_CMD ( CMD_data, index );

		break;
//..................................................

		case	'g':

			index++;

			OK = bgXX_CMD ( CMD_data, index );

		break;
//..................................................

		default:
			OK = bXX_Y_ZZ_CMD ( CMD_data, index );
//..................................................
	}

	return (OK);
}
//***********************************************************************




//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//
//
//
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\




//***********************************************************************
//	Comando "@0": desliga o Display.
//=======================================================================

bool	Display_OFF_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( END_CMD_CHK ( CMD_data, index ) )
	{
		DISPLAY_view_SET ( VIEW_OFF );

		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//	Comando "@1": liga o Display.
//=======================================================================

bool	Display_ON_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( END_CMD_CHK ( CMD_data, index ) )
	{
		DISPLAY_view_SET ( VIEW_ON );

		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	Display_ON_OFF_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	switch ( CMD_data [index] )
	{
//..................................................
		case	'0':	// comando para desligar o Display.

			index++;

			OK = Display_OFF_CMD ( CMD_data, index );

		break;
//..................................................

		case	'1':	// comando para ligar o Display.

			index++;

			OK = Display_ON_CMD ( CMD_data, index );

		break;
//..................................................

		default:
			OK = false;
//..................................................
	}

	return (OK);
}
//***********************************************************************




//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//
//
//
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\




//***********************************************************************
//	Comando "$c": seleciona resposta "codificada" aos comandos
//	recebidos.
//=======================================================================

bool	code_reply_SET_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( END_CMD_CHK ( CMD_data, index ) )
	{
		CMD_reply = CMD_code_reply;

		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//	Comando "$f": seleciona resposta "completa" aos comandos
//	recebidos.
//=======================================================================

bool	full_reply_SET_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( END_CMD_CHK ( CMD_data, index ) )
	{
		CMD_reply = CMD_full_reply;

		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//	Comando "$n": seleciona sem resposta aos comandos recebidos.
//=======================================================================

bool	no_reply_SET_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	if ( END_CMD_CHK ( CMD_data, index ) )
	{
		CMD_reply = CMD_no_reply;

		OK = true;
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

bool	REPLY_mode_CMD ( BYTE_PTR CMD_data, BYTE index )
{
//--------------------------------------------------
bool OK = false;
//--------------------------------------------------

	switch ( CMD_data [index] )
	{
//..................................................
		case	'n':	// especifica "sem resposta" aos comandos recebidos.

			index++;

			OK = no_reply_SET_CMD ( CMD_data, index );

		break;
//..................................................

		case	'f':	// especifica resposta "completa" aos comandos recebidos.

			index++;

			OK = full_reply_SET_CMD ( CMD_data, index );

		break;
//..................................................

		case	'c':	// especifica resposta "codificada" aos comandos recebidos.

			index++;

			OK = code_reply_SET_CMD ( CMD_data, index );

		break;
//..................................................

		default:
			OK = false;
//..................................................
	}

	return (OK);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

ROM_set	CMD_LOC_type CMD_list [] =
{
	'b', B_MAIN_CMD,	// comando genérico para setar o "spot de bola".

	't', T_MAIN_CMD,	// comando genérico para setar o "timeout".

	'l', L_MAIN_CMD,	// comando genérico para setar a "vantagem".

	'c', C_MAIN_CMD,	// comando genérico para setar o "Home Score".

	'v', V_MAIN_CMD,	// comando genérico para setar o "Guest Score".

	'q', Q_MAIN_CMD,	// comando genérico para setar o "Game Quarter".

	'g', G_MAIN_CMD,	// comando genérico para setar o "Game Time".

	'p', P_MAIN_CMD,	// comando genérico para setar o "Play Clock".

// - - - - - - - - - - - - - - - -

	'@', Display_ON_OFF_CMD,	// comando genérico para ON/OFF do Display.

// - - - - - - - - - - - - - - - -

	'$', REPLY_mode_CMD,	// comando genérico para setar o modo de resposta.

//..................................................

	CMD_LIST_end_mark, DUMMY_CMD	// marcador do fim da lista.
};
//***********************************************************************




//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//
//
//
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\




//***********************************************************************
//
//=======================================================================

void	SYS_HARDWARE_init ()
{
//..................................................

#if	( Main_DISPLAY_ON == 1 )

	digitalWrite ( SDOUT_main, LOW );
	pinMode ( SDOUT_main, OUTPUT );

	digitalWrite ( SCLK_main, LOW );
	pinMode ( SCLK_main, OUTPUT );

	digitalWrite ( PCLK_main, HIGH );
	pinMode ( PCLK_main, OUTPUT );

#endif
//..................................................

#if	( Pilot_DISPLAY_ON == 1 )

	digitalWrite ( SDOUT_piloto, LOW );
	pinMode ( SDOUT_piloto, OUTPUT );

	digitalWrite ( SCLK_piloto, LOW );
	pinMode ( SCLK_piloto, OUTPUT );

	digitalWrite ( PCLK_piloto, LOW );
	pinMode ( PCLK_piloto, OUTPUT );

#endif
//..................................................
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	SYS_init ()
{
	SYS_HARDWARE_init ();

//..................................................

	SYS_100ms_CRONO_startup ();
	SYS_100ms_CRONO_run ();

//..................................................

	DISPLAY_view_SET ( VIEW_OFF );

	DISPLAY_refresh_startup ();
	DISPLAY_refresh_run ();

//..................................................

	Serial.begin (9600);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	SYS_cadence ()
{
	SYS_100ms_CRONO_cadence ();

	DISPLAY_refresh_cadence ();
	DISPLAY_refresh_make ();
}
//***********************************************************************




//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\
//
//
//
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\




//***********************************************************************
//
//=======================================================================

void	PLACAR_SETTING_init ()
{
	GAME_TIME_min = 12;	// seta os minutos do tempo de jogo (00..59).
	GAME_TIME_seg = 00;	// seta os segundos do tempo de jogo (00..59).

//..................................................

	BALL_position = 0;	// seta a posição atual da Bola (00..99).

	BALL_down = 1;		// seta o "down" da Bola (0..9).

	BALL_to_go = 00;	// seta o "to go" da Bola (00..99).

//..................................................

	SCORE_home = 00;	// seta a pontuação da "casa" (00..99).

	TMOUT_home = 3;		// seta o "timeout" da "casa" (0..3).

//-----------------------------------------------------------------------

	SCORE_guest = 00;	// seta a pontuação do "visitante" (00..99).

	TMOUT_guest = 3;	// seta o "timeout" do "visitante" (0..3).

//-----------------------------------------------------------------------

	GAME_VANTAGE = HOME_VANTAGE;	// seta a vantagem atual no jogo 
					// ( HOME_VANTAGE ou GUEST_VANTAGE ).
//-----------------------------------------------------------------------

	GAME_quarter = 1;	// seta o "quarter" do jogo (1, 2, 3, ou 4).

//-----------------------------------------------------------------------

	PLAY_CLOCK = 40;	// seta o "Play Clock" atual do jogo (00..99).
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	setup ()
{
//--------------------------------------------------
//--------------------------------------------------

	SYS_init ();		//	<==== setar Pullups ou OUTs p/ pinos não usados.
				//		(checar padrão de startup do Arduino)
//..................................................

	PLACAR_SETTING_init ();

//	DISPLAY_view_SET ( VIEW_ON );

	DISPLAY_refresh_force = true;
	DISPLAY_refresh_make ();

//..................................................

	PLAY_CLOCK_startup ();		// PLAY_CLOCK_cadence_inform ()
	PLAY_CLOCK_notify_ON ( PLAY_CLOCK_cadence_inform );

	GAME_TIME_startup ();
	GAME_TIME_notify_ON ( GAME_TIME_cadence_inform );

//..................................................

	ROM_SERIAL_LF_print ("iniciando Placar...");
	Serial.println ();
	Serial.println ();

	delay (10);
}
//***********************************************************************




//***********************************************************************
//
//=======================================================================

void	loop ()
{
//--------------------------------------------------
//--------------------------------------------------

	COMMAND_proc ( (CMD_LOC_type*) CMD_list );

//..................................................

	PLAY_CLOCK_cadence ();

	GAME_TIME_cadence ();

//..................................................

	SYS_cadence ();

//..................................................

	delay (10);
}
//***********************************************************************
