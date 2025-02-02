#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7
#define BUTTON_A 5  // Pino do botão para incrementar o numero gerado na matriz de leds
#define BUTTON_B 6  // Pino do botão para decrementar o numero gerado na matriz de leds
#define led_pin_red 13

static volatile uint32_t last_time_a = 0; // Armazena o tempo do último evento para BUTTON_A
static volatile uint32_t last_time_b = 0; // Armazena o tempo do último evento para BUTTON_B
static const uint32_t debounce_time = 200000; // 200ms de debouncing  (em microssegundos)

// Representação dos números de 0 a 9 como matriz 5x5
float numbers[10][25] = {
    {0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0},  // 1

    {0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 1.0, 0.0, 0.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 0.0, 0.0, 1.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0},  // 2

    {0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 1.0, 0.0, 0.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 1.0, 0.0, 0.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0},  // 3

    {0.0, 1.0, 0.0, 1.0, 0.0,
     0.0, 1.0, 0.0, 1.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 1.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 1.0, 0.0},  // 4

    {0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 0.0, 0.0, 1.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 1.0, 0.0, 0.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0},  // 5

    {0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 0.0, 0.0, 1.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 1.0, 0.0, 1.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0},  // 6

    {0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 1.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 1.0, 0.0,
     0.0, 1.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 1.0, 0.0},  // 7

    {0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 1.0, 0.0, 1.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 1.0, 0.0, 1.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0},  // 8

    {0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 1.0, 0.0, 1.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 1.0, 0.0, 0.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0},  // 9

    {0.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 1.0, 0.0, 1.0, 0.0,
     0.0, 1.0, 0.0, 1.0, 0.0,
     0.0, 1.0, 0.0, 1.0, 0.0,
     0.0, 1.0, 1.0, 1.0, 0.0}   // 0
};

// Variáveis globais para controle dos LEDs
uint32_t led_buffer[NUM_PIXELS] = {0}; // Buffer para armazenar as cores de todos os LEDs
uint8_t current_number = 0;  // Número atual a ser exibido

// Função para enviar um pixel para a matriz
static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

// Função para criar uma cor no formato GRB
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// Função para atualizar o buffer de LEDs
void update_led_buffer() {
    // Apaga todos os LEDs
    for (int i = 0; i < NUM_PIXELS; i++) {
        led_buffer[i] = 0; // Desliga todos os LEDs
    }

    // Carrega o número atual na matriz
    for (int i = 0; i < NUM_PIXELS; i++) {
        if (numbers[current_number][i] == 1.0) {
            led_buffer[i] = urgb_u32(0, 20, 0);  // Cor verde
        }
    }
}

// Função para enviar o estado de todos os LEDs para a matriz
void set_leds_from_buffer() {
    for (int i = 0; i < NUM_PIXELS; i++) {
        put_pixel(led_buffer[i]);
    }
}

// Função de interrupção para os botões com debouncing
static void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = time_us_32(); // Obtém o tempo atual em microssegundos

    if (gpio == BUTTON_A) {
        // Verifica se passou o tempo de debounce (200ms)
        if (current_time - last_time_a > debounce_time) {
            last_time_a = current_time;
            // Incrementa o número atual (de 0 a 9)
            current_number = (current_number + 1) % 10;
            // Apaga todos os LEDs antes de acender novos
            update_led_buffer();
            // Atualiza a matriz de LEDs com o novo número
            set_leds_from_buffer();
        }
    }

    if (gpio == BUTTON_B) {
        // Verifica se passou o tempo de debounce (200ms)
        if (current_time - last_time_b > debounce_time) {
            last_time_b = current_time;
            // Decrementa o número atual (de 0 a 9)
            current_number = (current_number - 1 + 10) % 10;
            // Apaga todos os LEDs antes de acender novos
            update_led_buffer();
            // Atualiza a matriz de LEDs com o novo número
            set_leds_from_buffer();
        }
    }
}

int main() {
    stdio_init_all();
    printf("WS2812 5x5 Matrix - Exibindo Números via Botão\n");

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    // Inicializa o PIO para controlar os LEDs WS2812
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    // Inicializa os pinos dos botões
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);  // Habilita o resistor pull-up interno

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);  // Habilita o resistor pull-up interno

    gpio_init(led_pin_red); // Inicializa a porta 13 do microcontrolador
    gpio_set_dir(led_pin_red, GPIO_OUT); // Configura a porta como saída

    // Configura as interrupções dos botões para borda de queda
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (1) {
        // O loop principal está vazio, pois a ação depende da interrupção do botão
        tight_loop_contents();

        // Pisca o LED vermelho indicando que o programa está aguardando a entrada
        printf("Pisca led vermelho\n");
        gpio_put(led_pin_red, true); //  Led vermelho ligado
        sleep_ms(100); // Função de temporização em ms
        gpio_put(led_pin_red, false); // Led vermelho desligado
        sleep_ms(100);
    }

    return 0;
}
