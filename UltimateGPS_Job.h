#ifndef ULTIMATEGPS_JOB_H
#define ULTIMATEGPS_JOB_H

#include <Adafruit_GPS.h>
#include "Job.h"

#define GPS_PRINT_TIME 1000 //ms
#define GPSSerial Serial1

class UltimateGPS_Job : public Job {
public:
    Adafruit_GPS GPS;

    UltimateGPS_Job() : Job(GPS_PRINT_TIME), GPS(&GPSSerial) {}

    enum Outcome initialize() override {
        if (!GPS.begin(9600)) {
            JOB_DEBUG("Failed to open Serial to Ultimate GPS module");
            return Outcome::FailureRetry;
        }
        // I don't believe this is conclusive
        // JOB_DEBUG("Ultimate GPS Found!");

        GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
        GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
        GPS.sendCommand(PGCMD_ANTENNA);

        GPS.println(PMTK_Q_RELEASE);

        return Outcome::Success;
    }

    bool ready() override {
      return GPS.newNMEAreceived();
    }

    void poll() override {
      char c = GPS.read();
    }

    enum Outcome close() override {
      // TODO
      return Outcome::FailurePermanent;
    }

    enum Outcome execute() override {
      time_t now = millis();

      if (!GPS.newNMEAreceived()) {
          return Outcome::Waiting;
      }

      if (!GPS.parse(GPS.lastNMEA())) {
        return Outcome::FailureRetry;
      }

      if (now - last_execution_time > GPS_PRINT_TIME) {
        if (this->debug) {
          Serial.println("-- GPS READING --");

          Serial.print("Time: ");
          if (GPS.hour < 10) { Serial.print('0'); }
          Serial.print(GPS.hour, DEC); Serial.print(':');
          if (GPS.minute < 10) { Serial.print('0'); }
          Serial.print(GPS.minute, DEC); Serial.print(':');
          if (GPS.seconds < 10) { Serial.print('0'); }
          Serial.print(GPS.seconds, DEC); Serial.print('.');
          if (GPS.milliseconds < 10) {
              Serial.print("00");
          } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
              Serial.print("0");
          }
          Serial.println(GPS.milliseconds);

          Serial.print("Date: ");
          Serial.print(GPS.day, DEC); Serial.print('/');
          Serial.print(GPS.month, DEC); Serial.print("/20");
          Serial.println(GPS.year, DEC);

          if (GPS.fix) {
              Serial.print("Fix: Found");
              Serial.print(" quality: ");
              Serial.println((int)GPS.fixquality);

              Serial.print("Location: ");
              Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
              Serial.print(", ");
              Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);

              Serial.print("Speed (knots): ");
              Serial.println(GPS.speed);

              Serial.print("Angle: ");
              Serial.println(GPS.angle);

              Serial.print("Altitude: ");
              Serial.println(GPS.altitude);
          } else {
              Serial.println("Fix: Not Found");
          }

          Serial.print("Satellites: ");
          Serial.println((int)GPS.satellites);

          Serial.print("Antenna status: ");
          Serial.println((int)GPS.antenna);

          Serial.println("--------------------");\

          last_execution_time = now;
        }
}

      return Outcome::Success;
    }
};

#endif /* ULTIMATEGPS_JOB_H */