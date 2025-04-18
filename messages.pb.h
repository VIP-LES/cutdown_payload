/* Automatically generated nanopb header */
/* Generated by nanopb-1.0.0-dev */

#ifndef PB_MESSAGING_MESSAGES_PB_H_INCLUDED
#define PB_MESSAGING_MESSAGES_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
/* Enum to define the bitflags for BasePacket */
typedef enum _messaging_PacketFlags {
    messaging_PacketFlags_NONE = 0, /* No flags set */
    messaging_PacketFlags_RELATIVE_TIME = 1, /* Indicates the timestamp is relative to the board time */
    messaging_PacketFlags_TEST_PACKET = 2 /* Set if the packet is using testing data and is not in production mode. */
} messaging_PacketFlags;

/* Enum to define command types for CommandPacket */
typedef enum _messaging_Command {
    messaging_Command_SET_NICHROME = 0,
    messaging_Command_SET_MOTOR = 1,
    messaging_Command_ADJUST_TRANSMIT_POWER = 2,
    messaging_Command_CUTDOWN_PARACHUTE = 3,
    messaging_Command_CUTDOWN_BALLOON = 4,
    messaging_Command_PSEUDO_NUM_COMMANDS = 5 /* more here */
} messaging_Command;

/* Struct definitions */
typedef struct _messaging_CommandPacket {
    messaging_Command command;
    pb_size_t which_command_data;
    union _messaging_CommandPacket_command_data {
        bool boolean_data;
        /* string string_data = 3; */
        uint32_t int_data;
    } command_data;
} messaging_CommandPacket;

typedef struct _messaging_AckPacket {
    messaging_Command command;
    uint32_t your_nonce;
} messaging_AckPacket;

/* GPS data message */
typedef struct _messaging_GPSData {
    /* Latitude (scaled to int32 with 6 decimal places) */
    int32_t latitude; /* 37.7749 * 1,000,000 = 37774900 */
    /* Longitude (scaled to int32 with 6 decimal places) */
    int32_t longitude; /* -122.4194 * 1,000,000 = -122419400 */
    /* Altitude (scaled to int32 with 6 decimal places) */
    int32_t altitude; /* 30.5 * 1,000,000 = 30500000 */
    /* GPS fix quality */
    uint32_t fix_quality;
    /* Number of satellites */
    uint32_t num_satellites;
} messaging_GPSData;

/* Define telemetry data (inner packet for telemetry) */
typedef struct _messaging_TelemetryPacket {
    /* Temperature (as scaled int32 to represent 6 decimal places) */
    int32_t temperature; /* 22.123456 * 1,000,000 = 22123456 */
    /* Humidity (scaled int32) */
    int32_t humidity; /* 55.234567 * 1,000,000 = 55234567 */
    /* Pressure (scaled int32) */
    int32_t pressure; /* 1013.25 * 1,000,000 = 1013250000 */
    /* GPS data. Not always present because the GPS takes time to fix. */
    bool has_gps_data;
    messaging_GPSData gps_data;
    bool relay_1;
    bool relay_2;
    bool relay_3;
    bool relay_4;
    bool motor;
} messaging_TelemetryPacket;

/* Define the base packet */
typedef struct _messaging_BasePacket {
    char callsign[10]; /* Fixed callsign (9 characters max) */
    uint64_t timestamp; /* Timestamp, e.g., Unix time (int64) */
    messaging_PacketFlags flags; /* Uses PacketFlag enum to represent bitflags */
    uint32_t nonce_id; /* Nonce ID (idempotency/logging), now uint32 */
    pb_size_t which_packet_data;
    union _messaging_BasePacket_packet_data {
        messaging_TelemetryPacket telemetry_packet;
        messaging_CommandPacket command_packet;
        messaging_AckPacket ack_packet; /* Other packet types can go here (e.g. ExecutedPacket) */
    } packet_data;
} messaging_BasePacket;


