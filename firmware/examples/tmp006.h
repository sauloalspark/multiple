//
// VARIABLES FOR TMP006
//

// This #include statement was automatically added by the Spark IDE.
#include "Adafruit_TMP006/Adafruit_TMP006.h"

//#define TMP006_SERIAL
//#define TMP006_FUNCTION
//#define TMP006_PUBLISH
#define TMP006_VARIABLE

char              TMP006info[256];
#ifdef TMP006_PUBLISH
char              TMP006infoS[62];
#endif
//unsigned long   BMP085interval = 30000;
unsigned long     TMP006interval = 60000;
//unsigned long     BMP085altDiff  = 101500;
unsigned long     TMP006lastTime;

uint8_t  TMP006sensor1 = 0x40; // I2C address of TMP006, can be 0x40-0x47
uint16_t TMP006samples = TMP006_CFG_16SAMPLE; // # of samples per reading, can be 1/2/4/8/16

TMP006 tmp006(TMP006sensor1);



//
// TMP006 FUNCTIONS
//

// Initialize TMP006
void InitializeTMP006(){
	if (!tmp006.begin(TMP006samples)) {
	    RGB.control(true);
	    RGB.color(0, 255, 0);
        #ifdef TMP006_SERIAL
		    Serial.println("Could not find a valid TMP006 sensor, check wiring!");
        #endif
		
		while (1) {}
	}
	
	RGB.control(false);
}

int getTMP006info(String command){
    //    Spark.publish("neoUpdate", "updating", publishTTL, PRIVATE);

    double object_temp = tmp006.readObjTempC();
    double sensor_temp = tmp006.readDieTempC();

    
    sprintf(TMP006info , "{\"Object Temperature\": %.5f, \"Sensor Temperature\": %.5f}", object_temp, sensor_temp);

    #ifdef TMP006_PUBLISH
        sprintf(TMP006infoS, "{\"temp_o\": %.5f, \"temp_s\": %.5f}", object_temp, sensor_temp);
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
void publishTMP006info(){
    if ( (millis()-TMP006lastTime) < TMP006interval ) {
        return;
    }
    
    //Spark.publish("neoUpdate"   , "publish update", publishTTL, PRIVATE);
    
    getTMP006info("");
    
    //Spark.publish("neoUpdate"   , "publish updated", publishTTL, PRIVATE);
    
    Spark.publish("TMP006infoEv", TMP006infoS, publishTTL, PRIVATE);
    //Spark.publish("BMP085infoEv", "pubishing actual results", publishTTL, PRIVATE);
    
    TMP006lastTime = millis();
}
#endif

void updateTMP006info(){
    if ( (millis()-TMP006lastTime) < TMP006interval ) {
        return;
    }
    
    //Spark.publish("neoUpdate", "function", publishTTL, PRIVATE);
    
    getTMP006info("");
    
    TMP006lastTime = millis();
}

void InitializeApplication_TMP006() {
    #ifdef TMP006_SERIAL
        Serial.begin(9600);
    #endif
    
    #ifdef TMP006_VARIABLE
        Spark.variable( "TMP006"   , &TMP006info   , STRING );
    #endif
    
    #ifdef TMP006_FUNCTION
        Spark.function( "getTMP006",  getTMP006info         );
    #endif

    getTMP006info("");

    TMP006lastTime = millis();
}

void runTMP006() {
    #ifdef TMP006_PUBLISH
        publishTMP006info();
    #else

        #ifdef TMP006_VARIABLE
            updateTMP006info();
        #endif

    #endif
}
