#ifndef KEYPAD_H
#define	KEYPAD_H

#include <stdint.h>
#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
    #ifndef KEYPAD_TYPE
    #define KEYPAD_TYPE uint8_t
    #endif
    
    // Forward declaration of the struct type
    typedef struct KeypadStruct Keypad;
    
    // typedefs for struct function pointers
    typedef void (*KeypadStateFn)(Keypad *me);
    typedef KEYPAD_TYPE (*KeypadPhyStateFn)(void);
    typedef void (*KeyPressFn)(KEYPAD_TYPE keyPressed);
    
    struct KeypadStruct {
        KeypadStateFn currentStateFn;
        KeypadPhyStateFn getKeypadPhysicalStateFn;
        KeyPressFn keyPressCallbackFn;
        KEYPAD_TYPE lastKeyPressed;
        uint16_t currentTicks;
        uint16_t stabilizationTicks;
    };
    
    // State machine/Lifecycle function forward declarations, never to be called outside
    
    void Keypad_Scan(Keypad *me);
    void Keypad_Pressed_Unstable(Keypad *me);
    void Keypad_Pressed_Stable(Keypad *me);
    void Keypad_Reset(Keypad *me);
    
    // State machine tick function, should be executed constantly (i.e. Each time a timing event occurs) 
    void Keypad_Ticker(Keypad *me);
    

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* KEYPAD_H */

