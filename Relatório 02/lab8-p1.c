#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_timer.h"

// Definindo os pinos e variáveis
#define LED_GPIO_PIN GPIO_NUM_2 

static int segundos = 0, minutos = 0, horas = 0;
bool casa = 1 ;

static void IRAM_ATTR timer_callback(void* arg) {
    
    segundos++;

    casa = !casa;
    gpio_set_level(LED_GPIO_PIN, casa);

    if (segundos > 59) {
        segundos = 0;
        minutos++;
    }
    if (minutos > 59) {
        minutos = 0;
        horas++;
    }
    if (horas > 23) {
        horas = 0;
    }
    
    printf("%02d:%02d:%02d\n", horas, minutos, segundos);
}

void app_main() {

    // Configura o pino do LED como saída
    gpio_reset_pin(LED_GPIO_PIN);
    gpio_set_direction(LED_GPIO_PIN, GPIO_MODE_OUTPUT);

    // Configura e inicia o temporizador
    const esp_timer_create_args_t timer_args = {
        .callback = &timer_callback,
        .name = "periodic"
    };

    esp_timer_handle_t periodic_timer;
    esp_timer_create(&timer_args, &periodic_timer);
    esp_timer_start_periodic(periodic_timer, 1000000); // 1 s em ms
}