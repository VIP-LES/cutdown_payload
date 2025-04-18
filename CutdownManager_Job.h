#ifndef CUTDOWN_MANAGER_JOB_H
#define CUTDOWN_MANAGER_JOB_H

#include "Job.h"

#define INTERVAL_CHECK_MS 1000

enum class MotorState
{
    Off,
    Forward,
    Reverse
};

class CutdownManager_Job : public Job
{
public:

    const int NICHROME_MS = 5000;
    const int MOTOR_MS = 150000;

    bool r1 = false;
    bool r2 = false;
    bool r3 = false;
    bool r4 = false;

    MotorState motor = MotorState::Off;


    CutdownManager_Job(int r1_pin, int r2_pin, int r3_pin, int r4_pin, int motor1_pin, int motor2_pin, int slp_pin, int flt_pin) : Job(INTERVAL_CHECK_MS),
                                                                                                         _r1_pin(r1_pin),
                                                                                                         _r2_pin(r2_pin),
                                                                                                         _r3_pin(r3_pin),
                                                                                                         _r4_pin(r4_pin),
                                                                                                         _motor1_pin(motor1_pin),
                                                                                                         _motor2_pin(motor2_pin),
                                                                                                         _slp_pin(slp_pin),
                                                                                                         _flt_pin(flt_pin)
                                                                                                          {}

    bool ready() override {
        return (cutdown_balloon_stage != 0 || cutdown_parachute_stage != 0 ) && (millis() - last_execution_time >= interval);
    }
    
    enum Outcome initialize() override {
        pinMode(_r1_pin, OUTPUT);
        digitalWrite(_r1_pin, HIGH);
        pinMode(_r2_pin, OUTPUT);
        digitalWrite(_r2_pin, HIGH);
        pinMode(_r3_pin, OUTPUT);
        digitalWrite(_r3_pin, HIGH);
        pinMode(_r4_pin, OUTPUT);
        digitalWrite(_r4_pin, HIGH);

        pinMode(_motor1_pin, OUTPUT);
        digitalWrite(_motor1_pin, LOW);
        pinMode(_motor2_pin, OUTPUT);
        digitalWrite(_motor2_pin, LOW);

        pinMode(_slp_pin, OUTPUT);
        digitalWrite(_slp_pin, HIGH);

        pinMode(_flt_pin, INPUT_PULLUP);

        return Outcome::Success;
    }

    enum Outcome execute() override {
        time_t now = millis();
        if (cutdown_balloon_stage != 0 && (now - cutdown_balloon_stage_ms > MOTOR_MS || cutdown_balloon_stage_ms == 0) ) {
            switch (cutdown_balloon_stage)
            {
            case 1:
                set_motor(MotorState::Forward);
                cutdown_balloon_stage_ms = now;
                cutdown_balloon_stage++;
                break;
            case 2:
                set_motor(MotorState::Off);
                cutdown_balloon_stage_ms = 0;
                cutdown_balloon_stage = 0;
            default:
                break;
            }
        }
        if (cutdown_parachute_stage != 0 && (now - cutdown_parachute_stage_ms > NICHROME_MS || cutdown_parachute_stage_ms == 0)) {
            switch (cutdown_parachute_stage)
            {
            case 1:
                set_relay_1(true);
                cutdown_parachute_stage_ms = now;
                cutdown_parachute_stage++;
                break;
            case 2:
                set_relay_1(false);
                set_relay_2(true);
                cutdown_parachute_stage_ms = now;
                cutdown_parachute_stage++;
                break;
            case 3:
                set_relay_2(false);
                set_relay_3(true);
                cutdown_parachute_stage_ms = now;
                cutdown_parachute_stage++;
                break;
            case 4:
                set_relay_3(false);
                set_relay_4(true);
                cutdown_parachute_stage_ms = now;
                cutdown_parachute_stage++;
                break;
            case 5:
                set_relay_1(false);
                set_relay_2(false);
                set_relay_3(false);
                set_relay_4(false);
                cutdown_parachute_stage_ms = 0;
                cutdown_parachute_stage = 0;

            default:
                break;
            }
        }

        last_execution_time = now;
        return Outcome::Success;
    }

    enum Outcome close() override {
        set_relay_1(false);
        set_relay_2(false);
        set_relay_3(false);
        set_relay_4(false);
        set_motor(MotorState::Off);
        return Outcome::Success;
    }

    void cutdownParachute(bool startstop) {
        cutdown_parachute_stage = startstop;
        cutdown_parachute_stage_ms = 0;
        if (!startstop) {    
            set_relay_1(false);
            set_relay_2(false);
            set_relay_3(false);
            set_relay_4(false);
        }
        JOB_DEBUG("CUTDOWN PARACHUTE TRIGGERED WITH VALUE: " + startstop);
    }

    void cutdownBalloon(bool startstop) {
        cutdown_balloon_stage_ms = 0;
        cutdown_balloon_stage = startstop;
        if (!startstop) {
            set_motor(MotorState::Off);
        }
        JOB_DEBUG("CUTDOWN BALLOON TRIGGERED WITH VALUE: " + startstop);
    }

    bool set_relay_1(bool state) {
        digitalWrite(_r1_pin, !state);
        r1 = state;
        JOB_DEBUG("R1 SET: " + r1);
        return r1;
    }
    bool set_relay_2(bool state) {
        digitalWrite(_r2_pin, !state);
        r2 = state;
        JOB_DEBUG("R2 SET: " + r2);
        return r2;
    }
    bool set_relay_3(bool state) {
        digitalWrite(_r3_pin, !state);
        r3 = state;
        JOB_DEBUG("R3 SET: " + r3);
        return r3;
    }
    bool set_relay_4(bool state) {
        digitalWrite(_r4_pin, !state);
        r4 = state;
        JOB_DEBUG("R4 SET: " + r4);
        return r4;
    }

    bool set_motor(enum MotorState state) {
        bool on;
        switch (state) {
            case MotorState::Forward:
                digitalWrite(_motor1_pin, HIGH);
                digitalWrite(_motor2_pin, LOW);
                on = true;
                break;
            case MotorState::Reverse:
                digitalWrite(_motor1_pin, LOW);
                digitalWrite(_motor2_pin, HIGH);
                on = true;
                break;
            case MotorState::Off:
                digitalWrite(_motor1_pin, LOW);
                digitalWrite(_motor2_pin, LOW);
                on = false;
                break;
        }
        motor = state;
        
        JOB_DEBUG("MOTOR SET: " + on);
        if (!digitalRead(_flt_pin)) {
          JOB_DEBUG("FAULT PIN FOR MOTOR IS LOW. INDICATES MOTOR FAILURE");
            return false; // Pull low of fault pin means motor failure...
        }
        return true; // Succeeded
    }
    

protected:
    int _r1_pin;
    int _r2_pin;
    int _r3_pin;
    int _r4_pin;
    int _motor1_pin;
    int _motor2_pin;
    int _slp_pin;
    int _flt_pin;

    int cutdown_parachute_stage = 0;
    time_t cutdown_parachute_stage_ms;
    int cutdown_balloon_stage = 0;
    time_t cutdown_balloon_stage_ms;
};

#endif /* CUTDOWN_MANAGER_JOB_H */