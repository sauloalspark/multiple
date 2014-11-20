//
// VARIABLES FOR TCS34725
//
// This #include statement was automatically added by the Spark IDE.
#include "Adafruit_TCS34725/Adafruit_TCS34725.h"

//#define TCS34725_SERIAL
//#define TCS34725_FUNCTION
//#define TCS34725_PUBLISH
#define TCS34725_VARIABLE

char              info_TCS34725[256];
#ifdef TCS34725_PUBLISH
char              infoS_TCS34725[62];
#endif
//unsigned long   BMP085interval = 30000;
unsigned long     interval_TCS34725 = 60000;
//unsigned long     BMP085altDiff  = 101500;
unsigned long     lastTime_TCS34725;

boolean commonAnode_TCS34725 = false;
Adafruit_TCS34725  tcs = Adafruit_TCS34725 (TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);


//
// TCS34725 FUNCTIONS
//

// Initialize TCS34725
void Initialize_TCS34725(){
	if (!tcs.begin()) {
	    RGB.control(true);
	    RGB.color(255, 0, 0);
        #ifdef TCS34725_SERIAL
		    Serial.println("Could not find a valid TCS34725 sensor, check wiring!");
        #endif
		
		while (1) {}
	}
}

int getinfo_TCS34725(String command){
    tcs.setInterrupt(false);      // turn on LED
    
    delay(60);  // takes 50ms to read 
     
    uint16_t clear, red, green, blue;
     
    tcs.getRawData(&red, &green, &blue, &clear);
    tcs.setInterrupt(true);  // turn off LED
    
    // Figure out some basic hex code for visualization
    uint32_t sum = clear;
    float r, g, b;
    
    r = red;   r /= sum;
    g = green; g /= sum;
    b = blue;  b /= sum;
    r *= 256; g *= 256; b *= 256;
    
    sprintf(info_TCS34725 , "{\"red\": %d, \"green\": %d, \"blue\": %d, \"clear\": %d}", (int)r, (int)g, (int)b, clear);

    #ifdef TCS34725_PUBLISH
        sprintf(infoS_TCS34725, "{\"r\": %d, \"g\": %d, \"b\": %d, \"c\": %d}", (int)r, (int)g, (int)b, clear);
    #endif

        
    #ifdef TCS34725_SERIAL
        Serial.println(info_TCS34725);
    #endif

    return 1;
}

#ifdef TCS34725_PUBLISH
void publishinfo_TCS34725(){
    if ( (millis()-lastTime_TCS34725) < interval_TCS34725 ) {
        return;
    }
    
    getinfo_TCS34725("");
    
    Spark.publish("TCS34725infoEv", infoS_TCS34725, publishTTL, PRIVATE);

    lastTime_TCS34725 = millis();
}
#endif

void updateinfo_TCS34725(){
    if ( (millis()-lastTime_TCS34725) < interval_TCS34725 ) {
        return;
    }
    
    getinfo_TCS34725("");
    
    lastTime_TCS34725 = millis();
}

void InitializeApplication_TCS34725() {
    Initialize_TCS34725();
    
    #ifdef TCS34725_SERIAL
        Serial.begin(9600);
    #endif
    
    #ifdef TCS34725_VARIABLE
        Spark.variable( "TCS34725"   , &info_TCS34725   , STRING );
    #endif
    
    #ifdef TCS34725_FUNCTION
        Spark.function( "getTCS34725",  getinfo_TCS34725         );
    #endif
    
    getinfo_TCS34725("");
    
    lastTime_TCS34725 = millis();
}

void run_TCS34725() {
    #ifdef TCS34725_PUBLISH
        publishinfo_TCS34725();
    #else

        #ifdef BMP_TCS34725
            updateinfo_TCS34725();
        #endif

    #endif
}
