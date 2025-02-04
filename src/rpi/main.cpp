/* #include <Adafruit_GFX.h> */
/* #include <Adafruit_SSD1306.h> */
#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>

/* int counter = 0;                  // Counter variable to track the number of sent packets */
/* unsigned long previousMillis = 0; // Variable to store the previous time for interval timing */
/* const long interval = 1000;       // Interval time of 1 second (1000 ms) */
/**/
/* // Define the pins used by the LoRa transceiver module */
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

void setup() {
    /* Serial.begin(115200); // Initialize the serial port */
    /* while (!Serial) */
    /* ; */
    /**/
    /* // Initialize SPI communication for LoRa */
    /* SPI.begin(SCK, MISO, MOSI, SS); */
    /**/
    /* // Set the pins for the LoRa transceiver module */
    /* LoRa.setPins(SS, RST, DIO0); */
    /**/
    /* // Initialize the LoRa module at 868 MHz */
    /* if (!LoRa.begin(868E6)) { */
    /*     Serial.println("LoRa Error"); */
    /*     while (1) */
    /*         ; // Halt the program if LoRa initialization fails */
    /* } else { */
    /*     Serial.println("LoRa OK"); */
    /**/
    /*     // Optional: Increase sensitivity and adjust settings */
    /*     // LoRa.setSpreadingFactor(12); // Maximum spreading factor */
    /*     // LoRa.setSignalBandwidth(125E3); // Bandwidth */
    /*     // LoRa.setCodingRate4(4); // Coding rate (4/8) */
    /**/
    /*     // Set maximum transmission power to 20 dBm (default is 17 dBm) */
    /*     LoRa.setTxPower(20, PA_OUTPUT_PA_BOOST_PIN); */
    /* } */
}

void loop() {
    /* unsigned long currentMillis = millis(); // Get the current time */

    /* // Check if the interval (1 second) has passed */
    /* if (currentMillis - previousMillis >= interval) { */
    /*     previousMillis = currentMillis; // Update the previous time */
    /*     LoRa.beginPacket();             // Start a new LoRa packet */
    /*     String msg = String(counter);   // Create a message with the counter value */
    /*     LoRa.print(msg);                // Add the message to the packet */
    /*     LoRa.endPacket();               // End and send the packet */
    /**/
    /*     Serial.println("Sent packet: " + msg); // Output the sent message to the serial monitor */
    /*     counter++;                             // Increment the counter */
    /* } */
    /**/
    /* // Check if a new packet has been received */
    /* int packetSize = LoRa.parsePacket(); */
    /* if (packetSize) { */
    /*     // A packet has been received */
    /*     Serial.print("Data: "); */
    /**/
    /*     // Read the packet */
    /*     while (LoRa.available()) { */
    /*         String LoRaData = LoRa.readString(); // Read the received data as a string */
    /*         int rssi = LoRa.packetRssi();        // Read the RSSI (signal strength) */
    /*         Serial.println(LoRaData);            // Output the received data to the serial monitor */
    /*     } */
    /* } */
}
