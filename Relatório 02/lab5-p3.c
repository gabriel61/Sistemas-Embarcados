#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <rom/ets_sys.h>

#define INPUT_GPIO 4  // Define o pino de entrada que você deseja testar

void app_main(void) {
    // Variáveis para armazenar o tempo da última transição e o tempo atual
    uint32_t  ultimo = xTaskGetTickCount();
    uint32_t  atual = xTaskGetTickCount();
    double frequencia;
    uint32_t cont=0;
    // Variável para armazenar o período entre transições
    double  period = 0;
    // Inicializa o nível atual como 0 (baixo)
    int current_level = 0;
while (1){
    cont = 0;
    ultimo = xTaskGetTickCount();

 // Loop principal
    while (cont<5000000) {
        // Lê o nível lógico atual do pino de entrada
        int new_level = gpio_get_level(INPUT_GPIO);

        // Verifica se houve transição de alto para baixo ou de baixo para alto
        if (new_level != current_level) {
            // Atualiza o tempo atual com o tempo de tick atual

            // Calcula o período da onda como a diferença entre o tempo atual e o tempo da última transiçã
        // period += (atual - ultimo);
                // Calcula a frequência a partir do período
                        
            // Atualiza o tempo da última transição com o tempo atual
            //ultimo = atual;
            
            // Atualiza o nível atual para o novo nível lido
            current_level = new_level;
            cont++;
        }        

        // Adiciona um pequeno atraso para evitar alta carga na CPU
        // ets_delay_us(50);
    }
        atual = xTaskGetTickCount();
        period= (double)(atual-ultimo);
        period /= 100*cont/2;
    
        frequencia = 1/period;

     // Imprime a frequência detectada em Hz
        printf("Frequência detectada: %f Hz, %f s  \n", frequencia, period);

vTaskDelay(pdMS_TO_TICKS(500));

}
}