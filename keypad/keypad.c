#include "keypad.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    void Keypad_Idle(Keypad *me){
        KEYPAD_TYPE keyPressed = me->getKeypadPhysicalStateFn();
        if(keyPressed > 0){
            me->currentStateFn = Keypad_Pressed_Unstable;
            me->lastKeyPressed = keyPressed;
        }
    }

    void Keypad_Pressed_Unstable(Keypad *me){
        me->currentTicks++;
        if(me->getKeypadPhysicalStateFn() != me->lastKeyPressed){
            me->currentTicks = 0;
            me->currentStateFn = Keypad_Idle;
        }
        if(me->currentTicks > me->stabilizationTicks){
            me->currentTicks = 0;
            me->currentStateFn = Keypad_Pressed_Stable;
        }
    }

    void Keypad_Pressed_Stable(Keypad *me){
        me->keyPressCallbackFn(me->lastKeyPressed);
        me->currentStateFn = Keypad_Reset;
    }

    void Keypad_Reset(Keypad *me){
        if(me->getKeypadPhysicalStateFn() == 0){
            me->currentTicks = 0;
            me->lastKeyPressed = 0;
            me->currentStateFn = Keypad_Idle;
        }
    }

    void Keypad_Ticker(Keypad *me){
        me->currentStateFn(me);
    }

#ifdef	__cplusplus
}
#endif /* __cplusplus */
