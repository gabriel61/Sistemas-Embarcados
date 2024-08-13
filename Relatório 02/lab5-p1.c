#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define TEST_GPIO 4

void test_gpio_pull_mode(gpio_num_t gpio_num, gpio_pull_mode_t pull_mode) {
   
gpio_config_t io_conf = {
    .pin_bit_mask = (1ULL << gpio_num),
    
    .mode = GPIO_MODE_INPUT,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .intr_type = GPIO_INTR_DISABLE
};
    
   // Verifica se a configuração é para habilitar o resistor de pull-up
if (pull_mode == GPIO_PULLUP_ONLY) {
    // Habilita o resistor de pull-up para o pino
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
}
// Verifica se a configuração é para habilitar o resistor de pull-down
else if (pull_mode == GPIO_PULLDOWN_ONLY) {
    // Habilita o resistor de pull-down para o pino
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
}

gpio_config(&io_conf);
int value = gpio_get_level(gpio_num);

    if (pull_mode == GPIO_PULLUP_ONLY) {
        printf("GPIO %d configurado com resistor pull-up. Valor lido: %d\n", gpio_num, value);
    } else if (pull_mode == GPIO_PULLDOWN_ONLY) {
        printf("GPIO %d configurado com resistor pull-down. Valor lido: %d\n", gpio_num, value);
    } else {
        printf("GPIO %d configurado sem resistor interno. Valor lido: %d\n", gpio_num, value);
    }
}

void app_main(void) {
    
    printf("GPIO entrada diferentes configurações");
   
    // Teste da porta com resistor pull-up
    //Manda o numero da porta de entrada e o modo da porta de entrada
    test_gpio_pull_mode(TEST_GPIO, GPIO_PULLUP_ONLY);
    // Aguarda um pouco antes do próximo teste
    vTaskDelay(pdMS_TO_TICKS(1000));

    // Teste da porta com resistor pull-down
    test_gpio_pull_mode(TEST_GPIO, GPIO_PULLDOWN_ONLY);
    // Aguarda um pouco antes do próximo teste
    vTaskDelay(pdMS_TO_TICKS(1000));

    // Teste da porta sem resistor interno
    test_gpio_pull_mode(TEST_GPIO, GPIO_FLOATING);
    vTaskDelay(pdMS_TO_TICKS(1000));
     
}