Programa em C desenvolvido para a placa BitDogLab com microcontrolador RP2040

Descrição do Projeto

Este projeto visa o desenvolvimento de um programa em C para ser executado na placa RP2040. O programa possui as seguintes funcionalidades:

Piscar LED RGB: O LED vermelho do LED RGB na placa  pisca continuamente 5 vezes por segundo.
Incremento e Decremento com Botões: O botão A incrementa um número exibido na matriz de LEDs a cada vez que for pressionado, enquanto o botão B decrementa esse número a cada vez que for pressionado.
Exibição de Números com LEDs WS2812: A exibição do número na matriz de LEDs foi feita usando LEDs WS2812, com um efeito visual correspondente aos números de 0 a 9.

 Requisitos técnicos descritos abaixo:

Uso de Interrupções (IRQ): Toda a interação com os botões (A e B) foi feita por meio de rotinas de interrupção.
Debouncing: O tratamento do bouncing dos botões foi implementado via software, evitando múltiplos registros de um único pressionamento do botão.
Controle de LEDs: O controle dos LEDs  inclui tanto LEDs comuns quanto LEDs WS2812, demonstrando o uso de diferentes tipos de controle.

Requisitos de Hardware
Placa RP2040 ( Raspberry Pi Pico).
LED RGB (para piscar o LED vermelho).
Botões A e B (para incrementar e decrementar o número exibido).
Matriz de LEDs (para exibição do número).
LEDs WS2812 (para efeitos visuais dos números).

Funcionalidades

1. Piscar o LED Vermelho

O LED vermelho do LED RGB  pisca 5 vezes por segundo. Para isso, o código faz alternâncias entre ligar e desligar o LED com um intervalo de 200ms, criando o efeito de piscamento.

2. Incrementar e Decrementar com os Botões
Botão A: Ao ser pressionado, o número exibido na matriz de LEDs será incrementado em 1.
Botão B: Ao ser pressionado, o número exibido na matriz de LEDs será decrementado em 1.
O controle foi realizado por interrupções configuradas para cada botão, garantindo uma resposta imediata ao pressionamento.

3. Exibição de Números com LEDs WS2812
O número de 0 a 9 são exibidos utilizando LEDs WS2812. Cada número é  representado por um padrão específico de cores e brilho nos LEDs WS2812, criando um efeito visual que facilita a leitura do número na matriz.

Requisitos Técnicos
1. Uso de Interrupções (IRQ)
A interação com os botões foi tratada por rotinas de interrupção, garantindo que o programa responda imediatamente ao pressionamento de qualquer botão. Cada botão (A e B) tem a sua própria interrupção associada.

2. Debouncing
O tratamento de debouncing é obrigatório para evitar múltiplos registros de um único pressionamento do botão. A implementação foi feita por meio de um pequeno atraso (software) após cada detecção de pressionamento de botão.

3. Controle de LEDs
A placa RP2040 tem suporte tanto para LEDs comuns (utilizando pinos GPIO) quanto para LEDs WS2812. O controle dos LEDs é feito por meio de funções específicas, como gpio_set(), para LEDs comuns, e uma biblioteca adequada para controlar os LEDs WS2812.

4. Organização do Código
O código foi organizado em diferentes módulos, incluindo:

Módulo para o controle do LED RGB.
Módulo para a leitura e debouncing dos botões.
Módulo para o controle e exibição dos números nos LEDs WS2812.
Todos os módulos serão bem documentados e comentados para facilitar a compreensão.

Estrutura de Pastas
bash
Copiar
/src
    /main.c                 # Código principal do programa
    /led_rgb.c              # Controle do LED RGB
    /buttons.c              # Controle dos botões A e B
    /ws2812.c               # Controle dos LEDs WS2812
    /debouncing.c           # Implementação do debouncing dos botões
    /irq_handler.c          # Rotinas de interrupção dos botões
/inc
    /led_rgb.h              # Cabeçalho para controle do LED RGB
    /buttons.h              # Cabeçalho para controle dos botões
    /ws2812.h               # Cabeçalho para controle dos LEDs WS2812
    /debouncing.h           # Cabeçalho para debouncing
    /irq_handler.h          # Cabeçalho para interrupções
/Makefile                  # Makefile para compilação
Compilação e Execução
1. Configuração do Ambiente
Para compilar o código, foi necessário ter o ambiente de desenvolvimento para RP2040 configurado. 



2. Execução
Após carregar o código na placa RP2040, o programa inicia automaticamente. O LED vermelho começará a piscar, e a interação com os botões A e B controlará a exibição de números na matriz de LEDs WS2812.

Segue abaixo o link do vídeo demonstrando o funcionamento da placa.


https://drive.google.com/file/d/1Nr0aIxgP2RgO5l3GtDgXsK_UoeznQF0L/view?usp=sharing
