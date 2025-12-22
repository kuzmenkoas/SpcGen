#include "DigitizerConsoleParser.h"

Parser::DigitizerConsoleParser::DigitizerConsoleParser(std::vector<std::string> aTypes) : fTypes(aTypes) {
}

Parser::DigitizerConsoleParser::~DigitizerConsoleParser() {
}

void Parser::DigitizerConsoleParser::Start() {
    ReadWriter();
    if (fTypes.empty()) ReadFileType();
    for (std::string file : fTypes) {
        if (file == "PSD") ReadDataPSD();
        if (file == "Waveform") {
            ReadDataWaveform();
            ReadConfig();
        }
    }
    ReadHistograms();

    SetUsedParameters(usedPar);
}

void Parser::DigitizerConsoleParser::ReadFileType() {
    std::cout << "Enter format file to decode:" << "\n";
    int i = 1;
    std::cout << "(" << i++ << ")" << " PSD" << "\n";
    std::cout << "(" << i++ << ")" << " Waveform" << "\n";

    int val;
    std::cin >> val;
    i = 1;
    if (val == i++) fTypes.push_back("PSD");
    if (val == i++) fTypes.push_back("Waveform");
}

void Parser::DigitizerConsoleParser::ReadWriter() {
    std::cout << "Choose format to output:" << "\n";
    int i = 0;
    std::cout << "(" << i++ << ") Root" << "\n";
    std::cout << "(" << i++ << ") Txt" << "\n";
    std::cout << "(" << i++ << ") Root and Txt" << "\n";

    i = 0;
    int val;
    std::cin >> val;
    if (val == i++) SetUsedWriterVector({"Root"});
    if (val == i++) SetUsedWriterVector({"Txt"});
    if (val == i++) SetUsedWriterVector({"Root", "Txt"});
}

void Parser::DigitizerConsoleParser::ReadDataPSD() {
    std::cout << "\n";
    std::cout << "Enter parameters to use:" << "\n";
    int i = 0;
    std::cout << "(" << i++ << ")" << " all" << "\n";
    std::cout << "(" << i++ << ")" << " qShort" << "\n";
    std::cout << "(" << i++ << ")" << " qLong" << "\n";
    std::cout << "(" << i++ << ")" << " cfd_y1" << "\n";
    std::cout << "(" << i++ << ")" << " cfd_y2" << "\n";
    std::cout << "(" << i++ << ")" << " baseline" << "\n";
    std::cout << "(" << i++ << ")" << " height" << "\n";
    std::cout << "(" << i++ << ")" << " eventCounter" << "\n";
    std::cout << "(" << i++ << ")" << " eventCounterPSD" << "\n";
    std::cout << "(" << i++ << ")" << " psdValue" << "\n";

    std::string val;
    std::cin >> val;
    for (int k = 0; k < val.length(); k++) {
        std::string tmp(1, val[k]);
        int f = 0;
        if (tmp == std::to_string(f++)) {
            usedPar.qShort = DEFAULT_VALUE;
            usedPar.qLong = DEFAULT_VALUE;
            usedPar.cfd_y1 = DEFAULT_VALUE;
            usedPar.cfd_y2 = DEFAULT_VALUE;
            usedPar.baselinePSD = DEFAULT_VALUE;
            usedPar.height = DEFAULT_VALUE;
            usedPar.eventCounter = DEFAULT_VALUE;
            usedPar.eventCounterPSD = DEFAULT_VALUE;
            usedPar.psdValue = DEFAULT_VALUE;
        }
        if (tmp == std::to_string(f++)) usedPar.qShort = DEFAULT_VALUE;
        if (tmp == std::to_string(f++)) usedPar.qLong = DEFAULT_VALUE;
        if (tmp == std::to_string(f++)) usedPar.cfd_y1 = DEFAULT_VALUE;
        if (tmp == std::to_string(f++)) usedPar.cfd_y2 = DEFAULT_VALUE;
        if (tmp == std::to_string(f++)) usedPar.baselinePSD = DEFAULT_VALUE;
        if (tmp == std::to_string(f++)) usedPar.height = DEFAULT_VALUE;
        if (tmp == std::to_string(f++)) usedPar.eventCounter = DEFAULT_VALUE;
        if (tmp == std::to_string(f++)) usedPar.eventCounterPSD = DEFAULT_VALUE;
        if (tmp == std::to_string(f++)) usedPar.psdValue = DEFAULT_VALUE;
    }
}

void Parser::DigitizerConsoleParser::ReadDataWaveform() {
    std::cout << "\n";
    std::cout << "Enter parameters to use:" << "\n";
    int i = 0;
    std::cout << "(" << i++ << ")" << " all" << "\n";
    std::cout << "(" << i++ << ")" << " baseline" << "\n";
    std::cout << "(" << i++ << ")" << " charge" << "\n";
    std::cout << "(" << i++ << ")" << " amplitude" << "\n";

    std::string val;
    std::cin >> val;
    for (int k = 0; k < val.length(); k++) {
        std::string tmp(1, val[k]);
        int f = 0;
        if (tmp == std::to_string(f++)) {
            usedPar.baseline = DEFAULT_VALUE;
            usedPar.charge = DEFAULT_VALUE;
            usedPar.amplitude = DEFAULT_VALUE;
        }
        if (tmp == std::to_string(f++)) usedPar.baseline = DEFAULT_VALUE;
        if (tmp == std::to_string(f++)) usedPar.charge = DEFAULT_VALUE;
        if (tmp == std::to_string(f++)) usedPar.amplitude = DEFAULT_VALUE;
    }
}

void Parser::DigitizerConsoleParser::ReadHistograms() {

}

void Parser::DigitizerConsoleParser::ReadConfig() {
    if (usedPar.baseline.has_value() || usedPar.charge.has_value()) {
        std::cout << "\n";
        std::cout << "Configure limits for charge algorithm" << "\n";

        std::cout << "Enter min value point for baseline" << "\n";
        int16_t min;
        std::cin >> min;

        std::cout << "Enter max value point for baseline" << "\n";
        int16_t max;
        std::cin >> max;

        usedPar.baselineLimits = std::make_pair(min, max);
    }
    if (usedPar.charge.has_value()) {
        std::cout << "Enter min value point for charge" << "\n";
        int16_t min;
        std::cin >> min;

        std::cout << "Enter max value point for charge" << "\n";
        int16_t max;
        std::cin >> max;

        usedPar.chargeLimits = std::make_pair(min, max);
    }
}