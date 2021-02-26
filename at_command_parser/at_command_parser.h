#include "at_commands.h"
#include <stdint.h>
#include <stdbool.h>

#ifndef AT_COMMAND_PARSER_H
#define AT_COMMAND_PARSER_H


#ifdef __cplusplus
extern "C" {
#endif

    #define AT_BYTES_BUFFER_MAX_SIZE 32
    #define AT_DATA_MAX_SIZE 10

    typedef struct ATCommandParserStruct ATCommandParser;
    typedef struct ATCommandStruct ATCommand;
    typedef char ATCharacter;

    typedef void (*ATCommandParserStateFn)(ATCommandParser *me, ATCharacter currentCharacter);
    typedef void (*ATCommandParsedCallbackFn)(ATCommand *command);
    typedef void (*ATCommandErrorCallbackFn)(void);

    struct ATCommandParserStruct {
        ATCommandParserStateFn currentState;
        ATCommandParsedCallbackFn callbackFn;
        ATCommandErrorCallbackFn errorCallbackFn;
        ATCommand *parsedCommand;
        ATCharacter buffer[AT_BYTES_BUFFER_MAX_SIZE];
        uint8_t bufferIndex;
    };

    struct ATCommandStruct {
        uint8_t commandIndex;
        uint8_t modifierIndex;
        int16_t data[AT_DATA_MAX_SIZE];
        uint8_t dataIndex;
    };

    extern ATCommandParser atCommandParserSingleton;
    extern ATCommand atCommandSingleton;
    
    void AT_Idle(ATCommandParser *me, ATCharacter currentCharacter);
    void AT_InitialParse(ATCommandParser *me, ATCharacter currentCharacter);
    void AT_CommandParse(ATCommandParser *me, ATCharacter currentCharacter);
    void AT_CommandModifierParse(ATCommandParser *me, ATCharacter currentCharacter);
    void AT_DataParse(ATCommandParser *me, ATCharacter currentCharacter);
    void AT_DataParseNumber(ATCommandParser *me, ATCharacter currentCharacter);
    void AT_Error(ATCommandParser *me, ATCharacter currentCharacter);
    void AT_Reset(ATCommandParser *me, ATCharacter currentCharacter);

    void AT_Ticker(ATCommandParser *me, ATCharacter currentCharacter);



#ifdef __cplusplus
}
#endif

#endif /*AT_COMMAND_PARSER_H*/