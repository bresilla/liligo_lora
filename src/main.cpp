#include "mesh.h"
#include <RHHardwareSPI.h>
#include <RH_RF95.h>
#include <SPI.h>

// Frequency for RF95
#define RF95_FREQ 868.0

// Adjust the following definitions depending on your board:
#if defined(__AVR_ATmega32U4__)
#define RFM95_CS 8
#define RFM95_INT 7
#define RFM95_RST 4
RH_RF95 rf95(RFM95_CS, RFM95_INT);
#elif defined(ADAFRUIT_FEATHER_M0) || defined(ADAFRUIT_FEATHER_M0_EXPRESS) || defined(ARDUINO_SAMD_FEATHER_M0)
#define RFM95_CS 8
#define RFM95_INT 3
#define RFM95_RST 4
RH_RF95 rf95(RFM95_CS, RFM95_INT);
#elif defined(ARDUINO_ADAFRUIT_FEATHER_RP2040_RFM)
#define RFM95_CS 16
#define RFM95_INT 21
#define RFM95_RST 17
// Create an RHHardwareSPI instance for SPI1
RHHardwareSPI myspi;
RH_RF95 rf95(RFM95_CS, RFM95_INT, myspi);
#endif

// Unique node ID for this device
#define MY_NODE_ID 2

// This node's simplified IPv6 address.
uint8_t myIPv6Address[IPV6_ADDR_LEN];

unsigned long previousMillis = 0;
const long interval = 5000; // Send a packet every 5 seconds

void setup() {
    // Setup radio reset pin.
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);

#if defined(ARDUINO_ADAFRUIT_FEATHER_RP2040_RFM)
    // Configure SPI1 pins for Feather RP2040 RFM.
    SPI1.setMISO(12);
    SPI1.setMOSI(11);
    SPI1.setSCK(10);
    SPI1.begin();
#endif

    Serial.begin(115200);
    while (!Serial)
        delay(1);
    delay(100);

    Serial.println("RadioHead LoRa Mesh Example");

    // Manual reset for the radio
    digitalWrite(RFM95_RST, LOW);
    delay(100);
    digitalWrite(RFM95_RST, HIGH);
    delay(100);

    // Initialize the radio.
    while (!rf95.init()) {
        Serial.println("LoRa radio init failed");
        delay(1000);
    }
    Serial.println("LoRa radio init OK!");

    if (!rf95.setFrequency(RF95_FREQ)) {
        Serial.println("setFrequency failed");
        while (1)
            ;
    }
    Serial.print("Set Freq to: ");
    Serial.println(RF95_FREQ);

    rf95.setTxPower(23, false);

    // Set the radio pointer in the mesh library.
    setRadio(&rf95);

    // Initialize this node's IPv6 address.
    initIPv6Address(MY_NODE_ID, myIPv6Address);
    Serial.print("My IPv6 Address: ");
    printIPv6Address(myIPv6Address);
    Serial.println();
}

void loop() {
    unsigned long currentMillis = millis();

    // Periodically send a broadcast packet using the mesh library.
    if (currentMillis - previousMillis > interval) {
        previousMillis = currentMillis;
        sendIPv6Packet(myIPv6Address, BROADCAST_ADDRESS, "Hello IPv6 Mesh");
    }

    // Check for incoming packets.
    if (rf95.waitAvailableTimeout(1000)) {
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);

        // Attempt to receive a packet.
        if (rf95.recv(buf, &len)) {
            if (len == sizeof(SimpleIPv6Packet)) {
                SimpleIPv6Packet incomingPacket;
                memcpy(&incomingPacket, buf, sizeof(SimpleIPv6Packet));

                Serial.print("Received packet Seq: ");
                Serial.print(incomingPacket.sequenceNumber);
                Serial.print(" From: ");
                printIPv6Address(incomingPacket.source);
                Serial.print(" To: ");
                printIPv6Address(incomingPacket.destination);
                Serial.print(" HopLimit: ");
                Serial.print(incomingPacket.hopLimit);
                Serial.print(" Msg: ");
                Serial.println(incomingPacket.payload);

                // Process the packet if it is addressed to this node or is a broadcast.
                if (ipv6Equal(incomingPacket.destination, myIPv6Address) || ipv6Equal(incomingPacket.destination, BROADCAST_ADDRESS)) {
                    Serial.println("Packet intended for this node.");
                    // Add application-specific processing here.
                } else {
                    Serial.println("Packet not for me; forwarding...");
                    forwardPacket(&incomingPacket);
                }
            } else {
                Serial.println("Received packet with unexpected length");
            }
        } else {
            Serial.println("Receive failed");
        }
    }
}
