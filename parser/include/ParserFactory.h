#pragma once

#include <memory>
#include "DRSConsoleParser.h"
#include "DRSConfigParser.h"
#include "DigitizerConfigParser.h"
#include "DigitizerConsoleParser.h"
#include "Types.h"

namespace Parser {
    class ParserFactory {
    public:
        ParserFactory();
        ~ParserFactory();

        void SetDigitizerTypes(std::vector<std::string> aTypes) {fDigitizerTypes = aTypes;};

        std::shared_ptr<IParser> BuildParser(Global::DeviceType aType);
        std::shared_ptr<IParser> BuildParser(Global::DeviceType aType, std::filesystem::path path);
    private:
        std::vector<std::string> fDigitizerTypes = {};
    };
}