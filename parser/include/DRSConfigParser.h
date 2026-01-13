#pragma once

#include "IDRSParser.h"
#include <filesystem>
#include <fstream>

namespace Parser {
    class DRSConfigParser : public IDRSParser {
    public:
        DRSConfigParser(std::filesystem::path path);
        ~DRSConfigParser();

        void Start() final;
    private:
        Global::Parameters usedPar;
        std::filesystem::path fPath;
        std::ifstream OpenFile();
        void ReadWriter(std::string key="Output");
        void ReadData(std::string key="Data");
        void ReadCharge(std::string key="Charge");
        void ReadConfig(std::string key="Config");
        void ReadHistograms(std::string key="Histogram");
        void ReadSignal(std::string key="Amplitude");
    };
}