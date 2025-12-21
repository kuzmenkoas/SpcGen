#pragma once

#include <cstdint>
#include <string>

namespace Global {
    struct IHist {
        std::string parameter;
        int Nbins;
        double min;
        double max;
    };
}