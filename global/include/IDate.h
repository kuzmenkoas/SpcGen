#pragma once

#include <cstdint>

namespace Global {
    struct IDate {
        int16_t year;
        int16_t month;
        int16_t day;
        int16_t hour;
        int16_t minute;
        int16_t second;
        int16_t millisecond;
    };
}