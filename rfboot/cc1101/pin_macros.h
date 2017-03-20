// MACROS FOR EASY PIN HANDLING FOR ATMEL GCC-AVR
//these macros are used indirectly by other macros , mainly for string concatination

#ifndef PIN_MACROS_H
#include <avr/io.h>

#define _SET(type,name,bit)          type ## name  |= _BV(bit)
#define _CLEAR(type,name,bit)        type ## name  &= ~ _BV(bit)
#define _TOGGLE(type,name,bit)       type ## name  ^= _BV(bit)
#define _GET(type,name,bit)          ((type ## name >> bit) &  1)
#define _PUT(type,name,bit,value)    type ## name = ( type ## name & ( ~ _BV(bit)) ) | ( ( 1 & (unsigned char)value ) << bit )

//these macros are used by end user
#define OUTPUT(pin)         _SET(DDR,pin)
#define INPUT(pin)          _CLEAR(DDR,pin)
#define HIGH(pin)           _SET(PORT,pin)
#define LOW(pin)            _CLEAR(PORT,pin)
#define TOGGLE(pin)         _TOGGLE(PORT,pin)
#define READ(pin)           _GET(PIN,pin)

/*
    BASIC STAMPS STYLE COMMANDS FOR ATMEL GCC-AVR

    Usage Example:
    ———————————————–
    #define pinLed B,5  //define pins like this

    OUTPUT(pinLed);       //type fixed
    //OUTPUT(pinLED);     //compiles as DDRB |= (1<<5);
    HIGH(pinLed);         //compiles as PORTB |= (1<<5);
    ———————————————–
*/

#endif
