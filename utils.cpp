#include "utils.h"

void pretty_print_packet(const messaging_BasePacket *packet)
{
    // --- Line 1: [timestamp] @ CALLSIGN, NONCE [TEST?] ---
    Serial.print("[");
    if (packet->flags & messaging_PacketFlags_RELATIVE_TIME)
    {
        Serial.print("Rel:");
    }
    else
    {
        Serial.print("Epoch:");
    }
    Serial.print(packet->timestamp);
    Serial.print("] @ ");
    Serial.print(packet->callsign);
    Serial.print(", NONCE ");
    Serial.print(packet->nonce_id);
    if (packet->flags & messaging_PacketFlags_TEST_PACKET)
    {
        Serial.print(" [TEST]");
    }
    Serial.println();

    // --- Line 2: command, telemetry, or ack data ---
    switch (packet->which_packet_data)
    {
    case messaging_BasePacket_telemetry_packet_tag:
    {
        messaging_TelemetryPacket *tp = (messaging_TelemetryPacket *)&packet->packet_data.telemetry_packet;
        Serial.print("Telemetry: T=");
        Serial.print(tp->temperature / 1000000.0, 6);
        Serial.print("C, H=");
        Serial.print(tp->humidity / 1000000.0, 6);
        Serial.print("%, P=");
        Serial.print(tp->pressure / 1000000.0, 6);
        Serial.print("hPa");
        break;
    }
    case messaging_BasePacket_command_packet_tag:
    {
        messaging_CommandPacket *cp = (messaging_CommandPacket *)&packet->packet_data.command_packet;
        Serial.print("Command: ");
        switch (cp->command)
        {
        case messaging_Command_SET_NICHROME:
            Serial.print("SET_NICHROME");
            break;
        case messaging_Command_SET_MOTOR:
            Serial.print("SET_MOTOR");
            break;
        case messaging_Command_ADJUST_TRANSMIT_POWER:
            Serial.print("ADJUST_TRANSMIT_POWER");
            break;
        case messaging_Command_CUTDOWN_PARACHUTE:
            Serial.print("CUTDOWN_PARACHUTE");
            break;
        case messaging_Command_CUTDOWN_BALLOON:
            Serial.print("CUTDOWN_BALLOON");
            break;
        default:
            Serial.print("Unknown");
            break;
        }
        Serial.print(", Data:");
        if (cp->which_command_data == 0)
        {
            Serial.print(cp->command_data.boolean_data ? "true" : "false");
        }
        else if (cp->which_command_data == 1)
        {
            Serial.print("<string>");
        }
        else if (cp->which_command_data == 2)
        {
            Serial.print(cp->command_data.int_data);
        }
        else
        {
            Serial.print("N/A");
        }
        break;
    }
    case messaging_BasePacket_ack_packet_tag:
    {
        messaging_AckPacket *ap = (messaging_AckPacket *)&packet->packet_data.ack_packet;
        Serial.print("Ack: ");
        switch (ap->command)
        {
        case messaging_Command_SET_NICHROME:
            Serial.print("SET_NICHROME");
            break;
        case messaging_Command_SET_MOTOR:
            Serial.print("SET_MOTOR");
            break;
        case messaging_Command_ADJUST_TRANSMIT_POWER:
            Serial.print("ADJUST_TRANSMIT_POWER");
            break;
        case messaging_Command_CUTDOWN_PARACHUTE:
            Serial.print("CUTDOWN_PARACHUTE");
            break;
        case messaging_Command_CUTDOWN_BALLOON:
            Serial.print("CUTDOWN_BALLOON");
            break;
        default:
            Serial.print("Unknown");
            break;
        }
        Serial.print(", Nonce:");
        Serial.print(ap->your_nonce);
        break;
    }
    default:
        Serial.print("Unknown Packet Type");
        break;
    }
    Serial.println();

    // --- Line 3: GPS if applicable (only for telemetry packets) ---
    if (packet->which_packet_data == messaging_BasePacket_telemetry_packet_tag)
    {
        messaging_TelemetryPacket *tp = (messaging_TelemetryPacket *)&packet->packet_data.telemetry_packet;
        if (tp->has_gps_data)
        {
            Serial.print("GPS: Lat=");
            Serial.print(tp->gps_data.latitude / 1000000.0, 6);
            Serial.print(", Lon=");
            Serial.print(tp->gps_data.longitude / 1000000.0, 6);
            Serial.print(", Alt=");
            Serial.print(tp->gps_data.altitude / 1000000.0, 6);
        }
        else
        {
            Serial.print("GPS: N/A");
        }

        bool balloonActive = tp->motor;
        bool parachuteActive = tp->relay_1 || tp->relay_2 || tp->relay_3 || tp->relay_4;
        if (balloonActive || parachuteActive)
        {
            Serial.print(" | CUTDOWNS ACTIVE - ");
            bool printed = false;
            if (balloonActive)
            {
                Serial.print("BALLOON");
                printed = true;
            }
            if (parachuteActive)
            {
                if (printed)
                    Serial.print(", ");
                Serial.print("PARACHUTE");
            }
        }
        Serial.println();

        Serial.print("Relays: ");
        Serial.print(tp->relay_1 ? "ON " : "OFF ");
        Serial.print(tp->relay_2 ? "ON " : "OFF ");
        Serial.print(tp->relay_3 ? "ON " : "OFF ");
        Serial.print(tp->relay_4 ? "ON " : "OFF");
        Serial.print(" | Motor: ");
        Serial.println(tp->motor ? "ON" : "OFF");
        Serial.println();
        Serial.println();
    }
    else
    {
        Serial.print("GPS: N/A");
    }
    Serial.println();

    // Line 4 for relays and motors
}

const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void printBase64(const uint8_t* data, size_t len) {
  for (size_t i = 0; i < len; i += 3) {
    uint32_t triple = 0;
    int pad = 0;

    triple |= data[i] << 16;
    if (i + 1 < len) triple |= data[i + 1] << 8;
    else pad++;
    if (i + 2 < len) triple |= data[i + 2];
    else pad++;

    Serial.print(base64_chars[(triple >> 18) & 0x3F]);
    Serial.print(base64_chars[(triple >> 12) & 0x3F]);
    Serial.print(pad >= 2 ? '=' : base64_chars[(triple >> 6) & 0x3F]);
    Serial.print(pad >= 1 ? '=' : base64_chars[triple & 0x3F]);
  }
}