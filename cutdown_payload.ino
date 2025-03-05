#include "MS8607_Job.h"
#include "UltimateGPS_Job.h"
#include "RFM9x_Job.h"

const char* callsign = "KN4MXH-3";

MS8607_Job sensor_job = MS8607_Job();
UltimateGPS_Job gps_job = UltimateGPS_Job();
RFM9x_Job radio_job = RFM9x_Job();

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  radio_job.debug = true;
  sensor_job.debug = true;
  gps_job.debug = true;
  
  // TODO
  // add Initialized variable to all Jobs, if guard execute and poll with initialized
  // so we can not run their tasks if they fail to initialize

  // There's not much I can think of to do if these fail. 
  // Internally they will just do nothing if they fail to initialize.
  enum Outcome radio_status = radio_job.initialize(); 
  LOG_OUTCOME("radio initialization", radio_status);
  enum Outcome gps_status = gps_job.initialize(); 
  LOG_OUTCOME("gps initialization", gps_status);
  enum Outcome sensor_status = sensor_job.initialize(); 
  LOG_OUTCOME("MS8607 initialization", sensor_status);
  
}

void loop() {
  gps_job.poll();

  if (sensor_job.ready()) {
    sensor_job.execute();
  }

  if (gps_job.ready()) {
    gps_job.execute();
  }

  if (radio_job.ready()) {
    radio_job.execute();
  }
}

