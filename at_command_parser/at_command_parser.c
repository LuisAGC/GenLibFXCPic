#include "at_commands.h"
#include "at_command_parser.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

ATCommandParser atCommandParserSingleton = {
    .bufferIndex = 0,
    .currentState = AT_Idle,
    .parsedCommand = &atCommandSingleton
};

ATCommand atCommandSingleton = {
    .commandIndex = 0,
    .dataIndex = 0,
    .modifierIndex = 0
};

uint32_t AT_Hash( ATCharacter *cmd ){
    uint16_t ch     = 0;
    uint32_t hash   = 4321;
    while( ( ch = *( cmd++ ) ) )
        hash = ( ( hash << 2 ) + hash ) + ch;
    return hash;
}

void AT_CleanBuffer(ATCommandParser *me){
    me->bufferIndex = 0;
    for (uint8_t i = 0; i < AT_BYTES_BUFFER_MAX_SIZE; i++){
        me->buffer[i] = 0;   
    }
}

void AT_CleanCommand(ATCommand *command){
    command->commandIndex = 0;
    command->dataIndex = 0;
    command->modifierIndex = 0;
    for (uint8_t i = 0; i < AT_DATA_MAX_SIZE; i++){
        command->data[i] = 0;
    }
}

void AT_Idle(ATCommandParser *me, ATCharacter currentCharacter){
    switch(currentCharacter){
        case 'A':
            me->buffer[me->bufferIndex] = 'A';
            me->bufferIndex++;
            me->currentState = AT_InitialParse;
            break;
        default:
            break;
    }
}

void AT_InitialParse(ATCommandParser *me, ATCharacter currentCharacter){
    me->buffer[me->bufferIndex] = currentCharacter;
    me->bufferIndex++;
    if(me->bufferIndex == 3){
        uint32_t hashValue = AT_Hash(me->buffer);
        if(hashValue == AT_Initial){
            AT_CleanBuffer(me);
            me->currentState = AT_CommandParse;
        }
        else{
            me->currentState = AT_Error;
        }
    }
}

void AT_CommandParse(ATCommandParser *me, ATCharacter currentCharacter){
    me->buffer[me->bufferIndex] = currentCharacter;
    me->bufferIndex++;
    if((currentCharacter < 'A' || currentCharacter > 'Z')){
        me->currentState = AT_Error;
    }
    if(me->bufferIndex > AT_COMMAND_CHAR_LENGTH_MAX_SIZE){
        me->currentState = AT_Error;
    }
    else if(me->bufferIndex >= AT_COMMAND_CHAR_LENGTH_MIN_SIZE){
        uint32_t bufferHash = AT_Hash(me->buffer);
        for (uint8_t i = 0; i < AT_COMMAND_LIST_MAX_SIZE; i++){
            if(AT_CommandList[i] == bufferHash){
                me->currentState = AT_CommandModifierParse;
                me->parsedCommand->commandIndex = i;
                AT_CleanBuffer(me);
                break;
            }
        }
    }
}

void AT_CommandModifierParse(ATCommandParser *me, ATCharacter currentCharacter){
    me->currentState = AT_Error;
    for (uint8_t i = 0; i < AT_MODIFIER_LIST_MAX_SIZE; i++){
        if(AT_ModifierList[i] == currentCharacter){
            me->currentState = AT_DataParse;
            me->parsedCommand->modifierIndex = i;
            break;
        }   
    }
}

void AT_DataParse(ATCommandParser *me, ATCharacter currentCharacter){
    if((currentCharacter >= '0' && currentCharacter <= '9') || currentCharacter == '-'){
        me->buffer[me->bufferIndex] = currentCharacter;
        me->bufferIndex++;
        if(me->bufferIndex > AT_COMMAND_CHAR_LENGTH_MAX_SIZE){
            me->currentState = AT_Error;
        }
        else{
            me->currentState = AT_DataParseNumber;
        }
    }
    else if(currentCharacter == '\n' || currentCharacter == '\r'){
        me->callbackFn(me->parsedCommand);
        me->currentState = AT_Reset;
    } 
    else {
        me->currentState = AT_Error;
    }
}

void AT_DataParseNumber(ATCommandParser *me, ATCharacter currentCharacter){
    if((currentCharacter >= '0' && currentCharacter <= '9')){
        me->buffer[me->bufferIndex] = currentCharacter;
        me->bufferIndex++;
        if(me->bufferIndex > AT_COMMAND_CHAR_LENGTH_MAX_SIZE){
            me->currentState = AT_Error;
        }
    } 
    else if(currentCharacter == '\n' || currentCharacter == '\r'){
        me->parsedCommand->data[me->parsedCommand->dataIndex] = atoi(me->buffer);
        me->parsedCommand->dataIndex++;
        me->callbackFn(me->parsedCommand);
        me->currentState = AT_Reset;
    } 
    else if(currentCharacter == ','){
        me->parsedCommand->data[me->parsedCommand->dataIndex] = atoi(me->buffer);
        me->parsedCommand->dataIndex++;
        AT_CleanBuffer(me);
        me->currentState = AT_DataParse;
    } 
    else {
        me->currentState = AT_Error;
    }
}

void AT_Error(ATCommandParser *me, ATCharacter currentCharacter){
    AT_CleanBuffer(me);
    me->errorCallbackFn();
    me->currentState = AT_Reset;
}

void AT_Reset(ATCommandParser *me, ATCharacter currentCharacter){
    if(currentCharacter == '\n' || currentCharacter == '\r'){
        AT_CleanBuffer(me);
        AT_CleanCommand(me->parsedCommand);    
        me->currentState = AT_Idle;
    }
}

void AT_Ticker(ATCommandParser *me, ATCharacter currentCharacter){
    me->currentState(me, currentCharacter);
}

