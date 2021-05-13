//
// Created by Telephone on 2021/5/12.
//

#include "DHT.h"

#define PULL_TIME_USEC 55UL
#define TIMEOUT UINT32_MAX

typedef unsigned int word;

void dht_init(uint16_t pin){
    DHT_pinMode(pin, DHT_PIN_MODE_INPUT_PULLUP);
}

static uint32_t expectPulse(uint16_t pin, DHT_PIN_VALUE level) {
    uint32_t _maxcycles =
            DHT_microsecondsToClockCycles(1000); // 1 millisecond timeout for
    // reading pulses from DHT sensor.
    // Note that count is now ignored as the DHT reading algorithm adjusts itself
    // based on the speed of the processor.
#if (F_CPU > 16000000L)
    uint32_t count = 0;
#else
    uint16_t count = 0; // To work fast enough on slower AVR boards
#endif
// On AVR platforms use direct GPIO port access as it's much faster and better
// for catching pulses that are 10's of microseconds in length:
#ifdef __AVR
    uint8_t portState = level ? _bit : 0;
  while ((*portInputRegister(_port) & _bit) == portState) {
    if (count++ >= _maxcycles) {
      return TIMEOUT; // Exceeded timeout, fail.
    }
  }
// Otherwise fall back to using digitalRead (this seems to be necessary on
// ESP8266 right now, perhaps bugs in direct port access functions?).
#else
    while (DHT_digitalRead(pin) == level) {
        if (count++ >= _maxcycles) {
            return TIMEOUT; // Exceeded timeout, fail.
        }
    }
#endif

    return count;
}

int dht_read(uint16_t pin, uint8_t data[5], dht_sensor_type _type){
    // Reset 40 bits of received data to zero.
    data[0] = data[1] = data[2] = data[3] = data[4] = 0;

    // Send start signal.  See DHT datasheet for full signal diagram:
    //   http://www.adafruit.com/datasheets/Digital%20humidity%20and%20temperature%20sensor%20AM2302.pdf
    DHT_pinMode(pin, DHT_PIN_MODE_INPUT_PULLUP);
    DHT_delay(1);

    // First set data line low for a period according to sensor type
    DHT_pinMode(pin, DHT_PIN_MODE_OUTPUT);
    DHT_digitalWrite(pin, DHT_PIN_VALUE_LOW);
    switch (_type) {
        case dht_sensor_type_DHT22:
        case dht_sensor_type_DHT21:
            DHT_delayMicroseconds(1100); // data sheet says "at least 1ms"
            break;
        case dht_sensor_type_DHT11:
        default:
            DHT_delay(20); // data sheet says at least 18ms, 20ms just to be safe
            break;
    }

    uint32_t cycles[80];
    {
        // End the start signal by setting data line high for 40 microseconds.
        DHT_pinMode(pin, DHT_PIN_MODE_INPUT_PULLUP);

        // Delay a moment to let sensor pull data line low.
        DHT_delayMicroseconds(PULL_TIME_USEC);

        // Now start reading the data line to get the value from the DHT sensor.

        // Turn off interrupts temporarily because the next sections
        // are timing critical and we don't want any interruptions.
        DHT_disable_interrupt();

        // First expect a low signal for ~80 microseconds followed by a high signal
        // for ~80 microseconds again.
        if (expectPulse(pin, DHT_PIN_VALUE_LOW) == TIMEOUT) {
            return 0;
        }
        if (expectPulse(pin, DHT_PIN_VALUE_HIGH) == TIMEOUT) {
            return 0;
        }

        // Now read the 40 bits sent by the sensor.  Each bit is sent as a 50
        // microsecond low pulse followed by a variable length high pulse.  If the
        // high pulse is ~28 microseconds then it's a 0 and if it's ~70 microseconds
        // then it's a 1.  We measure the cycle count of the initial 50us low pulse
        // and use that to compare to the cycle count of the high pulse to determine
        // if the bit is a 0 (high state cycle count < low state cycle count), or a
        // 1 (high state cycle count > low state cycle count). Note that for speed
        // all the pulses are read into a array and then examined in a later step.
        for (int i = 0; i < 80; i += 2) {
            cycles[i] = expectPulse(pin, DHT_PIN_VALUE_LOW);
            cycles[i + 1] = expectPulse(pin, DHT_PIN_VALUE_HIGH);
        }
        DHT_enable_interrupt();
    } // Timing critical code is now complete.

    // Inspect pulses and determine which ones are 0 (high state cycle count < low
    // state cycle count), or 1 (high state cycle count > low state cycle count).
    for (int i = 0; i < 40; ++i) {
        uint32_t lowCycles = cycles[2 * i];
        uint32_t highCycles = cycles[2 * i + 1];
        if ((lowCycles == TIMEOUT) || (highCycles == TIMEOUT)) {
            return 0;
        }
        data[i / 8] <<= 1;
        // Now compare the low and high cycle times to see if the bit is a 0 or 1.
        if (highCycles > lowCycles) {
            // High cycles are greater than 50us low cycle count, must be a 1.
            data[i / 8] |= 1;
        }
        // Else high cycles are less than (or equal to, a weird case) the 50us low
        // cycle count so this must be a zero.  Nothing needs to be changed in the
        // stored data.
    }

    // Check we read 40 bits and that the checksum matches.
    if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
        return 1;
    } else {
        return 0;
    }
}

static float convertCtoF(float c) { return c * 1.8 + 32; }

static float convertFtoC(float f) { return (f - 32) * 0.55555; }

int dht_readTemperatureAndHumidity(uint16_t pin, dht_sensor_type sensor_type, dht_temperature_scale S, float *temperature, float *humidity){
    float ft, fh;
    uint8_t data[5] = {0};
    if (dht_read(pin, data, sensor_type)) {
        switch (sensor_type) {
            case dht_sensor_type_DHT11:
            { // ft
                ft = data[2];
                if (data[3] & 0x80) {
                    ft = -1 - ft;
                }
                ft += (data[3] & 0x0f) * 0.1;
                if (S) {
                    ft = convertCtoF(ft);
                }
            }
                { // fh
                    fh = data[0] + data[1] * 0.1;
                }
                break;
            case dht_sensor_type_DHT12:
            { // ft
                ft = data[2];
                ft += (data[3] & 0x0f) * 0.1;
                if (data[2] & 0x80) {
                    ft *= -1;
                }
                if (S) {
                    ft = convertCtoF(ft);
                }
            }
                { // fh
                    fh = data[0] + data[1] * 0.1;
                }
                break;
            case dht_sensor_type_DHT22:
            case dht_sensor_type_DHT21:
            { // ft
                ft = ((word) (data[2] & 0x7F)) << 8 | data[3];
                ft *= 0.1;
                if (data[2] & 0x80) {
                    ft *= -1;
                }
                if (S) {
                    ft = convertCtoF(ft);
                }
            }
                { // fh
                    fh = ((word)data[0]) << 8 | data[1];
                    fh *= 0.1;
                }
                break;
        }
        *temperature = ft;
        *humidity = fh;
        return 1;
    }
    return 0;
}
