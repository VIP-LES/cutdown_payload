#include "globals.h"
#include "time.h"
#include "TimeKeeper.h"

enum TimeFormat get_time(time_t * time) {
    if (gps_job.GPS.fix) {
      tm gpsTime;
      gpsTime.tm_year = gps_job.GPS.year - 1900;  // tm_year is years since 1900
      gpsTime.tm_mon  = gps_job.GPS.month - 1;    // tm_mon is 0-based
      gpsTime.tm_mday = gps_job.GPS.day;
      gpsTime.tm_hour = gps_job.GPS.hour;
      gpsTime.tm_min  = gps_job.GPS.minute;
      gpsTime.tm_sec  = gps_job.GPS.seconds;
      time_t epoch = mktime(&gpsTime);
      *time = epoch;
      return TIME_GPS;
    } else {
      *time = millis();
      return TIME_RELATIVE;
    }
}