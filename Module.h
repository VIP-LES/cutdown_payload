#ifndef MODULE_H
#define MODULE_H

class Module {
public:

    enum Outcome {
        SUCCESS,
        DEGRADED,
        TEMPORARY_FAILURE,
        PERMANENT_FAILURE
    };

    virtual enum Outcome init() = 0; // Pure virtual function for initialization
};

#endif // MODULE_H