#pragma once

#include "IDRSParser.h"
#include <filesystem>

namespace Parser {
    class DRSConfigParser : public IDRSParser {
    public:
        DRSConfigParser(std::filesystem::path path);
        ~DRSConfigParser();

        void Start() final;
    private:
        Global::Parameters usedPar;
        void ReadData();
        void ReadConfig();
        void ReadHistograms();
    };
}