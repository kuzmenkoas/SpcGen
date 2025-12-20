#pragma once

#include <optional>
#include <cstdint>
#include <utility>
#include <vector>
#include <string>

#include "IHist.h"
#include "IDate.h"

namespace Global {
    struct Parameters {
        // all
        std::optional<int16_t> baseline;
        std::optional<std::vector<IHist>> hist;
        std::optional<std::vector<double>> waveform;
        std::optional<float> charge;
        std::optional<int16_t> amplitude;

        // DRS data
        std::optional<int32_t> time;
        std::optional<IDate> date;
        std::optional<int16_t> range;
        std::optional<int16_t> trigger;

        // Digitizer PSD
        std::optional< int32_t> qShort;
        std::optional< int32_t> qLong;
        std::optional< int16_t> cfd_y1;
        std::optional< int16_t> cfd_y2;
        std::optional< int16_t> height;
        std::optional< int32_t> eventCounter;
        std::optional< int32_t> eventCounterPSD;
        std::optional< int16_t> psdValue;
    };
}