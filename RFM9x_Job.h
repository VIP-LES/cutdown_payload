#ifndef RFM9X_JOB_H
#define RFM9X_JOB_H

#include "Job.h"
#include <RH_RF95.h>
#include "messages.pb.h"
#include <pb_decode.h>
#include "utils.h"

#define SCRATCH_SIZE RH_RF95_MAX_MESSAGE_LEN // bytes
#define UNUSED_TIMEOUT 1000 // ms

/**
 * @class RFM9x_Job
 * @brief Defines initialization, destruction, and functionality
 * upon receiving command packet of the RFM9x onboard the payload
 */
class RFM9x_Job: public Job {
  public:
    RH_RF95 rfm95;
  
    // Just what the example said; Hardcoded Pins for Adafruit Feather.
    RFM9x_Job() : rfm95(8, 3), Job(UNUSED_TIMEOUT) {}

    /**
     * @brief Initializes RFM9x and gives it power
     * 
     * @details Tx power is set to 20 because this is 
     * ostensibly the maximum power
     * 
     * @return Outcome enum that indicates success or PERMANENT failure
     * (if initialization fails, it is unlikely to be able to be fixed)
     */
    enum Outcome initialize() override {
        if (!rfm95.init()) {
          JOB_DEBUG("RFM9x Initialization Failed!");
          return Outcome::FailurePermanent;
        }

        rfm95.setTxPower(20, false);

        return Outcome::Success;
    }

    /**
     * @brief Indicates if the radio has received data
     * and that it is ready to be read
     */
    bool ready() override {
      return rfm95.available();
    }

    /**
     * TODO!!
     */
    enum Outcome close() override {
      return Outcome::FailurePermanent;
    }

    /**
     * @brief Receives, decodes, and executes command packet
     * 
     * @details If message has not been received, Outcome::Waiting enum
     * returned. If decoding fails, Outcome::FailureRetry enum returned.
     * If decoding succeeds, confirms packet is a command and then
     * executes the command. If not a command packet, Outcome::Partial
     * returned (not our fault, just an unexpected packet type was
     * received).
     * 
     * @return see details
     */
    enum Outcome execute() override {
      time_t now = millis();
      static uint8_t packet_scratch_buffer[SCRATCH_SIZE];
      uint8_t bytes_read = SCRATCH_SIZE;
      if (!rfm95.recv(packet_scratch_buffer, &bytes_read)) {
        return Outcome::Waiting;
      }
      JOB_DEBUG("Received a radio packet!");

      pb_istream_t stream = pb_istream_from_buffer(packet_scratch_buffer, bytes_read);
      messaging_BasePacket packet = messaging_BasePacket_init_zero;
      // packet.packet_data.command_packet.command_data.string_data.funcs.decode = NULL;
      if (!pb_decode(&stream, messaging_BasePacket_fields, &packet)) {
        JOB_DEBUG("Failed to decode a packet, ignoring!");
        return Outcome::FailureRetry;
      }

    Serial.print("Length received: _");
    Serial.println(bytes_read);
    Serial.print("[RAW RX]: ");
    printBase64(packet_scratch_buffer, bytes_read);
    pretty_print_packet(&packet);
          
      switch(packet.which_packet_data) {
        case messaging_BasePacket_command_packet_tag:
          handle_command_packet(&packet);
          break;
        case messaging_BasePacket_telemetry_packet_tag:
          JOB_DEBUG("We received telemetry, weird! Ignoring...");
          return Outcome::Partial;
        default:
          JOB_DEBUG("Unknown packet type. Ignoring...");
          return Outcome::Partial;
      };

      last_execution_time = now;
      return Outcome::Success;
    }

    private:
      void handle_command_packet(messaging_BasePacket *packet);
      bool sendAck(messaging_BasePacket * packet);
};




#endif /* RFM9X_JOB_H */


