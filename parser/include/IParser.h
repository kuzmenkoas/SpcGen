#pragma once

#include <iostream>

#include "Parameters.h"

#define DEFAULT_VALUE 1

namespace Parser {
    class IParser {
    public:
        IParser() {this->Start();};
        ~IParser() {};

        std::vector<std::string> GetUsedWriterVector() {return fUsedWriterVector;};

        void SetUsedParameters(Global::Parameters par) {fUsedParameters = par;};
        Global::Parameters GetUsedParameters() {return fUsedParameters;};

        virtual void Start() {};
    private:
        std::vector<std::string> fUsedWriterVector{"Root"};
        Global::Parameters fUsedParameters{};
    };
}