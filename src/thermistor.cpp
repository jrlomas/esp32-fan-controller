#include "math.h"
#include "config.h"
#include "log.h"
#include "temperatureController.h"

#define ANALOG_READ_PIN 36

#define MAX_ADC_VALUE 4095
#define NUM_SAMPLES 100

float temp;

void initThermistor(void){
  temp = NAN;
  pinMode(ANALOG_READ_PIN, INPUT);
}

void updateThermistor(void){
    float adc_values[NUM_SAMPLES];
    for(int i = 0; i < NUM_SAMPLES; i++){
        float value = analogRead(ANALOG_READ_PIN);
        adc_values[i] = value;
    }
    // // order the array
    // for(int i = 0; i < NUM_SAMPLES; i++){
    //     for(int j = i + 1; j < NUM_SAMPLES; j++){
    //         if(adc_values[i] > adc_values[j]){
    //             float temp = adc_values[i];
    //             adc_values[i] = adc_values[j];
    //             adc_values[j] = temp;
    //         }
    //     }
    // }
    // now find the median
    //float adc_value = adc_values[NUM_SAMPLES / 2];
    // find the average
    float adc_value = 0;
    for(int i = 0; i < NUM_SAMPLES; i++){
        adc_value += adc_values[i];
    }

    adc_value /= NUM_SAMPLES;
    // We are now going to convert 100K thermistor readings to temperature in degrees Celsius
    // https://learn.adafruit.com/thermistor/using-a-thermistor
    float v_in = 3.3;
    float v_out = 5.0 * adc_value / MAX_ADC_VALUE;
    float resistance = (v_in / v_out - 1) * 10000;
    //Log.printf("Resistance: %.2f\r\n", resistance);
    float steinhart;
    steinhart = resistance / 100000; // (R/Ro)
    steinhart = log(steinhart); // ln(R/Ro)
    steinhart /= 3950; // 1/B * ln(R/Ro)
    steinhart += 1.0 / (25 + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart; // Invert
    steinhart -= 273.15; // convert to C
    temp = steinhart;
    //Log.printf(" Temperature: %.2f, Resistance: %.2f, ADC Value: %.2f\r\n", steinhart, resistance, adc_value);
    #ifndef setActualTemperatureViaMQTT
    setActualTemperatureAndPublishMQTT(temp);
    #endif
}