#include "led.h"

void Led_Switch_Helper(Led *me, LedExternalEvent Event);

void Led_Idle(Led *me, LedExternalEvent Event){
    Led_Switch_Helper(me, Event);
}

void Led_Off(Led *me, LedExternalEvent Event){
    me->ledChangeCallbackFn(false);
    me->ledLastPhyState = false;
    me->currentStateFn = Led_Idle;
}

void Led_On(Led *me, LedExternalEvent Event){
    me->ledChangeCallbackFn(true);
    me->ledLastPhyState = true;
    me->currentStateFn = Led_Idle;
}

void Led_Pulse_Low(Led *me, LedExternalEvent Event){
    if(Event == LED_NO_EVENT){
        me->ledChangeCallbackFn(false);
        me->currentTicks++;
        if(me->currentTicks >= me->pulseLowDurationTicks){
            me->currentTicks = 0;
            me->currentPulses++;
            me->currentStateFn = Led_Pulse_High;
        }

        if(me->pulsesAmount >= 0 && me->currentPulses >= me->pulsesAmount){
            me->currentPulses = 0;
            me->currentStateFn = me->ledLastPhyState ? Led_On : Led_Off;
        }
    }
    else{
        Led_Switch_Helper(me, Event);
    }
}
    
void Led_Pulse_High(Led *me, LedExternalEvent Event){
    if(Event == LED_NO_EVENT){
        me->ledChangeCallbackFn(true);
        me->currentTicks++;
        if(me->currentTicks >= me->pulseHighDurationTicks){
            me->currentTicks = 0;
            me->currentPulses++;
            me->currentStateFn = Led_Pulse_Low;
        }

        if(me->pulsesAmount >= 0 && me->currentPulses >= me->pulsesAmount){
            me->currentPulses = 0;
            me->currentStateFn = me->ledLastPhyState ? Led_On : Led_Off;
        }
    }
    else{
        Led_Switch_Helper(me, Event);
    }
    
}

void Led_Switch_Helper(Led* me, LedExternalEvent Event){
    switch(Event){
        case LED_OFF:
            me->currentStateFn = Led_Off;
            break;
        case LED_ON:
            me->currentStateFn = Led_On;
            break;
        case LED_TOGGLE:
            me->currentStateFn = me->ledLastPhyState ? Led_Off : Led_On;
            break;
        case LED_PULSE:
            me->currentPulses = 0;
            me->currentTicks = 0;
            me->currentStateFn = me->ledLastPhyState ? Led_Pulse_Low : Led_Pulse_High;
            break;
        default:
            break;
    }
}

void Led_Ticker(Led* me, LedExternalEvent Event){
    me->currentStateFn(me, Event);
}