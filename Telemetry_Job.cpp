#include "globals.h"
#include "TimeKeeper.h"
#include "Telemetry_Job.h"

#include <pb.h>
#include <pb_encode.h>
#include "messages.pb.h"

enum Outcome Telemetry_Job::execute()
{
    time_t now = millis();
    last_execution_time = now;


    messaging_BasePacket packet = messaging_BasePacket_init_zero;
    strncpy(packet.callsign, callsign, MAX_CALLSIGN_SIZE);
    packet.nonce_id = (uint32_t)random();
    
    uint64_t time;
    enum TimeFormat format = get_time(&time);

    if (format == TIME_RELATIVE) {
        packet.flags = static_cast<messaging_PacketFlags>(messaging_PacketFlags_RELATIVE_TIME | packet.flags);
    }
    packet.timestamp = time;

    packet.which_packet_data = messaging_BasePacket_telemetry_packet_tag;

    SET_VALUE(packet.packet_data.telemetry_packet.temperature,sensor_job.temperature);
    SET_VALUE(packet.packet_data.telemetry_packet.pressure,sensor_job.pressure);
    SET_VALUE(packet.packet_data.telemetry_packet.humidity, sensor_job.humidity);
    SET_GPS_DATA(packet.packet_data.telemetry_packet.gps_data, gps_job.GPS);

    packet.packet_data.telemetry_packet.relay_1 = cutdown.r1;
    packet.packet_data.telemetry_packet.relay_2 = cutdown.r2;
    packet.packet_data.telemetry_packet.relay_3 = cutdown.r3;
    packet.packet_data.telemetry_packet.relay_4 = cutdown.r4;
    bool motor_state;
    switch (cutdown.motor) {
        case MotorState::Forward:
            motor_state = true;
            break;
        case MotorState::Reverse:
            motor_state = true;
            break;
        case MotorState::Off:
            motor_state = false;
            break;
        default:
            motor_state = true;
            break;
    };
    packet.packet_data.telemetry_packet.motor = motor_state;

    datalog.log(
        sensor_job.temperature, 
        sensor_job.pressure, 
        sensor_job.humidity, 
        cutdown.r1 ? '1': '0', 
        cutdown.r2 ? '1': '0', 
        cutdown.r3 ? '1': '0', 
        cutdown.r4 ? '1' : '0', 
        motor_state ? '1' : '0'
    );

    static uint8_t packet_buffer[PACKET_MAX_SIZE];
    pb_ostream_t output_stream = pb_ostream_from_buffer(packet_buffer, sizeof(packet_buffer));
    bool successful_encode = pb_encode(&output_stream, messaging_BasePacket_fields, &packet);

    if (successful_encode) {
        radio_job.rfm95.send(packet_buffer, output_stream.bytes_written);
        return Outcome::Success;
    } else {
        JOB_DEBUG("Failed to encode and send packet in Telemetry Job");
        return Outcome::FailureRetry;
    }

    return Outcome::Success;
}