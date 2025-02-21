#ifndef MS8607_MODULE_H
#define MS8607_MODULE_H

#include <Adafruit_Sensor.h>
#include <Adafruit_MS8607.h>
#include "Module.h"

class MS8607_Module : public Module {
public:
    MS8607_Module() : 
    ms8607(), 
    pressureSensor(nullptr), 
    temperatureSensor(nullptr), 
    humiditySensor(nullptr) {}

    

    enum Outcome init() override {
        if (!ms8607.begin()) {
            Serial.println("Failed to init MS8607 chip");
            return TEMPORARY_FAILURE;
        }
        Serial.println("MS8607 Found!");

        ms8607.setPressureResolution(MS8607_PRESSURE_RESOLUTION_OSR_8192);
        
        // Store sensor instances for pressure, temperature, and humidity
        pressureSensor = ms8607.getPressureSensor();
        temperatureSensor = ms8607.getTemperatureSensor();
        humiditySensor = ms8607.getHumiditySensor();

        if (pressureSensor == NULL || temperatureSensor == NULL || humiditySensor == NULL) {
          return DEGRADED;
        }
        return SUCCESS;
    }

    void read(sensors_event_t *data) {
        if (data == NULL) return;

        Serial.println("-- MS8607 READING --");

        // Read Pressure
        if (pressureSensor) {
            pressureSensor->getEvent(data);
            Serial.print("Pressure: ");
            Serial.print(data->pressure);
            Serial.println(" hPa");
        } else {
            Serial.println("Pressure: **FAIL**");
            data->pressure = 0; // Reset to 0 if read fails
        }

        // Read Temperature
        if (temperatureSensor) {
            temperatureSensor->getEvent(data);
            Serial.print("Temperature: ");
            Serial.print(data->temperature);
            Serial.println(" °C");
        } else {
            Serial.println("Temperature: **FAIL**");
            data->temperature = 0; // Reset to 0 if read fails
        }

        // Read Humidity
        if (humiditySensor) {
            humiditySensor->getEvent(data);
            Serial.print("Humidity: ");
            Serial.print(data->relative_humidity);
            Serial.println(" %");
        } else {
            Serial.println("Humidity: **FAIL**");
            data->relative_humidity = 0;  // Reset to 0 if read fails
        }

        Serial.println("--------------------");
    }

private:
    Adafruit_MS8607 ms8607; // Instance of the MS8607 sensor
    Adafruit_Sensor* pressureSensor;     // Pointer to the pressure sensor instance
    Adafruit_Sensor* temperatureSensor;  // Pointer to the temperature sensor instance
    Adafruit_Sensor* humiditySensor;     // Pointer to the humidity sensor instance
};

#endif // MS8607_MODULE_H