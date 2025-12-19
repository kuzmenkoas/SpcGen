#pragma once

#include <cstdint>
#include <string>

namespace Global {
    struct IHist {
        std::string parameter;
        int Nbins;
        int32_t min;
        int32_t max;
    };
}