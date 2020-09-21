#ifndef MILLIS_H
#define	MILLIS_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    /**
     * 
     * This function should be used to update the amount of milliseconds that have passed
     * since the device was turned on, the value is a 32 bit unsigned integer
     * meaning it will overflow after 50 days or so have passed
     * 
     * Probably should be executed in an ISR(Interrupt service routine) of a timer
     * 
     */
    void updateMillis(void);
    
    /**
     * 
     * Arduino inspired function that will return the amount of milliseconds that have
     * passed since the device was turned on, the value is a 32 bit unsigned integer
     * meaning it will overflow after 50 days or so have passed
     * 
     */
    uint32_t millis(void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif

