#include "mesh.h"

// Define the broadcast address as all 0xFF.
const uint8_t BROADCAST_ADDRESS[IPV6_ADDR_LEN] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Global sequence number for outgoing packets.
static uint16_t globalSequenceNumber = 0;

// Pointer to the radio instance used by the mesh functions.
static RH_RF95 *meshRadio = nullptr;

void setRadio(RH_RF95 *radio) { meshRadio = radio; }

void initIPv6Address(uint8_t nodeId, uint8_t *addr) {
    memset(addr, 0, IPV6_ADDR_LEN);
    addr[IPV6_ADDR_LEN - 1] = nodeId;
}

void printIPv6Address(const uint8_t *addr) {
    for (int i = 0; i < IPV6_ADDR_LEN; i++) {
        if (addr[i] < 16)
            Serial.print("0");
        Serial.print(addr[i], HEX);
        if (i < IPV6_ADDR_LEN - 1)
            Serial.print(":");
    }
}

bool ipv6Equal(const uint8_t *addr1, const uint8_t *addr2) { return (memcmp(addr1, addr2, IPV6_ADDR_LEN) == 0); }

void sendIPv6Packet(const uint8_t *srcAddr, const uint8_t *destAddr, const char *message) {
    if (!meshRadio) {
        Serial.println("Radio not set in mesh library!");
        return;
    }

    SimpleIPv6Packet packet;
    packet.version = 6;   // IPv6 version
    packet.hopLimit = 10; // Initial hop limit
    packet.sequenceNumber = globalSequenceNumber++;
    memcpy(packet.source, srcAddr, IPV6_ADDR_LEN);
    memcpy(packet.destination, destAddr, IPV6_ADDR_LEN);
    memset(packet.payload, 0, sizeof(packet.payload));
    strncpy(packet.payload, message, sizeof(packet.payload) - 1);

    // Send the packet using the RadioHead interface.
    meshRadio->send((uint8_t *)&packet, sizeof(packet));
    meshRadio->waitPacketSent();

    Serial.print("Sent packet Seq: ");
    Serial.print(packet.sequenceNumber);
    Serial.print(" From: ");
    printIPv6Address(packet.source);
    Serial.print(" To: ");
    printIPv6Address(packet.destination);
    Serial.print(" Msg: ");
    Serial.println(packet.payload);
}

void forwardPacket(SimpleIPv6Packet *packet) {
    if (!meshRadio) {
        Serial.println("Radio not set in mesh library!");
        return;
    }

    if (packet->hopLimit > 0) {
        packet->hopLimit--;
        meshRadio->send((uint8_t *)packet, sizeof(SimpleIPv6Packet));
        meshRadio->waitPacketSent();

        Serial.print("Forwarded packet Seq: ");
        Serial.println(packet->sequenceNumber);
    }
}
