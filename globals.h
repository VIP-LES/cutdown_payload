#ifndef GLOBALS_H
#define GLOBALS_H

#include "messages.pb.h"

#include "MS8607_Job.h"
#include "UltimateGPS_Job.h"
#include "RFM9x_Job.h"
#include "Telemetry_Job.h"
#include "DataLogger_Job.h"
#include "CutdownManager_Job.h"

extern const char * callsign;
extern MS8607_Job sensor_job;
extern UltimateGPS_Job gps_job;
extern RFM9x_Job radio_job;
extern Telemetry_Job tele_job;
extern DataLogger_Job datalog;
extern CutdownManager_Job cutdown;

typedef const void (*CommandCallbackFn)(messaging_BasePacket *packet, bool *booldata, unsigned long *intdata, char **stringdata);
extern CommandCallbackFn callbacks[messaging_Command_PSEUDO_NUM_COMMANDS];

#endif /* GLOBALS_H */