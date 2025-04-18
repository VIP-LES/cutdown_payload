#ifndef TELEMETRY_JOB_H
#define TELEMETRY_JOB_H

#define TELEMETRY_INTERVAL 5000
#define MAX_CALLSIGN_SIZE 10 // Need to take from options later...
#define PACKET_MAX_SIZE 251 // Max size of packet buffer. Can use from rf9x lib that defines it.. TODO

#define MILLION_FIX(x) ((int32_t)((x) * 1000000))

#define SET_VALUE(key, value) do { \
    (key) = MILLION_FIX(value); \
} while (0)

#define SET_GPS_DATA(gps_data, gps) do { \
    SET_VALUE((gps_data).altitude, (gps).altitude); \
    SET_VALUE((gps_data).latitude, (gps).latitude); \
    SET_VALUE((gps_data).longitude, (gps).longitude); \
    (gps_data).num_satellites = (gps).satellites; \
    (gps_data).fix_quality = (gps).fixquality; \
} while (0)

class Telemetry_Job: public Job {
public:

    Telemetry_Job() : Job(TELEMETRY_INTERVAL) {}

    enum Outcome initialize() override {
        return Outcome::Success;
    }

    enum Outcome close() override {
        return Outcome::Success;
    }

    enum Outcome execute() override;
};

#endif /* TELEMETRY_JOB_H */