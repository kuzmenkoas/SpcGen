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
        std::optional<double> baseline;
        std::optional<std::vector<IHist>> hist;
        std::optional<std::vector<double>> waveform;
        std::optional<double> charge;
        std::optional<double> amplitude;
        std::optional<std::pair<int16_t, int16_t>> baselineLimits;
        std::optional<std::pair<int16_t, int16_t>> chargeLimits;

        // DRS data
        std::optional<int32_t> time;
        std::optional<IDate> date;
        std::optional<int16_t> range;
        std::optional<int16_t> trigger;
        std::optional<uint32_t> scaler;
        std::optional<std::string> signal;
        std::optional<std::pair<int16_t, int16_t>> signalRange;

        // Digitizer PSD
        std::optional<int32_t> qShort;
        std::optional<int32_t> qLong;
        std::optional<int16_t> cfd_y1;
        std::optional<int16_t> cfd_y2;
        std::optional<int16_t> baselinePSD;
        std::optional<int16_t> height;
        std::optional<uint32_t> eventCounter;
        std::optional<uint32_t> eventCounterPSD;
        std::optional<int16_t> psdValue;
    };
}