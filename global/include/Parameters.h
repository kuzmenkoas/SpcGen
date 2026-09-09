#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "IDate.h"
#include "IHist.h"
#include "Time.h"
#include <ctime>

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
    std::optional<std::pair<int16_t, int16_t>> amplitudeLimits;
    std::optional<double> factorCharge;
    std::optional<double> shiftCharge;
    std::optional<double> factorAmplitude;
    std::optional<double> shiftAmplitude;
    std::optional<double> cut;
    std::optional<double> threshold;

    // DRS data
    std::optional<int32_t> time;
    std::optional<IDate> date;
    std::optional<int16_t> range;
    std::optional<int16_t> trigger;
    std::optional<uint32_t> scaler;
    std::optional<std::string> signal;
    std::optional<std::pair<int16_t, int16_t>> signalRange;
    std::optional<Time> date_time;

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
    std::optional<int32_t> wavelength;
};
}  // namespace Global