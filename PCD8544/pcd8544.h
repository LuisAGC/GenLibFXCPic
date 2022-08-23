#ifndef PCD8544_H
#define	PCD8544_H 

#include <stdint.h>

#define PCD8544_SCREEN_BYTES 504


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    void PCD8544_Init(void);
    void PCD8544_ClearScreen(void);
    void PCD8544_SetAddress(uint8_t x, uint8_t y);
    void PCD8544_SendCommand(uint8_t command);
    void PCD8544_SendData(uint8_t data);
    void PCD8544_SendDataBuffer(uint8_t * buffer, uint16_t size);


    //Low level functions to be provided by user
    extern void PCD8544_SPI_SendBuffer(uint8_t * buffer, uint16_t size); // Read data can be discarded safely
    extern void PCD8544_DC_Command(); // Pin assigned should be set Low
    extern void PCD8544_DC_Data(); // Pin assigned should be set High
    extern void PCD8544_ChipEnable(); // Pin assigned should be set Low
    extern void PCD8544_ChipDisable(); // Pin assigned should be set High
    extern void PCD8544_Reset(); // Pin assigned should be set High - Low - High

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* PCD8544_H */

