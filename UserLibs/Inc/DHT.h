//
// Created by Telephone on 2021/5/12.
//

#ifndef THSENSORSYSTEM_DHT_H
#define THSENSORSYSTEM_DHT_H

#include <stdint.h>

typedef enum DHT_PIN_MODE {
    DHT_PIN_MODE_INPUT, DHT_PIN_MODE_OUTPUT, DHT_PIN_MODE_INPUT_PULLUP
} DHT_PIN_MODE;

typedef enum DHT_PIN_VALUE {
    DHT_PIN_VALUE_LOW, DHT_PIN_VALUE_HIGH
} DHT_PIN_VALUE;

typedef enum dht_sensor_type {
    dht_sensor_type_DHT11, dht_sensor_type_DHT12, dht_sensor_type_DHT21, dht_sensor_type_DHT22
} dht_sensor_type;

typedef enum dht_temperature_scale {
    dht_temperature_scale_Celcius = 0, dht_temperature_scale_Fahrenheit
} dht_temperature_scale;

void DHT_pinMode(uint16_t pin, DHT_PIN_MODE mode);

void DHT_pinMode_Output_Low(uint16_t pin, DHT_PIN_MODE mode);

void DHT_pinMode_Output_High(uint16_t pin, DHT_PIN_MODE mode);

void DHT_delay(unsigned long ms);

void DHT_delayMicroseconds(unsigned long us);

void DHT_digitalWrite(uint16_t pin, DHT_PIN_VALUE value);

DHT_PIN_VALUE DHT_digitalRead(uint16_t pin);

long long DHT_microsecondsToClockCycles(long long us);

void DHT_disable_interrupt();

void DHT_enable_interrupt();

void dht_init(uint16_t pin);

// dht_read() 或 dht_readTemperatureAndHumidity() 的两次调用之间应至少间隔2s
int dht_read(uint16_t pin, uint8_t data[5], dht_sensor_type _type);

// dht_read() 或 dht_readTemperatureAndHumidity() 的两次调用之间应至少间隔2s
int dht_readTemperatureAndHumidity(uint16_t pin, dht_sensor_type sensor_type, dht_temperature_scale S, float *temperature, float *humidity);

#endif //THSENSORSYSTEM_DHT_H
