#include "TimeKeeper.h"
#include "RFM9x_Job.h"
#include "globals.h"
#include <pb_encode.h>
#include <pb_decode.h>
#include "messages.pb.h"

/**
 * @brief Logic to handle received command packet
 * 
 * @details Invokes the callback associated with the type of command packet
 * (as specified by the command packet ID), if such ID is valid
 * 
 * @param packet pointer to received packet
 */
void RFM9x_Job::handle_command_packet(messaging_BasePacket *packet) {
    // Logs received callsign to serial output
    JOB_DEBUG("---- COMMAND PACKET RECEIVED ----");
    Serial.print("CALLSIGN: ");
    Serial.println(packet->callsign);

    uint64_t time;
    enum TimeFormat format = get_time(&time);

    // Prints time with specified format (relative, GPS, other)
    Serial.print("Received At ");
    if (format == TIME_RELATIVE)
    {
        Serial.print("REL:");
    }
    else if (format == TIME_GPS)
    {
        Serial.print("GPS:");
    }
    else
    {
        Serial.print("OTH:");
    }
    Serial.println(time);

    // Checks for invalid command ID, and if invalid, returns
    if (packet->packet_data.command_packet.command >= messaging_Command_PSEUDO_NUM_COMMANDS || packet->packet_data.command_packet.command < 0)
    {
        JOB_DEBUG("INVALID COMMAND ENUM ID. IGNORING...");
        return;
    }

    // Retrieves callback associated with command ID
    CommandCallbackFn callback = callbacks[packet->packet_data.command_packet.command];
    if (callback == NULL)
    {
        JOB_DEBUG("CALLBACK FOR COMMAND NOT FOUND (null). IGNORING.");
    }

    bool *booldata = NULL;
    unsigned long *intdata = NULL;
    char **stringdata = NULL;

    // Retrieves either boolean or int data from packet
    // (string is currently unsupported)
    switch (packet->packet_data.command_packet.which_command_data)
    {
    case messaging_CommandPacket_boolean_data_tag:
        booldata = &(packet->packet_data.command_packet.command_data.boolean_data);
        break;
    case messaging_CommandPacket_int_data_tag:
        intdata = &(packet->packet_data.command_packet.command_data.int_data);
        break;
        // Unsupported for now
        // case messaging_CommandPacket_string_data_tag:
        //   stringdata = &(packet->packet_data.command_packet.command_data.string_data);
    };

    // Invokes callback with data retrieved from packet
    callback(packet, booldata, intdata, NULL);

    // Send ack (acknolwedgement) packet
    sendAck(packet);

    JOB_DEBUG("---------------------------------");
}

/**
 * @brief Creates and encodes acknowledgement packet to transmit
 * 
 * @details Creates the acknowledgement packet, encodes it, and then writes
 * it to a buffer to transmit. If encoding fails, funciton returns false.
 * 
 * @param packet pointer to packet that is being acknowledged
 * @returns true if encoding succeeds, false if it fails
 */
bool RFM9x_Job::sendAck(messaging_BasePacket *packet) {
    messaging_BasePacket ackpack = messaging_AckPacket_init_zero;
    strncpy(ackpack.callsign, callsign, MAX_CALLSIGN_SIZE);

    uint64_t time;
    enum TimeFormat format = get_time(&time);

    if (format == TIME_RELATIVE)
    {
        ackpack.flags = static_cast<messaging_PacketFlags>(messaging_PacketFlags_RELATIVE_TIME | packet->flags);
    }
    ackpack.timestamp = time;

    ackpack.nonce_id = (uint32_t)random();

    ackpack.which_packet_data = messaging_BasePacket_ack_packet_tag;
    ackpack.packet_data.ack_packet.command = packet->packet_data.command_packet.command;
    ackpack.packet_data.ack_packet.your_nonce = packet->nonce_id;

    static uint8_t scratch[SCRATCH_SIZE];
    pb_ostream_t stream = pb_ostream_from_buffer(scratch, sizeof(scratch));

    // Checks if enconding fails
    if (!pb_encode(&stream, messaging_BasePacket_fields, &ackpack))
    {
        JOB_DEBUG("Failed to encode acknowlegement packet!!");
        return false;
    }
    int length = stream.bytes_written;

    // Sends encoded packet
    rfm95.send(scratch, length);
    return true;
}