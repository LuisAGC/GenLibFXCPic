#include "button.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */ 

    void Button_Idle(Button *me){
        if(me->getButtonPhysicalStateCallbackFn() == true){
            me->currentStateFn = Button_Pressed_Unstable;
        }
    }

    void Button_Pressed_Unstable(Button *me){
        me->currentTicks++;
        if(me->getButtonPhysicalStateCallbackFn() == false){
            me->currentTicks = 0;
            me->currentStateFn = Button_Idle;
        }
        if(me->currentTicks > me->stabilizationTicks){
            me->currentTicks = 0;
            me->currentStateFn = Button_Pressed_Stable;
        }
    }

    void Button_Pressed_Stable(Button *me){
        me->currentTicks++;
        if(me->getButtonPhysicalStateCallbackFn() == false){
            me->currentStateFn = Button_ShortPress;
        }
        else if(me->currentTicks > me->longPressTicks){
            me->currentStateFn = Button_LongPress;
        }
    }

    void Button_ShortPress(Button *me){
        me->buttonShortPressCallbackFn();
        me->currentStateFn = Button_Reset;
    }

    void Button_LongPress(Button *me){
        me->buttonLongPressCallbackFn();
        me->currentStateFn = Button_Reset;
    }

    void Button_Reset(Button *me){
        if(me->getButtonPhysicalStateCallbackFn() == false){
            me->currentTicks = 0;
            me->buttonReleasedCallbackFn();
            me->currentStateFn = Button_Idle;
        }
    }

    void Button_Ticker(Button* me){
        me->currentStateFn(me);
    }
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */