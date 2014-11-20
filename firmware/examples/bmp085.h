//
// BMP_085 VARIABLES
//

// This #include statement was automatically added by the Spark IDE.
#include "Adafruit_BMP085/Adafruit_BMP085.h"


//#define BMP_SERIAL
//#define BMP_FUNCTION
//#define BMP_PUBLISH
#define BMP_VARIABLE

/*
	Wiring
	------
	BMP085 Vcc to 3.3V
	BMP085 GND to GND
	BMP085 SCL to D1
	BMP085 SDA to D0
*/

Adafruit_BMP085 bmp;

// Publish Pressure, Altitude
char              BMP085info[256];
//#ifdef BMP_PUBLISH
char              BMP085infoS[62];
//#endif

unsigned long   BMP085interval = 10000;
//unsigned long   BMP085interval = 30000;
//unsigned long     BMP085interval = 60000;
//unsigned long     BMP085altDiff  = 101500;
unsigned long     BMP085lastTime;



//
// BMP085 FUNCTIONS
//

// Initialize BMP085
void InitializeBMP085(){
	if (!bmp.begin()) {
	    RGB.control(true);
	    RGB.color(255, 255, 255);
        #ifdef BMP_SERIAL
		    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        #endif
		
		while (1) {}
	}
	
	RGB.control(false);
}

int getBMP085info(String command){
    //    Spark.publish("neoUpdate", "updating", publishTTL, PRIVATE);
        
    float   temp     = bmp.readTemperature();
    int32_t pressure = bmp.readPressure();
    float   alt      = bmp.readAltitude();
    //float ralt     = bmp.readAltitude(BMP085altDiff);


    sprintf(BMP085info , "{\"temperature\": %.5f, \"pressure\": %.5f, \"altitude\": %.5f}", temp, pressure/100.0, alt );
    //sprintf(BMP085info , "{\"temperature\": %.5f, \"pressure\": %.5f, \"altitude\": %.5f}", bmp.readTemperature(), bmp.readPressure()/100.0, bmp.readAltitude() );

    //#ifdef BMP_PUBLISH
        sprintf(BMP085infoS, "{\"t\":%.5f,\"p\":%.5f,\"a\":%.5f,\"ra\":%.5f}", temp, pressure/100.0, alt );
    //#endif

    #ifdef BMP_SERIAL
        Serial.print("Temperature = ");
        Serial.print(temp);
        Serial.println(" *C");
        
        Serial.print("Pressure = ");
        Serial.print(pressure);
        Serial.println(" Pa");
        
        // Calculate altitude assuming 'standard' barometric
        // pressure of 1013.25 millibar = 101325 Pascal
        Serial.print("Altitude = ");
        Serial.print(alt);
        Serial.println(" meters");
    
        // you can get a more precise measurement of altitude
        // if you know the current sea level pressure which will
        // vary with weather and such. If it is 1015 millibars
        // that is equal to 101500 Pascals.
        //Serial.print("Real altitude = ");
        //Serial.print(ralt);
        //Serial.println(" meters");
    #endif
    //Spark.publish("neoUpdate", "updated" , publishTTL, PRIVATE);

    return 1;
}

#ifdef BMP_PUBLISH
void publishBMP085info(){
    if ( (millis()-BMP085lastTime) < BMP085interval ) {
        return;
    }
    
    //Spark.publish("neoUpdate"   , "publish update", publishTTL, PRIVATE);
    
    getBMP085info("");
    
    //Spark.publish("neoUpdate"   , "publish updated", publishTTL, PRIVATE);
    
    Spark.publish("BMP085infoEv", BMP085infoS, publishTTL, PRIVATE);
    //Spark.publish("BMP085infoEv", "pubishing actual results", publishTTL, PRIVATE);
    
    BMP085lastTime = millis();
}
#endif

void updateBMP085info(){
    if ( (millis()-BMP085lastTime) < BMP085interval ) {
        return;
    }
    
    //Spark.publish("neoUpdate", "function", publishTTL, PRIVATE);
    
    getBMP085info("");
    
    BMP085lastTime = millis();
}

void InitializeApplication_BMP085() {
    #ifdef BMP_SERIAL
        Serial.begin(9600);
    #endif
    
    #ifdef BMP_VARIABLE
        Spark.variable( "BMP085"   , &BMP085info   , STRING );
    #endif
    
    #ifdef BMP_FUNCTION
        Spark.function( "getBMP085",  getBMP085info         );
    #endif
    
    getBMP085info("");
    
    BMP085lastTime = millis();
}

void runBMP085() {
    #ifdef BMP_PUBLISH
        publishBMP085info();
    #else

        #ifdef BMP_VARIABLE
            updateBMP085info();
        #endif

    #endif
}
