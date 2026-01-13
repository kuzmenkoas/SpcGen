#pragma once

#include "IDigitizerParser.h"
#include <filesystem>
#include <fstream>

namespace Parser {
    class DigitizerConfigParser : public IDigitizerParser {
    public:
        DigitizerConfigParser(std::filesystem::path path, std::vector<std::string> aTypes);
        ~DigitizerConfigParser();

        void Start() final;
    private:
        Global::Parameters usedPar;
        std::filesystem::path fPath;
        std::vector<std::string> fTypes = {};
        std::ifstream OpenFile();
        void ReadFileType();
        void ReadWriter(std::string key="Output");
        void ReadDataPSD(std::string key="DataPSD");
        void ReadDataWaveform(std::string key="DataWaveform");
        void ReadHistograms(std::string key="Histogram");
        void ReadCharge(std::string key="Charge");
        void ReadConfig(std::string key="Config");
        void ReadSignal(std::string key="Amplitude");
    };
}