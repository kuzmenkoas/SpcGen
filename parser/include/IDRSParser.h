#pragma once

#include "IParser.h"

namespace Parser {
    class IDRSParser : public IParser {
    public:
        IDRSParser() {
            Global::Parameters usedPar;
            usedPar.baseline = DEFAULT_VALUE;
            usedPar.charge = DEFAULT_VALUE;
            usedPar.time = DEFAULT_VALUE;
            usedPar.waveform = {DEFAULT_VALUE};
            usedPar.scaler = DEFAULT_VALUE;

            Global::IHist iHist{"charge", 1000, -10, 60};
            std::vector histVector = {iHist};
            usedPar.hist = histVector;

            Global::IDate date;
            usedPar.date = date;

            usedPar.baselineLimits = std::make_pair(16, 400);
            usedPar.chargeLimits = std::make_pair(420, 1000);

            SetUsedParameters(usedPar);
        };
        ~IDRSParser() {};
    private:
    };
}