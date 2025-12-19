#pragma once

#include <memory>
#include <filesystem>
#include "DRSConsoleParser.h"
#include "Types.h"

namespace Parser {
    class ParserFactory {
    public:
        ParserFactory();
        ~ParserFactory();

        std::shared_ptr<IParser> BuildParser(Global::DeviceType aType);
        std::shared_ptr<IParser> BuildParser(Global::DeviceType aType, std::filesystem::path path);
    };
}