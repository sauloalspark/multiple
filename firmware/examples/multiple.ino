//#define USE_ADJDS311
#define BLINK_LED
#define USE_BMP085
//#define USE_TCS34725
#define USE_TINKER
//#define USE_TMP006




//
// GLOBAL VARIABLES
//
unsigned long     publishTTL     = 21600;



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
}

void loop() {
    #ifdef USE_ADJDS311
        runADJDS311();
    #endif

    #ifdef BLINK_LED
        runBLINK_LED();
    #endif

    #ifdef USE_BMP085
        runBMP085();
    #endif
    
    #ifdef USE_TCS34725
        runTCS34725();
    #endif
    
    #ifdef USE_TMP006
        runTMP006();
    #endif
}
