#pragma once

#include <stdint.h>
#include <type_traits>

enum class SYSTEM_FLAG:uint32_t{
    //state flags
    STATE_IDLE = (1 << 0), 
    //flags
    DEBUG = (1 << 7),
    //critical messages 
    ERROR_SERIAL = (1 << 10),
    ERROR_CAN_LOGGER = (1 << 11),
    //if rocket is inverted
    ERROR_ORIENTATION = (1 << 24),
    //warn
    WARN_BATT = (1 << 25),
    //FLIGHTPHASES 
    FLIGHTPHASE_BOOST = (1 << 26),
    FLIGHTPHASE_COAST = (1 << 27),
    FLIGHTPHASE_APOGEE = (1 << 28)
    
};

using system_flag_t = uint32_t;

