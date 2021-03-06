#include "i2c_lcd_dma.h"

uint8_t lcdBuffer[I2C_LCD_BUFFER_SIZE] = {};
uint16_t lcdBufferIndex = 0;

void LCD_Init(void){
    lcdBufferIndex = 1;
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_BCKLGT_ON; //Turning off every pin in PCF8754/A except backlight
    
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_8BIT_MODE | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_HIGH ; //Setting LCD in 8bit data mode 0x3C
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_8BIT_MODE | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_LOW ;  //Setting LCD in 8bit data mode 0x38
    //This for is required to send enough dummy data to the screen to simulate a delay of 5ms while using dma
    for(uint8_t i = 0; i < 63; i++){
        lcdBuffer[lcdBufferIndex++] = I2C_LCD_BCKLGT_ON; //Dummy data
    }
    
    //Same thing as before set screen in 8bit mode and send dummy data for delay
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_8BIT_MODE | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_HIGH ; //Setting LCD in 8bit data mode 0x3C
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_8BIT_MODE | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_LOW ;  //Setting LCD in 8bit data mode 0x38
    for(uint8_t i = 0; i < 63; i++){
        lcdBuffer[lcdBufferIndex++] = I2C_LCD_BCKLGT_ON; //Dummy data
    }
    
    //And once again...
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_8BIT_MODE | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_HIGH ; //Setting LCD in 8bit data mode 0x3C
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_8BIT_MODE | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_LOW ;  //Setting LCD in 8bit data mode 0x38
    for(uint8_t i = 0; i < 63; i++){
        lcdBuffer[lcdBufferIndex++] = I2C_LCD_BCKLGT_ON; //Dummy data
    }
    
    //Here we set the screen in 4bit mode and some more dummy data for delay
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_4BIT_MODE | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_HIGH ; //Setting LCD in 4bit data mode 0x2C
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_4BIT_MODE | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_LOW ;  //Setting LCD in 4bit data mode 0x28
    for(uint8_t i = 0; i < 63; i++){
        lcdBuffer[lcdBufferIndex++] = I2C_LCD_BCKLGT_ON; //Dummy data
    }
    
    //Configure screen uint8_t
    // LCD 2 line mode
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_4BIT_MODE | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_HIGH ; //Setting LCD in 2 line mode, most significant nibble
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_4BIT_MODE | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_LOW ;  //Setting LCD in 2 line mode, most significant nibble
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_2LNE_MODE| I2C_LCD_BCKLGT_ON | I2C_LCD_EN_HIGH ;  //Setting LCD in 2 line mode, least significant nibble
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_2LNE_MODE | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_LOW ;  //Setting LCD in 2 line mode, least significant nibble
    
    // LCD cursor setup
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_BCKLGT_ON | I2C_LCD_EN_HIGH; //Setting lcd cursor off, most significant nibble
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_BCKLGT_ON | I2C_LCD_EN_LOW;  //Setting lcd cursor off, most significant nibble
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_DDRAM_CFG | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_HIGH ;  //Setting lcd cursor off, least significant nibble
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_DDRAM_CFG | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_LOW ;  //Setting lcd cursor off, least significant nibble
    
    // LCD ddram setup
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_BCKLGT_ON | I2C_LCD_EN_HIGH ; //Setting LCD increment DDRAM address on character write, most significant nibble
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_BCKLGT_ON | I2C_LCD_EN_LOW ;  //Setting LCD increment DDRAM address on character write, most significant nibble
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_CURSOR_CFG | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_HIGH ;  //Setting LCD increment DDRAM address on character write, least significant nibble
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_CURSOR_CFG | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_LOW ;  //Setting LCD increment DDRAM address on character write, least significant nibble
    
    LCD_Clear();
}

void LCD_AppendChar(char x){
    uint8_t mostSgnfNibble = x & 0xF0;
    uint8_t leastSgnfNibble = (x << 4) & 0xF0;
    lcdBuffer[lcdBufferIndex++] = mostSgnfNibble  | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_HIGH | I2C_LCD_DATA_R; 
    lcdBuffer[lcdBufferIndex++] = mostSgnfNibble  | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_LOW | I2C_LCD_DATA_R; 
    lcdBuffer[lcdBufferIndex++] = leastSgnfNibble | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_HIGH | I2C_LCD_DATA_R;   
    lcdBuffer[lcdBufferIndex++] = leastSgnfNibble | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_LOW | I2C_LCD_DATA_R;   
}

void LCD_AppendString(char * str){
    while( *str != 0){
		LCD_AppendChar(*str);
		str++;
	}
}

void LCD_AppendGoto(uint8_t x, uint8_t y){
    uint8_t addressCommand = 0x80;
    uint8_t mostSgnfNibble = 0x00;
    uint8_t leastSgnfNibble = 0x00;
    switch(x){
        case 0: 
            addressCommand |= 0x00;
            break;
        case 1:
            addressCommand |= 0x40; 
            break;
        case 2:
            addressCommand |= 0x14;
            break;
        case 3:  
            addressCommand |= 0x54;
            break;
        default:
            addressCommand |= 0x00;
            break;
    }
    if(y > 19){
        y = 0;
    }
    addressCommand += y;
    mostSgnfNibble =    (addressCommand << 0) & 0xF0;
    leastSgnfNibble =   (addressCommand << 4) & 0xF0;
    lcdBuffer[lcdBufferIndex++] = mostSgnfNibble  | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_HIGH; 
    lcdBuffer[lcdBufferIndex++] = mostSgnfNibble  | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_LOW; 
    lcdBuffer[lcdBufferIndex++] = leastSgnfNibble | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_HIGH;  
    lcdBuffer[lcdBufferIndex++] = leastSgnfNibble | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_LOW;  
}

void LCD_Clear(void){
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_BCKLGT_ON | I2C_LCD_EN_HIGH; 
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_BCKLGT_ON | I2C_LCD_EN_LOW;
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_CLEAR | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_HIGH ;
    lcdBuffer[lcdBufferIndex++] = I2C_LCD_CLEAR | I2C_LCD_BCKLGT_ON | I2C_LCD_EN_LOW ;
    for(uint8_t i = 0; i < 63; i++){
        lcdBuffer[lcdBufferIndex++] = I2C_LCD_BCKLGT_ON; //Dummy data
    }
}