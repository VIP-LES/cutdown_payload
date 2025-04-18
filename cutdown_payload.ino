#include "globals.h"

void setup() {
  Serial.begin(115200);
  // while (!Serial) delay(10);

  callbacks[messaging_Command_ADJUST_TRANSMIT_POWER] = adjust_transmit_power_handler;
  callbacks[messaging_Command_SET_NICHROME] = set_nichrome_handler;
  callbacks[messaging_Command_SET_MOTOR] = set_motor_handler;
  callbacks[messaging_Command_CUTDOWN_PARACHUTE] = set_cutdown_parachute_handler;
  callbacks[messaging_Command_CUTDOWN_BALLOON] = set_cutdown_balloon_handler;


  radio_job.debug = true;
  sensor_job.debug = false;
  gps_job.debug = true;
  tele_job.debug = true;
  // datalog.debug = true;
  cutdown.debug = true;
  
  // TODO
  // add Initialized variable to all Jobs, if guard execute and poll with initialized
  // so we can not run their tasks if they fail to initialize

  // There's not much I can think of to do if these fail. 
  // Internally they will just do nothing if they fail to initialize.
  enum Outcome cutdown_status = cutdown.initialize();
  LOG_OUTCOME("Cutdown Manager initialization", cutdown_status);
  enum Outcome radio_status = radio_job.initialize(); 
  LOG_OUTCOME("radio initialization", radio_status);
  enum Outcome gps_status = gps_job.initialize(); 
  LOG_OUTCOME("gps initialization", gps_status);
  enum Outcome sensor_status = sensor_job.initialize(); 
  LOG_OUTCOME("MS8607 initialization", sensor_status);
  // enum Outcome datalog_status = datalog.initialize();
  // LOG_OUTCOME("DataLogger initialization", datalog_status);
  enum Outcome telemetry_job_status = tele_job.initialize();
  LOG_OUTCOME("Telemetry Job initialization", telemetry_job_status);
  
  
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

  if (tele_job.ready()) {
    tele_job.execute();
  }

  if (datalog.ready()) {
    datalog.execute();
  }

  if (cutdown.ready()) {
    cutdown.execute();
  }
}

const void adjust_transmit_power_handler(messaging_BasePacket *packet, bool *booldata, unsigned long *intdata, char **stringdata)
{
  if (intdata == NULL) {
    Serial.println("Can't adjust transmit power with no integer. Ignoring.");
    return;
  }
  int power = *intdata;
  if (power > 20 || power < 0) {
    Serial.print("Invalid power value (dbm): ");
    Serial.print(power);
    Serial.println(". Ignoring...");
    return;
  }
  radio_job.rfm95.setTxPower(power, false);
}

const void set_nichrome_handler(messaging_BasePacket *packet, bool *booldata, unsigned long *intdata, char **stringdata)
{
  // Due to the nature of how we coded communications :((
  // We cannot specify state and which number,
  // So this command turns every nichrome off, as a safety backup

  cutdown.set_relay_1(false);
  cutdown.set_relay_2(false);
  cutdown.set_relay_3(false);
  cutdown.set_relay_4(false);
}

const void set_motor_handler(messaging_BasePacket *packet, bool *booldata, unsigned long *intdata, char **stringdata)
{
  if (booldata != NULL) {
    bool on = *booldata;
    cutdown.set_motor(on ? MotorState::Forward : MotorState::Off);
  }
}

const void set_cutdown_parachute_handler(messaging_BasePacket *packet, bool *booldata, unsigned long *intdata, char **stringdata)
{
  if (booldata == NULL) {
    Serial.println("No boolean specified with cutdown parachute, ignoring...");
    return;
  }
  cutdown.cutdownParachute(*booldata);
}

const void set_cutdown_balloon_handler(messaging_BasePacket *packet, bool *booldata, unsigned long *intdata, char **stringdata)
{
  if (booldata == NULL)
  {
    Serial.println("No boolean specified with cutdown balloon, ignoring...");
    return;
  }
  cutdown.cutdownBalloon(*booldata);
}