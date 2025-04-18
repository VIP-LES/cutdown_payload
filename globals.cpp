#include "globals.h"
#define MICROSD_CS A5
#define R1 10
#define R2 11
#define R3 12
#define R4 13 // ALSO THE LED PIN...

#define Motor1 A1
#define Motor2 A2
#define SLP A3
#define FLT A4

const char *callsign = "KN4MXH-4";
MS8607_Job sensor_job = MS8607_Job();
UltimateGPS_Job gps_job = UltimateGPS_Job();
RFM9x_Job radio_job = RFM9x_Job();
Telemetry_Job tele_job = Telemetry_Job();
DataLogger_Job datalog = DataLogger_Job(MICROSD_CS, 1000);
CutdownManager_Job cutdown = CutdownManager_Job(R1, R2, R3, R4, Motor1, Motor2, SLP, FLT);

CommandCallbackFn callbacks[messaging_Command_PSEUDO_NUM_COMMANDS] = { 0 };