#include "pcd8544.h"

void PCD8544_Init(void){
    PCD8544_Reset();
    PCD8544_SendCommand(0x21); //Set to extended command mode, horizontal addressing
    PCD8544_SendCommand(0xb2); //Set VLCD to 6 volts
    PCD8544_SendCommand(0x15); //Set voltage bias to 5
    PCD8544_SendCommand(0x20); //Set to normal command mode
    PCD8544_ClearScreen();
    PCD8544_SendCommand(0x09); //Activate all pixels
    PCD8544_SendCommand(0x0c); //Set display to normal mode
}

void PCD8544_ClearScreen(void){
    PCD8544_SetAddress(0, 0);
    for(uint16_t i = 0; i < PCD8544_SCREEN_BYTES; i++){
        PCD8544_SendData(0x00);
    }
}

void PCD8544_SetAddress(uint8_t x, uint8_t y){
    x = x % 84;
    x = x | 0x80; 
    PCD8544_SendCommand(x);
    
    y = y % 6;
    y = y | 0x40;
    PCD8544_SendCommand(y);
}

void PCD8544_SendCommand(uint8_t command){
    PCD8544_DC_Command();
    PCD8544_ChipDisable();
    PCD8544_ChipEnable();
    
    PCD8544_SPI_SendBuffer(&command, 1);
    
    PCD8544_ChipDisable();
}

void PCD8544_SendData(uint8_t data){
    PCD8544_DC_Data();
    PCD8544_ChipDisable();
    PCD8544_ChipEnable();
    
    PCD8544_SPI_SendBuffer(&data, 1);
    
    PCD8544_ChipDisable();
}

void PCD8544_SendDataBuffer(uint8_t * buffer, uint16_t size){
    PCD8544_DC_Data();
    PCD8544_ChipDisable();
    PCD8544_ChipEnable();
    
    PCD8544_SPI_SendBuffer(buffer, size);
    
    PCD8544_ChipDisable();
}