#ifndef AT_COMMANDS_H
#define AT_COMMANDS_H

#define AT_COMMAND_LIST_MAX_SIZE 2
#define AT_MODIFIER_LIST_MAX_SIZE 2
#define AT_COMMAND_CHAR_LENGTH_MAX_SIZE 10 - 1
#define AT_COMMAND_CHAR_LENGTH_MIN_SIZE 8 - 1  

#include <stdint.h>

extern const uint32_t AT_Initial;

extern const uint32_t AT_CommandList[AT_COMMAND_LIST_MAX_SIZE];

extern const uint8_t AT_ModifierList[AT_MODIFIER_LIST_MAX_SIZE];

#endif /*AT_COMMANDS_H*/