//
// ADJDS311 VARIABLES
//


// This #include statement was automatically added by the Spark IDE.
#include "Sparkfun_ADJD-S311/Sparkfun_ADJD-S311.h"

//#define ADJDS311_SERIAL
//#define ADJDS311_FUNCTION
//#define ADJDS311_PUBLISH
#define ADJDS311_VARIABLE


char              info_ADJDS311[256];
char              infoS_ADJDS311[62];
//unsigned long   BMP085interval = 30000;
unsigned long     interval_ADJDS311 = 60000;
//unsigned long     BMP085altDiff  = 101500;
unsigned long     lastTime_ADJDS311;

int sensorLed_pin_ADJDS311 = 2; //LED on the ADJDS-311
ADJDS311 adjds311(sensorLed_pin_ADJDS311);

//if using an RGB LED (Needs PWM Pins)
int redPin_ADJDS311   = 3;
int greenPin_ADJDS311 = 5;
int bluePin_ADJDS311  = 6;


//
// ADJDS311 FUNCTIONS
//

// Initialize ADJDS311
void Initialize_ADJDS311(){
	if (!adjds311.begin()) {
	    RGB.control(true);
	    RGB.color(0, 0, 255);
        #ifdef ADJDS311_SERIAL
		    Serial.println("Could not find a valid ADJDS311 sensor, check wiring!");
        #endif
		
		while (1) {}
	}
}


int getinfo_ADJDS311(String command){
    adjds311.ledOn(); //turn LED on
    
    //Calibrate white 
    //Need to hold white card in front (1-3mm) of it to calibrate from
    adjds311.calibrate(); 

    RGBC color = adjds311.read(); //read the color
    
    adjds311.ledOff(); //turn LED on
 
    int red   = map(color.red  , 0, 1024, 0, 255);
    int green = map(color.green, 0, 1024, 0, 255);
    int blue  = map(color.blue , 0, 1024, 0, 255);
    int clear = map(color.clear, 0, 1024, 0, 255);
 
    //sprintf(info_ADJDS311 , "{\"red\": %d, \"green\": %d, \"blue\": %d, \"clear\": %d}", red, green, blue, clear);
    sprintf(info_ADJDS311 , "{\"r\":%d,\"g\":%d,\"b\":%d,\"c\":%d}", red, green, blue, clear);
    sprintf(infoS_ADJDS311, "{\"r\":%d,\"g\":%d,\"b\":%d,\"c\":%d}", red, green, blue, clear);
    
    #ifdef ADJDS311_SERIAL
        Serial.println(info_ADJDS311);
    #endif

     
    //lightLED(color); //send color to the LED
    //void lightLED(RGBC color){
        //RGBC is an array of red/green/blue/clear readings 
        //Take a RGBC, and try to reproduce it on an RGB LED
        //This does not work very well as is because of how colors/our eyes work
        //analogWrite(redPin, map(color.red, 0, 1024, 0, 255));
        //analogWrite(greenPin, map(color.green, 0, 1024, 0, 255));
        //analogWrite(bluePin, map(color.blue, 0, 1024, 0, 255));
    //}


    return 1;
}

#ifdef ADJDS311_PUBLISH
void publishinfo_ADJDS311(){
    if ( (millis()-lastTime_ADJDS311) < interval_ADJDS311 ) {
        return;
    }

    getinfo_ADJDS311("");

    Spark.publish("ADJDS311infoEv", infoS_ADJDS311, publishTTL, PRIVATE);

    lastTime_ADJDS311 = millis();
}
#endif

void updateinfo_ADJDS311(){
    if ( (millis()-lastTime_ADJDS311) < interval_ADJDS311 ) {
        return;
    }
    
    //Spark.publish("neoUpdate", "function", publishTTL, PRIVATE);
    
    getinfo_ADJDS311("");
    
    lastTime_ADJDS311 = millis();
}

void InitializeApplication_ADJDS311() {
    Initialize_ADJDS311();
    
    #ifdef ADJDS311_SERIAL
        Serial.begin(9600);
    #endif
    
    #ifdef ADJDS311_VARIABLE
        Spark.variable( "ADJDS311"   , &info_ADJDS311   , STRING );
    #endif
    
    #ifdef ADJDS311_FUNCTION
        Spark.function( "getADJDS311",  getinfo_ADJDS311         );
    #endif
    
    getinfo_ADJDS311("");
    
    lastTime_ADJDS311 = millis();
}

void run_ADJDS311() {
    #ifdef ADJDS311_PUBLISH
        publishinfo_ADJDS311();
    #else

        #ifdef ADJDS311_VARIABLE
            updateinfo_ADJDS311();
        #endif

    #endif
}
