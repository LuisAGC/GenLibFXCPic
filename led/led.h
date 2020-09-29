#ifndef LED_H
#define	LED_H
#define LED_PULSE_FOREVER -1

#include <stdint.h>
#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
    
	//Forward declaration of the Led struct
    typedef struct LedStruct Led;
    
	//Enum declaration for the events that the state machine is able to receive
	typedef enum LedExternalEventsEnum {
        LED_NO_EVENT,
        LED_OFF,
        LED_ON,
        LED_TOGGLE,
        LED_PULSE
    } LedExternalEvent;
    
	// Typedefs for struct function pointers
	// State machine current state function type declaration
    typedef void (*LedStateFn)(Led *me, LedExternalEvent Event);
	
	// Function typedef, callback to change the "physical" state of the LED
    typedef void (*LedPhyStateChangeFn)(bool phyState);
    
	/** 
	* Definition of the Led struct, each LED that will be used should have 
	* it's own instance, then the function Led_Ticker should be executed passing
	* the address of the struct instance in the parameters.
	* Led_Ticker(&ledStructInstance, LED_NO_EVENT|LED_OFF|LED_ON|LED_TOGGLE|LED_PULSE);
	*/
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
    
	
	// State machine/Lifecycle function forward declarations
	// Should not be called outside of the state machine lifecycle
	
	/**
	* This is the function to which the Led state machine points after 
	* finishing the execution from the other states.
	* If the state machine receives a valid event then currentStateFn
	* will be changed to point to the respective state, for example: 
	* If the event received is LED_ON then currentStateFn will change to
	* Led_On(Led *me, LedExternalEvent Event)
	*/
    void Led_Idle(Led *me, LedExternalEvent Event);
	
	/**
	* This function will execute the function set to ledChangeCallbackFn
	* with the parameter set to false, as in turn OFF the LED, and update
	* ledLastPhyState to false, finally currentStateFn will 
	* point to Led_Idle(Led *me, LedExternalEvent Event) 
	*/
    void Led_Off(Led *me, LedExternalEvent Event);
	
	/**
	* This function will execute the function set to ledChangeCallbackFn
	* with the parameter set to true, as in turn ON the LED, and update
	* ledLastPhyState to true, finally currentStateFn will 
	* point to Led_Idle(Led *me, LedExternalEvent Event) 
	*/
    void Led_On(Led *me, LedExternalEvent Event);
	
	/**
	* If this functions receives an event other than LED_NO_EVENT then
	* currentStateFn will be set to point to the respective state otherwise
	* it will do the following:
	*
	* Execute the function set to ledChangeCallbackFn with the parameter 
	* set to false.
	* Increase the amount of currentTicks by one
	* Check if currentTicks is more than pulseLowDurationTicks, if yes then
	* set currentTicks back to 0, increase currentPulses by one and change
	* currentStateFn to (Led *me, LedExternalEvent Event)
	*
	* Check if pulsesAmount is greater than or equal to 0 and if currentPulses
	* is more than pulsesAmount then change state based on ledLastPhyState,
	* Led_On if true, Led_Off if false
	*
	* Note: If pulsesAmount is less than 0, led will pulse indefinitely until
	* a valid event is received
	*/
    void Led_Pulse_Low(Led *me, LedExternalEvent Event);
	
	/**
	* If this functions receives an event other than LED_NO_EVENT then
	* currentStateFn will be set to point to the respective state otherwise
	* it will do the following:
	*
	* Execute the function set to ledChangeCallbackFn with the parameter 
	* set to true.
	* Increase the amount of currentTicks by one
	* Check if currentTicks is more than pulseHighDurationTicks, if yes then
	* set currentTicks back to 0, increase currentPulses by one and change
	* currentStateFn to (Led *me, LedExternalEvent Event)
	*
	* Check if pulsesAmount is greater than or equal to 0 and if currentPulses
	* is more than pulsesAmount then change state based on ledLastPhyState,
	* Led_On if true, Led_Off if false
	*
	* Note: If pulsesAmount is less than 0, led will pulse indefinitely until
	* a valid event is received
	*/
    void Led_Pulse_High(Led *me, LedExternalEvent Event);
    
	
	// State machine tick function, should be executed constantly (i.e. Each time a timing event occurs)
    /**
    * 
    * This function will execute the function to which currentStateFn is pointing to.
    * This can be thought as the "heartbeat" or "clock" of the state machine,
    * so it should be run constantly.
    * 
    */ 
    void Led_Ticker(Led *me, LedExternalEvent Event);
    
    

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* LED_H */

