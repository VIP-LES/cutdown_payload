
#include <Wire.h>
#include <Adafruit_MS8607.h>
#include <Adafruit_Sensor.h>

#include "DataDump.h"
#include "MS8607_Module.h"


MS8607_Module baroSensor = MS8607_Module();

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  
  enum Module::Outcome baroStatus = baroSensor.init();
  if (baroStatus != Module::SUCCESS) {
    Serial.println("-- FAILED TO INIT BAROMETRIC SENSOR --");
  }
  
}

void loop() {
  struct GpsData gps;
  sensors_event_t baroReadings;
  
  baroSensor.read(&baroReadings);

  delay(500);

}