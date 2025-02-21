#ifndef STATUSFLAGS_H
#define STATUSFLAGS_H

#include <stdint.h>

namespace StatusFlags {
    // Function declarations
    void setFlag(uint32_t& flags, uint32_t bitPosition);
    void clearFlag(uint32_t& flags, uint32_t bitPosition);
    void toggleFlag(uint32_t& flags, uint32_t bitPosition);
    bool isFlagSet(const uint32_t& flags, uint32_t bitPosition);
    void setRelayState(uint32_t& flags, uint8_t relayNumber, bool state);
    void setOperationalMode(uint32_t& flags, uint8_t mode);
    void setGPSTime(uint32_t& flags, bool state);
    void setOnboardTime(uint32_t& flags, bool state);
}

#endif // STATUSFLAGS_H