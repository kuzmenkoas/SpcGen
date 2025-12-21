#pragma once

#include "IParser.h"

namespace Parser {
    class IDRSParser : public IParser {
    public:
        IDRSParser() {this->Start();};
        ~IDRSParser() {};
    private:
    };
}