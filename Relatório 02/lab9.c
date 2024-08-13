#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define UART_NUM UART_NUM_1
#define TX_PIN (GPIO_NUM_17)
#define RX_PIN (GPIO_NUM_16)
#define BUF_SIZE (1024)

static const char *TAG = "UART_LOOP";

void uart_init() {
    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    uart_driver_install(UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL, 0);
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

 char tx_data[] = {'A', 'D', 'G'};
    int tx_index = 0;
    char rx_data;
    uint8_t rx_buffer[BUF_SIZE];

void uart_loop_task(void *arg) {

    while (1) {

        // Envia o caractere via UART
        ESP_LOGI(TAG, "Enviando: %c", tx_data[tx_index]);
        uart_write_bytes(UART_NUM, &tx_data[tx_index], 1);

        // Recebe o caractere via UART
         int rx_bytes = uart_read_bytes(UART_NUM, rx_buffer, 1, pdMS_TO_TICKS(1000));
            //int rx_bytes = 0;
            //rx_buffer[0]=0;

            if (rx_bytes  > 0) {
            rx_data = rx_buffer[0];
            // Imprime o caractere recebido
            ESP_LOGE(TAG, "Recebido %c\n", rx_data);
           
        } else {
            ESP_LOGE(TAG, "Nenhum dado recebido.");
        }
         tx_index++;
        // Incrementa o caractere para o próximo no alfabeto
     
        if (tx_index >= sizeof(tx_data)) {
            tx_index = 0;  // Reinicia o índice se atingir o final do vetor
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void app_main(void) {
    uart_init();
    xTaskCreate(uart_loop_task, "uart_loop_task", 2048, NULL, 10, NULL);   
}