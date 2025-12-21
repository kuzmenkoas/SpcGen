#pragma once

#include "IDRSParser.h"
#include <filesystem>

namespace Parser {
    class DRSConfigParser : public IDRSParser {
    public:
        DRSConfigParser(std::filesystem::path path);
        ~DRSConfigParser();
    };
}