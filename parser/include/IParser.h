#pragma once

#include <iostream>

#include "Parameters.h"

#define DEFAULT_VALUE 1

namespace Parser {
class IParser {
public:
    IParser(){};
    ~IParser(){};

    std::vector<std::string> GetUsedWriterVector() { return fUsedWriterVector; };
    void SetUsedWriterVector(std::vector<std::string> writers) { fUsedWriterVector = writers; };

    void SetUsedParameters(Global::Parameters par) { fUsedParameters = par; };
    Global::Parameters GetUsedParameters() { return fUsedParameters; };

    virtual void Start() {};

    void SetDigitizerTypes(std::vector<std::string> types) { fDigitizerTypes = types; };
    std::vector<std::string> GetDigitizerTypes() { return fDigitizerTypes; };

    void SetCutFlag(bool flag) { fCutFlag = flag; };
    bool GetCutFlag() { return fCutFlag; };

    void SetThresholdFlag(bool flag) { fThresholdFlag = flag; };
    bool GetThresholdFlag() { return fThresholdFlag; };

private:
    std::vector<std::string> fUsedWriterVector{};
    Global::Parameters fUsedParameters{};
    std::vector<std::string> fDigitizerTypes = {};
    bool fCutFlag = false;
    bool fThresholdFlag = false;

protected:
    std::string ClearString(const std::string &line) {
        std::string res;
        const std::string trash = " \t\n\r\f\v";
        size_t start = line.find_first_not_of(trash);
        if (start == std::string::npos)
            return "";
        size_t end = line.find_last_not_of(trash);
        res = line.substr(start, end - start + 1);
        return res;
    };
};
}  // namespace Parser