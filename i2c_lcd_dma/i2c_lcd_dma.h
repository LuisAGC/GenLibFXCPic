#ifndef I2C_LCD_DMA_H
#define	I2C_LCD_DMA_H

#define I2C_LCD_BUFFER_SIZE     500

#define I2C_LCD_BCKLGT_ON       0x08
#define I2C_LCD_BCKLGT_OFF      0x00
#define I2C_LCD_CURSOR_ON       0x01
#define I2C_LCD_CURSOR_OFF      0x00
#define I2C_LCD_CURSOR_BLINK    0x02
#define I2C_LCD_CURSOR_NO_BLINK 0x00
#define I2C_LCD_CLEAR           0x01 << 4

#define I2C_LCD_DATA_R          0x01
#define I2C_LCD_COMMAND_R       0x00

#define I2C_LCD_EN_HIGH         0x04
#define I2C_LCD_EN_LOW          0x00

#define I2C_LCD_8BIT_MODE       0x03 << 4
#define I2C_LCD_4BIT_MODE       0x02 << 4

#define I2C_LCD_2LNE_MODE       0x08 << 4
#define I2C_LCD_CURSOR_CFG      0x0C << 4
#define I2C_LCD_DDRAM_CFG       0x06 << 4

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    extern uint8_t lcdBuffer[I2C_LCD_BUFFER_SIZE];
    extern uint16_t lcdBufferIndex;
    
    void LCD_Init(void);
    void LCD_AppendGoto(uint8_t x, uint8_t y);
    void LCD_AppendChar(char x);
    void LCD_AppendString(char * str);
    void LCD_Clear(void);
    
    

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* I2C_LCD_DMA_H */

