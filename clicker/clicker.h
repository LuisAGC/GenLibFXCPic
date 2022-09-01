#ifndef CLICKER_H
#define	CLICKER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */ 
  
    // Forward declaration of the struct type
    typedef struct ClickerStruct Clicker;
    
    typedef enum ClickerExternalEventsEnum {
        CLICK_TICK_EVENT,
        CLICK_EVENT,
    } ClickerExternalEvent;

    // Typedefs for struct function pointers
	// State machine current state function type declaration
    typedef void (*ClickerStateFn)(Clicker *me, ClickerExternalEvent event);
    typedef void (*ClickerPressFn)(void);
    
    // Definition of the Clicker struct
    struct ClickerStruct {
        ClickerStateFn currentStateFn;
        ClickerPressFn singleClickCallback;
        ClickerPressFn doubleClickCallback;
        ClickerPressFn tripleClickCallback;
        uint16_t currentTicks;
        uint16_t toleranceTicks;
    };
    
    // State machine/Lifecycle function forward declarations
	// Should not be called outside of the state machine lifecycle
    
    /**
    *  
    * This function executes constantly until a click event is received.
    * Once a valid click event is received, the currentStateFn pointer will change to
    * Clicker_SingleClick
    *  
    */
    void Clicker_Idle(Clicker *me, ClickerExternalEvent event);

    /**
     * 
     * This function will execute and increase the current tick timer until one of the 
     * following conditions is met:
     * 
     *  * me->currentTicks > me->toleranceTicks
     *  * event == CLICK_EVENT
     * 
     * if the first condition is met then the singleClickCallback will be executed and the state will point to Clicker_Idle
     * if the second is met then the state will start pointing to Clicker_DoubleClick
     * 
     */
    void Clicker_SingleClick(Clicker *me, ClickerExternalEvent event);

    /**
     * 
     * This function will execute and increase the current tick timer until one of the 
     * following conditions is met:
     * 
     *  * me->currentTicks > me->toleranceTicks
     *  * event == CLICK_EVENT
     * 
     * if the first condition is met then the doubleClickCallback will be executed and the state will point to Clicker_Idle
     * if the second is met then the state will start pointing to Clicker_TripleClick
     * 
     */
    void Clicker_DoubleClick(Clicker *me, ClickerExternalEvent event);

    /**
     * 
     * This function will immediatly execute the tripleClickCallback and state will point back to Clocker_Idle
     * 
     */
    void Clicker_TripleClick(Clicker *me, ClickerExternalEvent event);
    
    /**
     * 
     * This function should be the only one used and should be constantly executed in order to keep the state machine running 
     * 
     */
    void Clicker_Ticker(Clicker *me, ClickerExternalEvent event);
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* CLICKER_H */	

