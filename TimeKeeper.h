#ifndef TIMEKEEPER_H
#define TIMEKEEPER_H

#include "time.h"

enum TimeFormat {
    TIME_RELATIVE,
    TIME_GPS,
    TIME_EXTERNAL
};

enum TimeFormat get_time(uint64_t * time);

#endif /* TIMEKEEPER_H */