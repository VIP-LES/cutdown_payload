#ifndef DATADUMP_H
#define DATADUMP_H

#include <stdint.h>
#include <vector>

struct GPSData {
    int32_t latitude;      // Latitude in millionths of degrees
    int32_t longitude;     // Longitude in millionths of degrees
    int32_t altitude;      // Altitude in tenths of meters
    int32_t satellites;    // Number of satellites
    int32_t fix_quality;   // GPS fix quality
};

struct DataDump {
    int64_t timestamp;                       // Timestamp (Unix time or relative time)
    uint32_t flags;                          // Top-level integer for flags
    int32_t temperature_value;               // Temperature (int, multiplied by 10^6)
    int32_t humidity_value;                  // Humidity (int, multiplied by 10^6)
    int32_t barometric_pressure_value;       // Barometric Pressure (int, multiplied by 10^6)
    GPSData gps;                             // GPS data information
    std::vector<uint32_t> command_ids;       // List of heard command IDs (unsigned integers)
};

#endif // DATADUMP_H