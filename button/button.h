#ifndef BUTTON_H
#define	BUTTON_H

#include<stdint.h>
#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */ 
  
    // Forward declaration of the struct type
    typedef struct ButtonStruct Button;
    
    // Typedefs for struct function pointers
	// State machine current state function type declaration
    typedef void (*ButtonStateFn)(Button *me);
    typedef bool (*ButtonPhyStateFn)(void);
    typedef void (*ButtonPressFn)(void);
    typedef void (*ButtonReleaseFn)(void);
    
    // Definition of the Button struct
    struct ButtonStruct {
        ButtonPhyStateFn getButtonPhysicalStateCallbackFn;
        ButtonStateFn currentStateFn;
        ButtonPressFn buttonShortPressCallbackFn;
        ButtonPressFn buttonLongPressCallbackFn;
        ButtonReleaseFn buttonReleasedCallbackFn;
        uint16_t currentTicks;
        uint16_t stabilizationTicks;
        uint16_t longPressTicks;
    };
    
    // State machine/Lifecycle function forward declarations
	// Should not be called outside of the state machine lifecycle
    
    /**
    *  
    * This function executes constantly until the physical state of the button 
    * is changed to pressed or true.
    * Once the button is pressed, the currentStateFn pointer will change to
    * Button_Pressed_Unstable
    * 
    */
    void Button_Idle(Button *me);
    
    /**
    * 
    * This function will update the ticks of the struct by one each time it's called.
    * If the physical state of the button is false or released the currentStateFn 
    * variable will be changed back to Button_Idle because the button is still bouncing
    * and the currentTicks variable will be set to 0.
    * 
    * Once the bouncing stops and if the amount of currentTicks is greater than the 
    * value of the stabilizationTicks, the value of currentTicks will be set to zero
    * and currentStateFn will be changed to Button_Pressed_Stable
    * 
    */
    void Button_Pressed_Unstable(Button *me);
    
    /**
    * 
    * This function will update the ticks of the struct by one each time it's called.
    * This function will determine if the press of the button is a long or short one.
    *  
    * if the button is released while the value of currentTicks is less than or equal to
    * longPressTicks then currentStateFn will be changed to Button_ShortPress, otherwise
    * if the value of currentTicks is greater than longPressTicks then currentStateFn
    * will be set to Button_LongPress
    * 
    */
    void Button_Pressed_Stable(Button *me);
    
    /**
    * 
    * This function will execute the callback set to buttonShortPressCallbackFn and
    * then change currentStateFn to Button_Reset
    * 
    */
    void Button_ShortPress(Button *me);
    
    /**
    * 
    * This function will execute the callback set to buttonLongPressCallbackFn and
    * then change currentStateFn to Button_Reset
    * 
    */
    void Button_LongPress(Button *me);
    
    /**
    * 
    * This function will set currentTicks back to 0 then execute the callback set 
    * to buttonReleasedCallbackFn and finally change currentStateFn to Button_Idle
    * only if the button is already released, otherwise it will do nothing
    * 
    */
    void Button_Reset(Button *me);
    
    // State machine tick function, should be executed constantly (i.e. Each time a timing event occurs)
    /**
    * 
    * This function will execute the function to which currentStateFn is pointing to.
    * This can be thought as the "heartbeat" or "clock" of the state machine,
    * so it should be run constantly.
    * 
    */ 
    void Button_Ticker(Button *me);
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* BUTTON_H */	

