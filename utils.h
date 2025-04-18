#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include "messages.pb.h"

void pretty_print_packet(const messaging_BasePacket *packet);

void printBase64(const uint8_t* data, size_t len);

#endif /* UTILS_H */