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

            Global::IHist bHist{"baseline", 1000, 0, 20000};
            Global::IHist iHist{"charge", 1000, 0, 50000};
            std::vector histVector = {bHist, iHist};
            usedPar.hist = histVector;

            Global::IDate date;
            usedPar.date = date;

            SetUsedParameters(usedPar);
        };
        ~IDRSParser() {};
    };
}