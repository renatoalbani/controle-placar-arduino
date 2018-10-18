# Controle para Placar Baseado no arduino

Controle de placar de futebol americano baseado em arduíno

## Instruções para compilação do projeto

### Pré-requisitos

Atualmente a compilação do projeto deve ser realizada utilizando-se
o arduíno IDE

### Customização de acordo com o modelo da placa arduíno

Algumas definições modificam-se de acordo com o modelo do arduíno alvo da gravação
do resultado da compilação. Foi definido a macro HW_Board para parametrizar a compilação, 
é possível parametrizar dois modelos (UNO_rev3, MEGA_2560) na biblioteca **main.h**:

Caso o alvo seja uma arduíno UNO rev3

```
#define HW_Board UNO_rev3
```

Caso o alvo seja uma arduíno MEGA 2560
```
#define HW_Board MEGA_2560
```

### Definições relacionadas a placa alvo da gravação

O projeto está definido para dois modos de construção de acordo com o arduíno alvo da gravação.
Algumas macros foram definidas para flexibilizar a parametrização da construção do projeto permitindo
a diferenciação em dois modos.

* Modo para transmissão de comandos RADIO_MODE == TX_RADIO
* Modo para recebimento de comandos RADIO_MODE == RX_RADIO

Estas definição pode ser feita alterando na biblioteca **main.h** a definição
da macro RADIO_MODE.

Para o modo de transmissão
```
#define RX_MODE TX_RADIO
```

Para o mode de recebimento
```
#define RX_MODE RX_RADIO
```

Caso o alvo da construção seja a placa para controle do placar (RX_RADIO), também
é necessário habilitar o recebimento de comandos por rádio visto que nesse modo a
placa também permite a leitura de comandos pela interface serial. Para isso,
na biblioteca **main.h** defina a macro RX_RADIO_MODE conforme abaixo:

```
#define RX_RADIO_MODE RX_RADIO_ENABLED
``` 

### Definições relacionadas a pinagem das placas

É importante antes de proceder a compilação verificar se as definições
dos pinos utilizados estão de acordo com a montagem física da placa.
A biblioteca **types.h** concentra a definição dos pinos de acordo com
o modelo arduíno alvo da gravação.

Exemplo:
Definição de pinagem relacionada ao interfaceamento serial de comando dos displays para 
uma arduíno UNO rev3
```
#if ( HW_Board == UNO_rev3 )

  #define SDOUT_main  5 // pino da placa para o sinal "SDOUT" do "Main".
  #define SCLK_main 6 // pino da placa para o sinal "SCLK" do "Main".
  #define PCLK_main 7 // pino da placa para o sinal "PCLK" do "Main".

#endif
```


