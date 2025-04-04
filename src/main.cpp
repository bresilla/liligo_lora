#include <RHHardwareSPI.h>
#include <RH_RF95.h>
#include <SPI.h>

#define RF95_FREQ 868.0

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

// Singleton instance of the radio driver

void setup() {
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);

#if defined(ARDUINO_ADAFRUIT_FEATHER_RP2040_RFM)
    SPI1.setMISO(12);
    SPI1.setMOSI(11);
    SPI1.setSCK(10);
    SPI1.begin();
#endif

    Serial.begin(115200);
    while (!Serial)
        delay(1);
    delay(100);

    Serial.println(RFM95_INT);

    Serial.println("Feather LoRa TX Test!");

    // manual reset
    digitalWrite(RFM95_RST, LOW);
    delay(100);
    digitalWrite(RFM95_RST, HIGH);
    delay(100);

    while (!rf95.init()) {
        Serial.println("LoRa radio init failed");
        while (1) {
            printf("LoRa radio init failed");
        }
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
}

int16_t packetnum = 0; // packet counter, we increment per xmission

void loop() {
    delay(1000);                       // Wait 1 second between transmits, could also 'sleep' here!
    Serial.println("Transmitting..."); // Send a message to rf95_server

    char radiopacket[20] = "Hello World #      ";
    itoa(packetnum++, radiopacket + 13, 10);
    Serial.print("Sending ");
    Serial.println(radiopacket);
    radiopacket[19] = 0;

    Serial.println("Sending...");
    delay(10);
    rf95.send((uint8_t *)radiopacket, 20);

    Serial.println("Waiting for packet to complete...");
    delay(10);
    rf95.waitPacketSent();
    // Now wait for a reply
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    Serial.println("Waiting for reply...");
    if (rf95.waitAvailableTimeout(1000)) {
        // Should be a reply message for us now
        if (rf95.recv(buf, &len)) {
            Serial.print("Got reply: ");
            Serial.println((char *)buf);
            Serial.print("RSSI: ");
            Serial.println(rf95.lastRssi(), DEC);
        } else {
            Serial.println("Receive failed");
        }
    } else {
        Serial.println("No reply, is there a listener around?");
    }
    Serial.println("\n");
}
