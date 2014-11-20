//
// VARIABLES FOR TMP006
//

// This #include statement was automatically added by the Spark IDE.
#include "Adafruit_TMP006/Adafruit_TMP006.h"

//#define TMP006_SERIAL
//#define TMP006_FUNCTION
//#define TMP006_PUBLISH
#define TMP006_VARIABLE

char              info_TMP006[256];
#ifdef TMP006_PUBLISH
char              infoS_TMP006[62];
#endif
//unsigned long   BMP085interval = 30000;
unsigned long     interval_TMP006 = 60000;
//unsigned long     BMP085altDiff  = 101500;
unsigned long     lastTime_TMP006;

uint8_t  sensor1_TMP006 = 0x40; // I2C address of TMP006, can be 0x40-0x47
uint16_t samples_TMP006 = TMP006_CFG_16SAMPLE; // # of samples per reading, can be 1/2/4/8/16

TMP006 tmp006(sensor1_TMP006);



//
// TMP006 FUNCTIONS
//

// Initialize TMP006
void Initialize_TMP006(){
	if (!tmp006.begin(samples_TMP006)) {
	    RGB.control(true);
	    RGB.color(0, 255, 0);
        #ifdef TMP006_SERIAL
		    Serial.println("Could not find a valid TMP006 sensor, check wiring!");
        #endif
		
		while (1) {}
	}
}

int getinfo_TMP006(String command){
    double object_temp = tmp006.readObjTempC();
    double sensor_temp = tmp006.readDieTempC();

    
    sprintf(info_TMP006 , "{\"Object Temperature\": %.5f, \"Sensor Temperature\": %.5f}", object_temp, sensor_temp);

    #ifdef TMP006_PUBLISH
        sprintf(infoS_TMP006, "{\"temp_o\": %.5f, \"temp_s\": %.5f}", object_temp, sensor_temp);
    #endif

    #ifdef TMP006_SERIAL
        Serial.print("Object Temperature: ");
        Serial.print(object_temp); 
        Serial.println("*C");

        Serial.print("Sensor Temperature: ");
        Serial.print(sensor_temp); 
        Serial.println("*C");
    #endif

    return 1;
}

#ifdef TMP006_PUBLISH
void publishinfo_TMP006(){
    if ( (millis()-lastTime_TMP006) < interval_TMP006 ) {
        return;
    }
    
    getinfo_TMP006("");

    Spark.publish("TMP006infoEv", infoS_TMP006, publishTTL, PRIVATE);

    lastTime_TMP006 = millis();
}
#endif

void updateinfo_TMP006(){
    if ( (millis()-lastTime_TMP006) < interval_TMP006 ) {
        return;
    }

    getinfo_TMP006("");
    
    lastTime_TMP006 = millis();
}

void InitializeApplication_TMP006() {
    Initialize_TMP006();
    
    #ifdef TMP006_SERIAL
        Serial.begin(9600);
    #endif
    
    #ifdef TMP006_VARIABLE
        Spark.variable( "TMP006"   , &info_TMP006   , STRING );
    #endif
    
    #ifdef TMP006_FUNCTION
        Spark.function( "getTMP006",  getinfo_TMP006         );
    #endif

    getinfo_TMP006("");

    lastTime_TMP006 = millis();
}

void run_TMP006() {
    #ifdef TMP006_PUBLISH
        publishinfo_TMP006();
    #else

        #ifdef TMP006_VARIABLE
            updateinfo_TMP006();
        #endif

    #endif
}
