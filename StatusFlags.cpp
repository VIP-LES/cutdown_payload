#include "StatusFlags.h"

// Bit positions for flags
const uint32_t RELAY_FLAGS_OFFSET = 0;         // Relay states (bits 0-3)
const uint32_t OPERATIONAL_MODE_OFFSET = 4;    // Operational mode (bits 4-7)
const uint32_t GPS_TIME_FLAG_OFFSET = 8;       // GPS time flag (bit 8)
const uint32_t ONBOARD_TIME_FLAG_OFFSET = 9;   // Onboard relative time flag (bit 9)

// Function to set a bit
void StatusFlags::setFlag(uint32_t& flags, uint32_t bitPosition) {
    flags |= (1u << bitPosition);
}

// Function to clear a bit
void StatusFlags::clearFlag(uint32_t& flags, uint32_t bitPosition) {
    flags &= ~(1u << bitPosition);
}

// Function to toggle a bit
void StatusFlags::toggleFlag(uint32_t& flags, uint32_t bitPosition) {
    flags ^= (1u << bitPosition);
}

// Function to check if a bit is set
bool StatusFlags::isFlagSet(const uint32_t& flags, uint32_t bitPosition) {
    return (flags & (1u << bitPosition)) != 0;
}

// Example of setting relay states
void StatusFlags::setRelayState(uint32_t& flags, uint8_t relayNumber, bool state) {
    if (relayNumber < 4) { // Only supports 4 relays (0-3)
        if (state) {
            setFlag(flags, RELAY_FLAGS_OFFSET + relayNumber);
        } else {
            clearFlag(flags, RELAY_FLAGS_OFFSET + relayNumber);
        }
    }
}

// Example of setting operational mode
void StatusFlags::setOperationalMode(uint32_t& flags, uint8_t mode) {
    if (mode < 16) { // Operational mode range (0-15)
        flags = (flags & 0xFFFFFFF0) | (mode & 0x0F); // Clear old mode and set new mode
    }
}

// Set GPS Time
void StatusFlags::setGPSTime(uint32_t& flags, bool state) {
    if (state) {
        setFlag(flags, GPS_TIME_FLAG_OFFSET);
    } else {
        clearFlag(flags, GPS_TIME_FLAG_OFFSET);
    }
}

// Set Onboard Relative Time
void StatusFlags::setOnboardTime(uint32_t& flags, bool state) {
    if (state) {
        setFlag(flags, ONBOARD_TIME_FLAG_OFFSET);
    } else {
        clearFlag(flags, ONBOARD_TIME_FLAG_OFFSET);
    }
}