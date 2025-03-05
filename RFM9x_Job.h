#ifndef RFM9X_JOB_H
#define RFM9X_JOB_H

#include "Job.h"
#include <RH_RF95.h>

#define SCRATCH_SIZE 256 // bytes
#define UNUSED_TIMEOUT 1000 // ms

class RFM9x_Job: public Job {
  public:
    RH_RF95 rfm95;
  
    // Just what the example said; Hardcoded Pins for Adafruit Feather.
    RFM9x_Job() : rfm95(8, 3), Job(UNUSED_TIMEOUT) {}

    enum Outcome initialize() override {
        if (!rfm95.init()) {
          JOB_DEBUG("RFM9x Initialization Failed!");
          return Outcome::FailurePermanent; // Permanent because this chip is onboard
                                            // And I don't think failure will be fixable.
        }

        rfm95.setTxPower(20, false);

        return Outcome::Success;
    }

    bool ready() override {
      return rfm95.available();
    }

    enum Outcome close() override {
      //TODO
      return Outcome::FailurePermanent;
    }

    enum Outcome execute() override {
      time_t now = millis();
      static uint8_t packet_scratch_buffer[SCRATCH_SIZE];
      uint8_t bytes_read = 0;
      if (!rfm95.recv(packet_scratch_buffer, &bytes_read)) {
        return Outcome::Waiting;
      }
      JOB_DEBUG("Received a radio packet!");
      
      // TODO
      // Packet is in packet_scratch_buffer from [0, bytes_read);
      // Parse using nanopb setup
      // switch case for command handling, ideally in another file.
      // build some convenience methods for sending on this device.

      last_execution_time = now;
      return Outcome::Success;
    }
};

#endif /* RFM9X_JOB_H */