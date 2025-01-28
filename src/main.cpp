#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>

int counter = 0;                  // Counter variable to track the number of sent packets
unsigned long previousMillis = 0; // Variable to store the previous time for interval timing
const long interval = 1000;       // Interval time of 1 second (1000 ms)

// Screen settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

// Create an object for the SSD1306 screen
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
    Serial.begin(115200); // Initialize the serial port
    while (!Serial)
        ;

    // Initialize the screen communication
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // I2C address 0x3C for SSD1306
        Serial.println(F("Cannot initialize SSD1306 screen"));
        for (;;)
            ; // Halt the program if initialization fails
    }

    // Clear the screen buffer
    display.clearDisplay();

    // Set the text size and color
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // Set the cursor position and display initial text
    display.setCursor(10, 20); // Set cursor position

    // Initialize SPI communication for LoRa
    SPI.begin(SCK, MISO, MOSI, SS);

    // Set the pins for the LoRa transceiver module
    LoRa.setPins(SS, RST, DIO0);

    // Initialize the LoRa module at 433 MHz
    if (!LoRa.begin(8E6)) {
        display.print("LoRa Error");
        Serial.println("LoRa Error");
        display.display();
        while (1)
            ; // Halt the program if LoRa initialization fails
    } else {
        display.print("LoRa OK");
        Serial.println("LoRa OK");
        display.display();

        // Optional: Increase sensitivity and adjust settings
        // LoRa.setSpreadingFactor(12); // Maximum spreading factor
        // LoRa.setSignalBandwidth(125E3); // Bandwidth
        // LoRa.setCodingRate4(4); // Coding rate (4/8)

        // Set maximum transmission power to 20 dBm (default is 17 dBm)
        LoRa.setTxPower(20, PA_OUTPUT_PA_BOOST_PIN);
    }
}

void loop() {
    unsigned long currentMillis = millis(); // Get the current time

    // Check if the interval (1 second) has passed
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;        // Update the previous time
        LoRa.beginPacket();                    // Start a new LoRa packet
        String msg = "heja" + String(counter); // Create a message with the counter value
        LoRa.print(msg);                       // Add the message to the packet
        LoRa.endPacket();                      // End and send the packet

        Serial.println("Sent packet: " + msg); // Output the sent message to the serial monitor
        counter++;                             // Increment the counter

        // Reset the counter after 60 packets
        if (counter > 60) {
            counter = 0;
        }
        display.clearDisplay();
        display.setCursor(0, 10);
        display.print("Sent:");
        display.setCursor(50, 10);
        display.print(counter);
        display.display();
    }

    // Check if a new packet has been received
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        // A packet has been received
        Serial.print("Data: ");

        // Read the packet
        while (LoRa.available()) {
            String LoRaData = LoRa.readString(); // Read the received data as a string
            int rssi = LoRa.packetRssi();        // Read the RSSI (signal strength)

            Serial.println(LoRaData); // Output the received data to the serial monitor

            // Display the received data and RSSI on the screen
            display.clearDisplay();
            display.setCursor(0, 20);
            display.print("Recieved:");
            display.setCursor(20, 30);
            display.print("Data:");
            display.setCursor(70, 30);
            display.print(LoRaData);
            display.setCursor(20, 40);
            display.print("RSSI:");
            display.setCursor(70, 40);
            display.print(rssi);
            display.display();
        }
    }
}
