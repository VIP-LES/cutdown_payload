#include "globals.h"
#include "time.h"
#include "TimeKeeper.h"

/**
 * @brief Gets time: GPS time if possible, else relative time
 * 
 * @details Writes to an address pointed to by the provided pointer,
 * rather than returning the time itself. Instead, will return an 
 * enum indicating the type of time returned (gps or relative).
 * 
 * @param time pointer to where the value will be stored
 * @return TimeFormat enum (TIME_GPS or TIME_RELATIVE)
 */
enum TimeFormat get_time(uint64_t* time) {
    if (gps_job.GPS.fix) {
        uint16_t year  = gps_job.GPS.year + 2000;
        uint8_t  month = gps_job.GPS.month;
        uint8_t  day   = gps_job.GPS.day;
        uint8_t  hour  = gps_job.GPS.hour;
        uint8_t  min   = gps_job.GPS.minute;
        uint8_t  sec   = gps_job.GPS.seconds;

        // Days per month, for non-leap and leap years
        static const uint8_t days_in_month[] = {
            31, 28, 31, 30, 31, 30,
            31, 31, 30, 31, 30, 31
        };

        // Leap year check
        auto is_leap = [](uint16_t y) -> bool {
            return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
        };

        // Count days since 1970-01-01
        uint32_t days = 0;

        for (uint16_t y = 1970; y < year; ++y)
            days += is_leap(y) ? 366 : 365;

        for (uint8_t m = 1; m < month; ++m) {
            days += days_in_month[m - 1];
            if (m == 2 && is_leap(year)) days += 1; // Feb in leap year
        }

        days += (day - 1); // Days so far in current month

        // Convert to seconds
        *time = ((uint64_t)days * 86400) + (hour * 3600) + (min * 60) + sec;
        return TIME_GPS;

    } else {
        *time = millis();
        return TIME_RELATIVE;
    }
}
