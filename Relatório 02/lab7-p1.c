#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#define RED GPIO_NUM_13
#define DELAY_TIME_MS 200

volatile bool button_pressed = false;

void IRAM_ATTR gpio_isr_handler(void* arg) {
    button_pressed = true;
}

void button_config() {
    printf("Configuring button\n");
    gpio_install_isr_service(0);
    gpio_set_direction(RED, GPIO_MODE_INPUT);
    gpio_pullup_en(RED);
    gpio_set_intr_type(RED, GPIO_INTR_POSEDGE);
    gpio_isr_handler_add(RED, gpio_isr_handler, NULL);
    printf("Button config complete\n");
}

void app_main(void) {
    int cont = 0;
    printf("Start\n");
    button_config();

    while (1) {
        printf("Counter: %d\n", cont);
        if (button_pressed) {            
            button_pressed = false; // Resetar o flag após detecção
            cont = 0; // Resetar o contador
            printf("OPA\n");
        }
        else {
            cont++; // Incrementar o contador se o botão não estiver pressionado
        }
        vTaskDelay(pdMS_TO_TICKS(DELAY_TIME_MS));
    }
}