#ifndef DATALOGGER_JOB_H
#define DATALOGGER_JOB_H

#include "CSVLogger.h"
#include "TimeKeeper.h"

class DataLogger_Job : public CSVLogger
{
    public: 
    const char * HEADER = "timestamp, temp, pressure, humidity, r1, r2, r3, r4, motor";
    DataLogger_Job(int CS, time_t syncInterval) : CSVLogger("data.csv", CS, HEADER, syncInterval) {}

    void log(float temp, float pressure, float humidity, char r1, char r2, char r3, char r4, char motor) {
        String line;
        line.reserve(64); // optimize memory

        uint64_t time;
        enum TimeFormat format = get_time(&time);
        if (format == TIME_RELATIVE) {
            line += "REL:";
        } else if (format == TIME_GPS) {
            line += "GPS:";
        } else {
            line += "OTH:";
        }

        char timebuf[21]; // max length for 64-bit int + null terminator
        sprintf(timebuf, "%llu", time);  // or PRIu64 for portable code
        line += String(timebuf);
        line += ",";
        line += String(temp, 2);
        line += ",";
        line += String(pressure, 2);
        line += ",";
        line += String(humidity, 2);
        line += ",";
        line += r1;
        line += ",";
        line += r2;
        line += ",";
        line += r3;
        line += ",";
        line += r4;
        line += ",";
        line += motor;

        bufferLine(line);
    }

    void log(float temp, float pressure, float humidity) {
        log(temp, pressure, humidity, 'X', 'X', 'X', 'X', 'X');
    }

    void log(char r1, char r2, char r3, char r4, char motor) {
        log(-1, -1, -1, r1, r2, r3, r4, motor);
    }
};

#endif /* DATALOGGER_JOB_H */