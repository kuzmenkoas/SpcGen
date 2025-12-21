#pragma once

#include "IDRSParser.h"

namespace Parser {
    class DRSConsoleParser : public IDRSParser {
    public:
        DRSConsoleParser();
        ~DRSConsoleParser();

        void Start() final;
    private:
        Global::Parameters usedPar;
        void ReadWriter();
        void ReadData();
        void ReadConfig();
        void ReadHistograms();
        void SetHistogramVector(std::string parameter);
    };
}