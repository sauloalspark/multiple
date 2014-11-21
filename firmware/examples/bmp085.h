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
char              info_BMP085[256];
char              infoS_BMP085[62];

//unsigned long   BMP085interval = 10000;
//unsigned long   BMP085interval = 30000;
unsigned long     interval_BMP085 = 60000;
//unsigned long     BMP085altDiff  = 101500;
unsigned long     lastTime_BMP085;



//
// BMP085 FUNCTIONS
//

// Initialize BMP085
void Initialize_BMP085(){
	if (!bmp.begin()) {
	    RGB.control(true);
	    RGB.color(255, 255, 255);
        #ifdef BMP_SERIAL
		    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        #endif
		
		while (1) {}
	}
}

int getinfo_BMP085(String command){
    float   temp     = bmp.readTemperature();
    float   pressure = bmp.readPressure() / 100.0;
    float   alt      = bmp.readAltitude();
    //float ralt     = bmp.readAltitude(BMP085altDiff);

    //sprintf(info_BMP085 , "{\"temperature\": %.5f, \"pressure\": %.5f, \"altitude\": %.5f}", temp, pressure, alt );
    sprintf(info_BMP085 , "{\"t\":%.5f,\"p\":%.5f,\"a\":%.5f}", temp, pressure, alt );
    sprintf(infoS_BMP085, "{\"t\":%.5f,\"p\":%.5f,\"a\":%.5f}", temp, pressure, alt );

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
    
    return 1;
}

#ifdef BMP_PUBLISH
void publishinfo_BMP085(){
    if ( (millis()-lastTime_BMP085) < interval_BMP085 ) {
        return;
    }
    
    getinfo_BMP085("");
    
    Spark.publish("BMP085infoEv", infoS_BMP085, publishTTL, PRIVATE);
    
    lastTime_BMP085 = millis();
}
#endif

void updateinfo_BMP085(){
    if ( (millis()-lastTime_BMP085) < interval_BMP085 ) {
        return;
    }
    
    getinfo_BMP085("");
    
    lastTime_BMP085 = millis();
}

void InitializeApplication_BMP085() {
    Initialize_BMP085();
    
    #ifdef BMP_SERIAL
        Serial.begin(9600);
    #endif
    
    #ifdef BMP_VARIABLE
        Spark.variable( "BMP085"   , &info_BMP085   , STRING );
    #endif
    
    #ifdef BMP_FUNCTION
        Spark.function( "getBMP085",  getinfo_BMP085         );
    #endif
    
    getinfo_BMP085("");
    
    lastTime_BMP085 = millis();
}

void run_BMP085() {
    #ifdef BMP_PUBLISH
        publishinfo_BMP085();
    #else

        #ifdef BMP_VARIABLE
            updateinfo_BMP085();
        #endif

    #endif
}
