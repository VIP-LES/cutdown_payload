#ifndef MS8607_JOB_H
#define MS8607_JOB_H

#include <Adafruit_Sensor.h>
#include <Adafruit_MS8607.h>

#include "Job.h"

#define MS8607_READ_INTERVAL 1000

class MS8607_Job: public Job {
public:
    MS8607_Job() :
    Job(MS8607_READ_INTERVAL),
    ms8607(), 
    pressureSensor(nullptr), 
    temperatureSensor(nullptr), 
    humiditySensor(nullptr) {}

    float temperature;
    float pressure;
    float humidity;

    enum Outcome initialize() override {
        if (!ms8607.begin()) {
            JOB_DEBUG("Failed to init MS8607 chip");
            return Outcome::FailureRetry;
        }

        ms8607.setPressureResolution(MS8607_PRESSURE_RESOLUTION_OSR_8192);
        
        // Store sensor instances for pressure, temperature, and humidity
        pressureSensor = ms8607.getPressureSensor();
        temperatureSensor = ms8607.getTemperatureSensor();
        humiditySensor = ms8607.getHumiditySensor();

        if (pressureSensor == nullptr || temperatureSensor == nullptr || humiditySensor == nullptr) {
          return Outcome::Partial;
        }
        
        return Outcome::Success;
    }

    enum Outcome close() override {
      // TODO
      return Outcome::FailurePermanent;
    }

    enum Outcome execute() override {
      time_t now = millis();

      bool partialFailure = false;

      sensors_event_t readings;
      if (temperatureSensor != nullptr && temperatureSensor->getEvent(&readings)) {
        this->temperature = readings.temperature;
      } else {
        partialFailure = true;
        this->temperature = -1;
      }
      if (humiditySensor != nullptr && humiditySensor->getEvent(&readings)) {
        this->humidity = readings.relative_humidity;
      } else {
        partialFailure = true;
        this->humidity = -1;
      }
      if (pressureSensor != nullptr && pressureSensor->getEvent(&readings)) {
        this->pressure = readings.pressure;
      } else {
        partialFailure = true;
        this->pressure = -1;
      }

      if (this->debug) {
        Serial.println("-- MS8607 READING --");

        if (this->temperature != -1) {
          Serial.print("Temperature: ");
          Serial.print(this->temperature);
          Serial.println(" °C");
        } else {
          Serial.println("Temperature: **FAIL**");
        }

        if (this->humidity != -1) {
          Serial.print("Humidity: ");
          Serial.print(this->humidity);
          Serial.println(" %");
        } else {
          Serial.println("Humidity: **FAIL**");
        }

        if (this->pressure != -1) {
          Serial.print("Pressure: ");
          Serial.print(this->pressure);
          Serial.println(" hPa");
        } else {
          Serial.println("Pressure: **FAIL**");
        }

        Serial.println("--------------------");
      }


      last_execution_time = now;

      if (partialFailure) {
        return Outcome::Partial;
      } else {
        return Outcome::Success;
      }
    }
    

private:
    Adafruit_MS8607 ms8607; // Instance of the MS8607 sensor
    Adafruit_Sensor* pressureSensor;     // Pointer to the pressure sensor instance
    Adafruit_Sensor* temperatureSensor;  // Pointer to the temperature sensor instance
    Adafruit_Sensor* humiditySensor;     // Pointer to the humidity sensor instance
};

#endif /* MS8607_MODULE_H */