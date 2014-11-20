//
// VARIABLES FOR BLINK LED
//
unsigned long BLINK_LEDinterval    = 500;
int           BLINK_LEDstate       = LOW;
const int     BLINK_LEDPin         = D7;
unsigned long BLINK_LEDlastTime;



//
// BLINK LED FUNCTIONS
//

// Blink LED and wait for some time
void InitializeApplication_BLINK_LED() {
    pinMode(BLINK_LEDPin, OUTPUT);
}

void runBLINK_LED() {
    unsigned long currentMillis = millis();
 
    if((currentMillis - BLINK_LEDlastTime) > BLINK_LEDinterval) {
        // save the last time you blinked the LED 
        BLINK_LEDlastTime = currentMillis;   

        // if the LED is off turn it on and vice-versa:
        if (BLINK_LEDstate == LOW) {
            BLINK_LEDstate = HIGH;
        } else {
            BLINK_LEDstate = LOW;
        }

        // set the LED with the ledState of the variable:
        digitalWrite(BLINK_LEDPin, BLINK_LEDstate);
    }
}
