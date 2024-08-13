#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/pcnt.h"
#include "sdkconfig.h"
#include "esp_timer.h" 

#define RED GPIO_NUM_13
#define PCNT_INPUT_SIG_IO 4
#define DELAY_TIME_MS 200

volatile bool button_pressed = false;

void IRAM_ATTR gpio_isr_handler(void* arg) {
    button_pressed = true;
}

static void pcnt_example_init(void) {
    pcnt_config_t pcnt_config = {
        .pulse_gpio_num = PCNT_INPUT_SIG_IO,
        .ctrl_gpio_num = PCNT_PIN_NOT_USED, // Não usar pino de controle
        .channel = PCNT_CHANNEL_0,
        .unit = PCNT_UNIT_0,
        .pos_mode = PCNT_COUNT_INC,   // Contar pulsos na borda de subida
        .neg_mode = PCNT_COUNT_DIS,   // Não contar pulsos na borda de descida
        .lctrl_mode = PCNT_MODE_KEEP, // Ignorar sinal de controle
        .hctrl_mode = PCNT_MODE_KEEP, // Ignorar sinal de controle
        .counter_h_lim = 0,           // Sem limite superior
        .counter_l_lim = 0,           // Sem limite inferior
    };

    // Inicializa a unidade de contagem de pulsos
    pcnt_unit_config(&pcnt_config);

    // Configura o filtro de pulsos (opcional)
    pcnt_set_filter_value(PCNT_UNIT_0, 100);
    pcnt_filter_enable(PCNT_UNIT_0);

    // Inicializa o contador
    pcnt_counter_pause(PCNT_UNIT_0);
    pcnt_counter_clear(PCNT_UNIT_0);

    // Inicia o contador
    pcnt_counter_resume(PCNT_UNIT_0);
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
    int16_t count = 0;
    printf("Start\n");
    button_config();
    pcnt_example_init();

    while (1) {
        pcnt_counter_clear(PCNT_UNIT_0);
        vTaskDelay(pdMS_TO_TICKS(50));
        pcnt_get_counter_value(PCNT_UNIT_0, &count);
        printf("Pulse count: %d\n", count);       
    }
}