#ifndef LED_H
#define	LED_H
#define LED_PULSE_FOREVER -1

#include <stdint.h>
#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
    typedef struct LedStruct Led;
    typedef enum LedExternalEventsEnum {
        LED_NO_EVENT,
        LED_OFF,
        LED_ON,
        LED_TOGGLE,
        LED_PULSE
    } LedExternalEvent;
    
    typedef void (*LedStateFn)(Led *me, LedExternalEvent Event);
    typedef void (*LedPhyStateChangeFn)(bool phyState);
    
    struct LedStruct{
        LedStateFn currentStateFn;
        LedPhyStateChangeFn ledChangeCallbackFn;
        uint16_t currentTicks;
        uint16_t pulseHighDurationTicks;
        uint16_t pulseLowDurationTicks;
        int16_t pulsesAmount;
        int16_t currentPulses;
        bool ledLastPhyState;
    };
    
    void Led_Idle(Led *me, LedExternalEvent Event);
    void Led_Off(Led *me, LedExternalEvent Event);
    void Led_On(Led *me, LedExternalEvent Event);
    void Led_Pulse_Low(Led *me, LedExternalEvent Event);
    void Led_Pulse_High(Led *me, LedExternalEvent Event);
    
    void Led_Ticker(Led *me, LedExternalEvent Event);
    
    

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* LED_H */

