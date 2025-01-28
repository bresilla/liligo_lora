#include "Arduino.h"
#include "Wire.h"

// put function declarations here:
int myFunction(int x, int y) { return x + y; }

void setup() {
    // put your setup code here, to run once:
    int result = myFunction(2, 3);
}

void loop() {
    // put your main code here, to run repeatedly:
}
