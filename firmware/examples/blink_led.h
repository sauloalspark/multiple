//
// VARIABLES FOR BLINK LED
//
unsigned long interval_LED_BLINK    = 500;
int           state_LED_BLINK       = LOW;
const int     pin_LED_BLINK         = D7;
unsigned long lastTime_LED_BLINK;



//
// BLINK LED FUNCTIONS
//

// Blink LED and wait for some time
void InitializeApplication_BLINK_LED() {
    pinMode(pin_LED_BLINK, OUTPUT);
}

void run_BLINK_LED() {
    unsigned long currentMillis = millis();
 
    if((currentMillis - lastTime_LED_BLINK) > interval_LED_BLINK) {
        // save the last time you blinked the LED 
        lastTime_LED_BLINK = currentMillis;   

        // if the LED is off turn it on and vice-versa:
        if (state_LED_BLINK == LOW) {
            state_LED_BLINK = HIGH;
        } else {
            state_LED_BLINK = LOW;
        }

        // set the LED with the ledState of the variable:
        digitalWrite(pin_LED_BLINK, state_LED_BLINK);
    }
}
