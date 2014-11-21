#define USE_ADJDS311
#define BLINK_LED
#define USE_BMP085
#define USE_TCS34725
#define USE_TINKER
#define USE_TMP006


#ifdef USE_TINKER
    #include "tinker.h"
#endif

#ifdef USE_ADJDS311
    #include "adjds311.h"
#endif

#ifdef BLINK_LED
    #include "blink_led.h"
#endif

#ifdef USE_BMP085
    #include "bmp085.h"    
#endif

#ifdef USE_TCS34725
    #include "tcs34725.h"
#endif

#ifdef USE_TMP006
    #include "tmp006.h"
#endif


//STRING (maximum string size is 622 bytes)
unsigned long     publishTTL     = 21600;



//#define ALL_FUNCTION
//#define ALL_PUBLISH
#define ALL_VARIABLE
char              info_ALL[ 621];
unsigned long     interval_ALL = 60000;
unsigned long     lastTime_ALL;


int getinfo_ALL(String command) {
    //char              info_ALLt[ 621];
    sprintf(info_ALL, "%s", "{");

    #ifdef USE_ADJDS311
        //sprintf(info_ALL, "\"ADJDS311\":%s", infoS_ADJDS311);
        strcat(info_ALL, "\"ADJDS311\":");
        strcat(info_ALL, infoS_ADJDS311);
        strcat(info_ALL, ",");
    #endif

    #ifdef USE_BMP085
        //sprintf(info_ALL, "\"BMP085\":%s,%s", infoS_BMP085, info_ALL);
        strcat(info_ALL, "\"BMP085\":");
        strcat(info_ALL, infoS_BMP085);
        strcat(info_ALL, ",");
    #endif
    
    #ifdef USE_TCS34725
        //sprintf(info_ALLt, "\"TCS34725\": %s, %s\x0", infoS_TCS34725, info_ALLt);
        strcat(info_ALL, "\"TCS34725\":");
        strcat(info_ALL, infoS_TCS34725);
        strcat(info_ALL, ",");
    #endif

    #ifdef USE_TMP006
        //sprintf(info_ALLt, "\"TMP006\": %s, %s\x0", infoS_TMP006, info_ALLt);
        strcat(info_ALL, "\"TMP006\":");
        strcat(info_ALL, infoS_TMP006);
        strcat(info_ALL, ",");
    #endif

    //sprintf(info_ALL, "{%s\"time\": %lu}\x0", info_ALLt, millis());

    //sprintf(info_ALL, "{\"ADJDS311\":%s,\"BMP085\":%s,\"TCS34725\":%s,\"TMP006\":%s,\"time\":%lu}\x0", infoS_ADJDS311, infoS_BMP085, infoS_TCS34725, infoS_TMP006, millis());

    //sprintf(info_ALL , "%s", info_ALLt);


    char    t[ 40];
    sprintf(t, "\"time\":%lu}", millis());

    strcat(info_ALL, t  );

    return 1;
}

void InitializeApplication_ALL() {
    #ifdef ALL_VARIABLE
        Spark.variable( "ALL"   , &info_ALL   , STRING );
    #endif
    
    #ifdef ALL_FUNCTION
        Spark.function( "getALL",  getinfo_ALL         );
    #endif
    
    getinfo_ALL("");
}

void updateinfo_ALL() {
    if ( (millis()-lastTime_ALL) < interval_ALL ) {
        return;
    }

    getinfo_ALL("");

    lastTime_ALL = millis();
}

void run_ALL() {
    updateinfo_ALL();
}









void setup() {
    #ifdef USE_TINKER
        InitializeApplication_Tinker();
    #endif

    #ifdef USE_ADJDS311
        InitializeApplication_ADJDS311();
    #endif
    
    #ifdef BLINK_LED
        InitializeApplication_BLINK_LED();
    #endif

    #ifdef USE_BMP085
        InitializeApplication_BMP085();
    #endif
    
    #ifdef USE_TCS34725
        InitializeApplication_TCS34725();
    #endif

    #ifdef USE_TMP006
        InitializeApplication_TMP006();
    #endif

    InitializeApplication_ALL();
}

void loop() {
    #ifdef USE_ADJDS311
        run_ADJDS311();
    #endif

    #ifdef BLINK_LED
        run_BLINK_LED();
    #endif

    #ifdef USE_BMP085
        run_BMP085();
    #endif
    
    #ifdef USE_TCS34725
        run_TCS34725();
    #endif
    
    #ifdef USE_TMP006
        run_TMP006();
    #endif

    run_ALL();
}
