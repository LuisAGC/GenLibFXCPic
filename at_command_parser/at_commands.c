#include "at_commands.h"
#include <stdint.h>

const uint32_t AT_Initial = 542213L;

const uint32_t AT_CommandList[AT_COMMAND_LIST_MAX_SIZE] = {
    1695183239L, //LEDSTATE
    1695181759L  //LEDPULSE
};

const uint8_t AT_ModifierList[AT_MODIFIER_LIST_MAX_SIZE] = {
    '=',
    '?'
};