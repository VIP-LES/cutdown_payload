#ifndef JOB_H
#define JOB_H

#include <Arduino.h>
#include <ctime>

#define LOG_OUTCOME(name, outcome) \
    Serial.print("["); \
    Serial.print(millis()); \
    Serial.print("]: "); \
    Serial.print(name); \
    Serial.print(" "); \
    switch(outcome) { \
        case Outcome::Success: \
            Serial.println("succeeded"); \
            break; \
        case Outcome::Partial: \
            Serial.println("partially succeeded"); \
            break; \
        case Outcome::Waiting: \
            Serial.println("is waiting"); \
            break; \
        case Outcome::FailureRetry: \
            Serial.println("failed temporarily"); \
            break; \
        case Outcome::FailurePermanent: \
            Serial.println("failed permanently"); \
            break; \
    }

#define JOB_DEBUG(MESSAGE) \
    do { if (this->debug) Serial.println((MESSAGE)); } while (0)

enum class Outcome {
    Success,
    Partial,
    Waiting,
    FailureRetry,
    FailurePermanent
};

class Job {
public:
    bool debug = false;

    Job(time_t schedule_interval) : interval(schedule_interval), last_execution_time(0) {}

    virtual ~Job() = default;

    // for setting up jobs that periodically run
    virtual Outcome initialize() = 0;
    // For larger periodic tasks
    virtual Outcome execute() = 0;
    //  Use for cleaning up jobs
    virtual Outcome close() = 0;
    // Use for very short but necessary rapid calls, ie: reading characters in from UART
    virtual void poll() {};

    // Determines if the job is ready to execute (can be overridden)
    virtual bool ready() {
        return millis() - last_execution_time >= interval;
    }

protected:
    time_t interval;  // Interval in seconds
    time_t last_execution_time;
};



#endif /* JOB_H */