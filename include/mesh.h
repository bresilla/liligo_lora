#ifndef MESH_H
#define MESH_H

#include <Arduino.h>
#include <RH_RF95.h>
#include <stdint.h>
#include <string.h>

#define IPV6_ADDR_LEN 16

// Define a broadcast IPv6 address (all bytes set to 0xFF)
extern const uint8_t BROADCAST_ADDRESS[IPV6_ADDR_LEN];

// Simple IPv6-like packet structure
typedef struct {
    uint8_t version;                    // Always 6 for IPv6
    uint8_t hopLimit;                   // Hop Limit (TTL)
    uint16_t sequenceNumber;            // Packet sequence number
    uint8_t source[IPV6_ADDR_LEN];      // Source IPv6 address
    uint8_t destination[IPV6_ADDR_LEN]; // Destination IPv6 address
    char payload[20];                   // Message payload (adjust size as needed)
} SimpleIPv6Packet;

// Sets the pointer to the RH_RF95 radio instance used by the mesh functions.
void setRadio(RH_RF95 *radio);

// Initializes a simplified IPv6 address for a node.
// The address is zeroed except for the last byte which holds the nodeId.
void initIPv6Address(uint8_t nodeId, uint8_t *addr);

// Prints an IPv6 address (in hexadecimal format) to Serial.
void printIPv6Address(const uint8_t *addr);

// Compares two IPv6 addresses.
bool ipv6Equal(const uint8_t *addr1, const uint8_t *addr2);

// Sends an IPv6 packet using the radio instance set via setRadio().
// 'srcAddr' is the sender’s address, and 'destAddr' may be a unicast or the BROADCAST_ADDRESS.
void sendIPv6Packet(const uint8_t *srcAddr, const uint8_t *destAddr, const char *message);

// Forwards a received packet by decrementing its hop limit and retransmitting.
void forwardPacket(SimpleIPv6Packet *packet);

#endif