#ifdef __cplusplus
extern "C" {
#endif

/* Helper constants for enums */
#define _messaging_PacketFlags_MIN messaging_PacketFlags_NONE
#define _messaging_PacketFlags_MAX messaging_PacketFlags_TEST_PACKET
#define _messaging_PacketFlags_ARRAYSIZE ((messaging_PacketFlags)(messaging_PacketFlags_TEST_PACKET+1))

#define _messaging_Command_MIN messaging_Command_SET_NICHROME
#define _messaging_Command_MAX messaging_Command_PSEUDO_NUM_COMMANDS
#define _messaging_Command_ARRAYSIZE ((messaging_Command)(messaging_Command_PSEUDO_NUM_COMMANDS+1))

#define messaging_BasePacket_flags_ENUMTYPE messaging_PacketFlags


#define messaging_CommandPacket_command_ENUMTYPE messaging_Command

#define messaging_AckPacket_command_ENUMTYPE messaging_Command



/* Initializer values for message structs */
#define messaging_BasePacket_init_default        {"", 0, _messaging_PacketFlags_MIN, 0, 0, {messaging_TelemetryPacket_init_default}}
#define messaging_TelemetryPacket_init_default   {0, 0, 0, false, messaging_GPSData_init_default, 0, 0, 0, 0, 0}
#define messaging_CommandPacket_init_default     {_messaging_Command_MIN, 0, {0}}
#define messaging_AckPacket_init_default         {_messaging_Command_MIN, 0}
#define messaging_GPSData_init_default           {0, 0, 0, 0, 0}
#define messaging_BasePacket_init_zero           {"", 0, _messaging_PacketFlags_MIN, 0, 0, {messaging_TelemetryPacket_init_zero}}
#define messaging_TelemetryPacket_init_zero      {0, 0, 0, false, messaging_GPSData_init_zero, 0, 0, 0, 0, 0}
#define messaging_CommandPacket_init_zero        {_messaging_Command_MIN, 0, {0}}
#define messaging_AckPacket_init_zero            {_messaging_Command_MIN, 0}
#define messaging_GPSData_init_zero              {0, 0, 0, 0, 0}

/* Field tags (for use in manual encoding/decoding) */
#define messaging_CommandPacket_command_tag      1
#define messaging_CommandPacket_boolean_data_tag 2
#define messaging_CommandPacket_int_data_tag     4
#define messaging_AckPacket_command_tag          1
#define messaging_AckPacket_your_nonce_tag       2
#define messaging_GPSData_latitude_tag           1
#define messaging_GPSData_longitude_tag          2
#define messaging_GPSData_altitude_tag           3
#define messaging_GPSData_fix_quality_tag        4
#define messaging_GPSData_num_satellites_tag     5
#define messaging_TelemetryPacket_temperature_tag 1
#define messaging_TelemetryPacket_humidity_tag   2
#define messaging_TelemetryPacket_pressure_tag   3
#define messaging_TelemetryPacket_gps_data_tag   4
#define messaging_TelemetryPacket_relay_1_tag    5
#define messaging_TelemetryPacket_relay_2_tag    6
#define messaging_TelemetryPacket_relay_3_tag    7
#define messaging_TelemetryPacket_relay_4_tag    8
#define messaging_TelemetryPacket_motor_tag      9
#define messaging_BasePacket_callsign_tag        1
#define messaging_BasePacket_timestamp_tag       2
#define messaging_BasePacket_flags_tag           3
#define messaging_BasePacket_nonce_id_tag        4
#define messaging_BasePacket_telemetry_packet_tag 5
#define messaging_BasePacket_command_packet_tag  6
#define messaging_BasePacket_ack_packet_tag      7

/* Struct field encoding specification for nanopb */
#define messaging_BasePacket_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, STRING,   callsign,          1) \
X(a, STATIC,   SINGULAR, UINT64,   timestamp,         2) \
X(a, STATIC,   SINGULAR, UENUM,    flags,             3) \
X(a, STATIC,   SINGULAR, UINT32,   nonce_id,          4) \
X(a, STATIC,   ONEOF,    MESSAGE,  (packet_data,telemetry_packet,packet_data.telemetry_packet),   5) \
X(a, STATIC,   ONEOF,    MESSAGE,  (packet_data,command_packet,packet_data.command_packet),   6) \
X(a, STATIC,   ONEOF,    MESSAGE,  (packet_data,ack_packet,packet_data.ack_packet),   7)
#define messaging_BasePacket_CALLBACK NULL
#define messaging_BasePacket_DEFAULT NULL
#define messaging_BasePacket_packet_data_telemetry_packet_MSGTYPE messaging_TelemetryPacket
#define messaging_BasePacket_packet_data_command_packet_MSGTYPE messaging_CommandPacket
#define messaging_BasePacket_packet_data_ack_packet_MSGTYPE messaging_AckPacket

#define messaging_TelemetryPacket_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    temperature,       1) \
X(a, STATIC,   SINGULAR, INT32,    humidity,          2) \
X(a, STATIC,   SINGULAR, INT32,    pressure,          3) \
X(a, STATIC,   OPTIONAL, MESSAGE,  gps_data,          4) \
X(a, STATIC,   SINGULAR, BOOL,     relay_1,           5) \
X(a, STATIC,   SINGULAR, BOOL,     relay_2,           6) \
X(a, STATIC,   SINGULAR, BOOL,     relay_3,           7) \
X(a, STATIC,   SINGULAR, BOOL,     relay_4,           8) \
X(a, STATIC,   SINGULAR, BOOL,     motor,             9)
#define messaging_TelemetryPacket_CALLBACK NULL
#define messaging_TelemetryPacket_DEFAULT NULL
#define messaging_TelemetryPacket_gps_data_MSGTYPE messaging_GPSData

#define messaging_CommandPacket_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UENUM,    command,           1) \
X(a, STATIC,   ONEOF,    BOOL,     (command_data,boolean_data,command_data.boolean_data),   2) \
X(a, STATIC,   ONEOF,    UINT32,   (command_data,int_data,command_data.int_data),   4)
#define messaging_CommandPacket_CALLBACK NULL
#define messaging_CommandPacket_DEFAULT NULL

#define messaging_AckPacket_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UENUM,    command,           1) \
X(a, STATIC,   SINGULAR, UINT32,   your_nonce,        2)
#define messaging_AckPacket_CALLBACK NULL
#define messaging_AckPacket_DEFAULT NULL

#define messaging_GPSData_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT32,    latitude,          1) \
X(a, STATIC,   SINGULAR, INT32,    longitude,         2) \
X(a, STATIC,   SINGULAR, INT32,    altitude,          3) \
X(a, STATIC,   SINGULAR, UINT32,   fix_quality,       4) \
X(a, STATIC,   SINGULAR, UINT32,   num_satellites,    5)
#define messaging_GPSData_CALLBACK NULL
#define messaging_GPSData_DEFAULT NULL

extern const pb_msgdesc_t messaging_BasePacket_msg;
extern const pb_msgdesc_t messaging_TelemetryPacket_msg;
extern const pb_msgdesc_t messaging_CommandPacket_msg;
extern const pb_msgdesc_t messaging_AckPacket_msg;
extern const pb_msgdesc_t messaging_GPSData_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define messaging_BasePacket_fields &messaging_BasePacket_msg
#define messaging_TelemetryPacket_fields &messaging_TelemetryPacket_msg
#define messaging_CommandPacket_fields &messaging_CommandPacket_msg
#define messaging_AckPacket_fields &messaging_AckPacket_msg
#define messaging_GPSData_fields &messaging_GPSData_msg

/* Maximum encoded size of messages (where known) */
#define MESSAGING_MESSAGES_PB_H_MAX_SIZE         messaging_BasePacket_size
#define messaging_AckPacket_size                 8
#define messaging_BasePacket_size                122
#define messaging_CommandPacket_size             8
#define messaging_GPSData_size                   45
#define messaging_TelemetryPacket_size           90

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
