#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "esp_timer.h"
#include <driver/adc.h>
#include <esp_adc_cal.h>

int cont = 0;
uint32_t voltage[1000];
esp_timer_handle_t timer_handler;
esp_adc_cal_characteristics_t adc_cal;

static const char *TAG = "ADC CAL";

static void periodic_timer_callback(void* arg){
    uint32_t adc_reading = adc1_get_raw(ADC1_CHANNEL_0);
    voltage[cont] = esp_adc_cal_raw_to_voltage(adc_reading, &adc_cal);
    cont ++;
    if(cont==1000){
        for (int i = 0; i < cont; i++){
            printf("%lu, ", (unsigned long)voltage[i]);
        }
        esp_timer_stop(timer_handler);
    }   
}

void timer_setup(){
    const esp_timer_create_args_t periodic_timer_args ={
        .callback = &periodic_timer_callback,
        .name = "periodic"
    };
    esp_timer_create(&periodic_timer_args, &timer_handler);
    
}

void app_main(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_12);
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12, 1100, &adc_cal);
    timer_setup();
    esp_timer_start_periodic(timer_handler,(uint64_t) 1000);
   
}