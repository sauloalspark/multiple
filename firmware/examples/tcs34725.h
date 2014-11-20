//
// VARIABLES FOR TCS34725
//
// This #include statement was automatically added by the Spark IDE.
#include "Adafruit_TCS34725/Adafruit_TCS34725.h"

//#define TCS34725_SERIAL
//#define TCS34725_FUNCTION
//#define TCS34725_PUBLISH
#define TCS34725_VARIABLE

char              TCS34725info[256];
#ifdef TCS34725_PUBLISH
char              TCS34725infoS[62];
#endif
//unsigned long   BMP085interval = 30000;
unsigned long     TCS34725interval = 60000;
//unsigned long     BMP085altDiff  = 101500;
unsigned long     TCS34725lastTime;

boolean TCS34725_commonAnode = false;
Adafruit_TCS34725  tcs = Adafruit_TCS34725 (TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);


//
// TCS34725 FUNCTIONS
//

// Initialize TCS34725
void InitializeTCS34725(){
	if (!tcs.begin()) {
	    RGB.control(true);
	    RGB.color(255, 0, 0);
        #ifdef TCS34725_SERIAL
		    Serial.println("Could not find a valid TCS34725 sensor, check wiring!");
        #endif
		
		while (1) {}
	}
	
	RGB.control(false);
}

int getTCS34725info(String command){
    //    Spark.publish("neoUpdate", "updating", publishTTL, PRIVATE);

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
    
    sprintf(TCS34725info , "{\"red\": %d, \"green\": %d, \"blue\": %d, \"clear\": %d}", (int)r, (int)g, (int)b, clear);

    #ifdef TCS34725_PUBLISH
        sprintf(TCS34725infoS, "{\"r\": %d, \"g\": %d, \"b\": %d, \"c\": %d}", (int)r, (int)g, (int)b, clear);
    #endif

        
    #ifdef TCS34725_SERIAL
        Serial.println(TCS34725info);
    #endif

    return 1;
}

#ifdef TCS34725_PUBLISH
void publishTCS34725info(){
    if ( (millis()-TCS34725lastTime) < TCS34725interval ) {
        return;
    }
    
    //Spark.publish("neoUpdate"   , "publish update", publishTTL, PRIVATE);
    
    getTCS34725info("");
    
    //Spark.publish("neoUpdate"   , "publish updated", publishTTL, PRIVATE);
    
    Spark.publish("TCS34725infoEv", TCS34725infoS, publishTTL, PRIVATE);
    //Spark.publish("BMP085infoEv", "pubishing actual results", publishTTL, PRIVATE);
    
    TCS34725lastTime = millis();
}
#endif

void updateTCS34725info(){
    if ( (millis()-TCS34725lastTime) < TCS34725interval ) {
        return;
    }
    
    //Spark.publish("neoUpdate", "function", publishTTL, PRIVATE);
    
    getTCS34725info("");
    
    TCS34725lastTime = millis();
}

void InitializeApplication_TCS34725() {
    #ifdef TCS34725_SERIAL
        Serial.begin(9600);
    #endif
    
    #ifdef TCS34725_VARIABLE
        Spark.variable( "TCS34725"   , &TCS34725info   , STRING );
    #endif
    
    #ifdef TCS34725_FUNCTION
        Spark.function( "getTCS34725",  getTCS34725info         );
    #endif
    
    getTCS34725info("");
    
    TCS34725lastTime = millis();
}

void runTCS34725() {
    #ifdef TCS34725_PUBLISH
        publishTCS34725info();
    #else

        #ifdef BMP_TCS34725
            updateTCS34725info();
        #endif

    #endif
}
