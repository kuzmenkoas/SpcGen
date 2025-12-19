#pragma once

#include "IParser.h"

namespace Parser {
    class IDRSParser : public IParser {
    public:
        IDRSParser() {
            Global::Parameters usedPar;
            usedPar.baseline = DEFAULT_VALUE;
            usedPar.integral = DEFAULT_VALUE;
            usedPar.time = DEFAULT_VALUE;
            usedPar.signal = {};

            Global::IHist bHist{"baseline", 1000, 0, 20000};
            Global::IHist iHist{"integral", 1000, 0, 50000};
            std::vector histVector = {bHist, iHist};
            usedPar.hist = histVector;

            Global::IDate date;
            usedPar.date = date;

            SetUsedParameters(usedPar);
        };
        ~IDRSParser() {};
    };
}