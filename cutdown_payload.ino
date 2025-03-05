
#include <Wire.h>
#include <Adafruit_MS8607.h>
#include <Adafruit_Sensor.h>c

#include "MS8607_Job.h"
#include "UltimateGPS_Job.h"


MS8607_Job sensor_job = MS8607_Job();
UltimateGPS_Job gps_job = UltimateGPS_Job();


void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  sensor_job.debug = true;
  gps_job.debug = true;
  
  // Should check init status!!!
  sensor_job.initialize();
  gps_job.initialize();
  
}

void loop() {
  gps_job.poll();

  if (sensor_job.ready()) {
    sensor_job.execute();
  }

  if (gps_job.ready()) {
    gps_job.execute();
  }

}