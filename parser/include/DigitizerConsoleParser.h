#pragma once

#include "IParser.h"

namespace Parser {
    class DigitizerConsoleParser : public IParser {
    public:
        DigitizerConsoleParser(std::vector<std::string> aTypes);
        ~DigitizerConsoleParser();

        void Start() final;
    private:
        Global::Parameters usedPar;
        std::vector<std::string> fTypes = {};
        void ReadFileType();
        void ReadWriter();
        void ReadDataPSD();
        void ReadDataWaveform();
        void ReadHistograms();
        void ReadConfig();
        void SetHistogramVector(std::string file, std::string parameter);
    };
}