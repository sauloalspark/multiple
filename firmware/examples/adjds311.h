//
// ADJDS311 VARIABLES
//


// This #include statement was automatically added by the Spark IDE.
#include "Sparkfun_ADJD-S311/Sparkfun_ADJD-S311.h"

//#define ADJDS311_SERIAL
//#define ADJDS311_FUNCTION
//#define ADJDS311_PUBLISH
#define ADJDS311_VARIABLE


char              ADJDS311info[256];
#ifdef ADJDS311_PUBLISH
char              ADJDS311infoS[62];
#endif
//unsigned long   BMP085interval = 30000;
unsigned long     ADJDS311interval = 60000;
//unsigned long     BMP085altDiff  = 101500;
unsigned long     ADJDS311lastTime;

int ADJDS311_sensorLed_pin = 2; //LED on the ADJDS-311
ADJDS311 adjds311(ADJDS311_sensorLed_pin);

//if using an RGB LED (Needs PWM Pins)
int ADJDS311_redPin   = 3;
int ADJDS311_greenPin = 5;
int ADJDS311_bluePin  = 6;


//
// ADJDS311 FUNCTIONS
//

// Initialize ADJDS311
void InitializeADJDS311(){
	if (!adjds311.begin()) {
	    RGB.control(true);
	    RGB.color(0, 0, 255);
        #ifdef ADJDS311_SERIAL
		    Serial.println("Could not find a valid ADJDS311 sensor, check wiring!");
        #endif
		
		while (1) {}
	}
	
	RGB.control(false);
}


int getADJDS311info(String command){
    //    Spark.publish("neoUpdate", "updating", publishTTL, PRIVATE);

    adjds311.ledOn(); //turn LED on
    
    //Calibrate white 
    //Need to hold white card in front (1-3mm) of it to calibrate from
    adjds311.calibrate(); 
    
    
    RGBC color = adjds311.read(); //read the color
    
    adjds311.ledOff(); //turn LED on
 
    int red   = color.red;
    int green = color.green;
    int blue  = color.blue;
    int clear = color.clear;
 
    sprintf(ADJDS311info , "{\"red\": %d, \"green\": %d, \"blue\": %d, \"clear\": %d}", red, green, blue, clear);

    #ifdef ADJDS311_PUBLISH
        sprintf(ADJDS311infoS, "{\"r\": %d, \"g\": %d, \"b\": %d, \"c\": %d}", red, green, blue, clear);
    #endif

        
    #ifdef ADJDS311_SERIAL
        Serial.println(ADJDS311info);
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
void publishADJDS311info(){
    if ( (millis()-ADJDS311lastTime) < ADJDS311interval ) {
        return;
    }
    
    //Spark.publish("neoUpdate"   , "publish update", publishTTL, PRIVATE);
    
    getADJDS311info("");
    
    //Spark.publish("neoUpdate"   , "publish updated", publishTTL, PRIVATE);
    
    Spark.publish("ADJDS311infoEv", ADJDS311infoS, publishTTL, PRIVATE);
    //Spark.publish("BMP085infoEv", "pubishing actual results", publishTTL, PRIVATE);
    
    ADJDS311lastTime = millis();
}
#endif

void updateADJDS311info(){
    if ( (millis()-ADJDS311lastTime) < ADJDS311interval ) {
        return;
    }
    
    //Spark.publish("neoUpdate", "function", publishTTL, PRIVATE);
    
    getADJDS311info("");
    
    ADJDS311lastTime = millis();
}

void InitializeApplication_ADJDS311() {
    #ifdef ADJDS311_SERIAL
        Serial.begin(9600);
    #endif
    
    #ifdef ADJDS311_VARIABLE
        Spark.variable( "ADJDS311"   , &ADJDS311info   , STRING );
    #endif
    
    #ifdef ADJDS311_FUNCTION
        Spark.function( "getADJDS311",  getADJDS311info         );
    #endif
    
    getADJDS311info("");
    
    ADJDS311lastTime = millis();
}

void runADJDS311() {
    #ifdef ADJDS311_PUBLISH
        publishADJDS311info();
    #else

        #ifdef ADJDS311_VARIABLE
            updateADJDS311info();
        #endif

    #endif
}
