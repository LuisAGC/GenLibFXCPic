#include "clicker.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */ 

    void Clicker_Idle(Clicker *me, ClickerExternalEvent event){
        if(event == CLICK_EVENT){
            me->currentStateFn = Clicker_SingleClick;
        }
    }

    void Clicker_SingleClick(Clicker *me, ClickerExternalEvent event){
        me->currentTicks++;
        if(event == CLICK_EVENT){
            me->currentTicks = 0;
            me->currentStateFn = Clicker_DoubleClick;
        }
        if(me->currentTicks > me->toleranceTicks){
            me->singleClickCallback();
            me->currentTicks = 0;
            me->currentStateFn = Clicker_Idle;
        }
    }

    void Clicker_DoubleClick(Clicker *me, ClickerExternalEvent event){
        me->currentTicks++;
        if(event == CLICK_EVENT){
            me->currentTicks = 0;
            me->currentStateFn = Clicker_TripleClick;
        }
        if(me->currentTicks > me->toleranceTicks){
            me->doubleClickCallback();
            me->currentTicks = 0;
            me->currentStateFn = Clicker_Idle;
        };
    }

    void Clicker_TripleClick(Clicker *me, ClickerExternalEvent event){
        me->currentTicks++;
        if(me->currentTicks > me->toleranceTicks){
            me->tripleClickCallback();
            me->currentTicks = 0;
            me->currentStateFn = Clicker_Idle;
        }
    }

    void Clicker_Ticker(Clicker *me, ClickerExternalEvent event){
        me->currentStateFn(me, event);
    }
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */
