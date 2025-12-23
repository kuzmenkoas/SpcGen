#include "DRSConsoleParser.h"

Parser::DRSConsoleParser::DRSConsoleParser() {
}

Parser::DRSConsoleParser::~DRSConsoleParser() {
}

void Parser::DRSConsoleParser::Start() {
    ReadWriter();
    ReadData();
    ReadConfig();
    ReadHistograms();
    if (usedPar.amplitude.has_value()) ReadSignal();

    SetUsedParameters(usedPar);
}

void Parser::DRSConsoleParser::ReadWriter() {
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

void Parser::DRSConsoleParser::ReadData() {
    std::cout << "\n";
    std::cout << "Enter parameters to use:" << "\n";
    int i = 0;
    std::cout << "(" << i++ << ")" << " all" << "\n";
    std::cout << "(" << i++ << ")" << " baseline" << "\n";
    std::cout << "(" << i++ << ")" << " charge" << "\n";
    std::cout << "(" << i++ << ")" << " amplitude" << "\n";
    std::cout << "(" << i++ << ")" << " waveform" << "\n";
    std::cout << "(" << i++ << ")" << " scaler" << "\n";
    std::cout << "(" << i++ << ")" << " time" << "\n";

    std::string val;
    std::cin >> val;
    for (int k = 0; k < val.length(); k++) {
        std::string tmp(1, val[k]);
        int f = 0;
        if (tmp == std::to_string(f++)) {
            usedPar.baseline = DEFAULT_VALUE;
            usedPar.charge = DEFAULT_VALUE;
            usedPar.amplitude = DEFAULT_VALUE;
            usedPar.waveform = {DEFAULT_VALUE};
            usedPar.scaler = DEFAULT_VALUE;
            usedPar.time = DEFAULT_VALUE;
        }
        if (tmp == std::to_string(f++)) usedPar.baseline = DEFAULT_VALUE;
        if (tmp == std::to_string(f++)) usedPar.charge = DEFAULT_VALUE;
        if (tmp == std::to_string(f++)) usedPar.amplitude = DEFAULT_VALUE;
        if (tmp == std::to_string(f++)) usedPar.waveform = {DEFAULT_VALUE};
        if (tmp == std::to_string(f++)) usedPar.scaler = DEFAULT_VALUE;
        if (tmp == std::to_string(f++)) usedPar.time = DEFAULT_VALUE;
    }
}

void Parser::DRSConsoleParser::ReadConfig() {
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

        std::cout << "Enter factor for charge" << "\n";
        double factor;
        std::cin >> factor;
        usedPar.factor = factor;

        std::cout << "Enter shift for charge" << "\n";
        double shift;
        std::cin >> shift;
        usedPar.shift = shift;
    }
}

void Parser::DRSConsoleParser::ReadHistograms() {
    if (usedPar.baseline.has_value() || usedPar.charge.has_value() || 
        usedPar.amplitude.has_value() || usedPar.scaler.has_value() ) {
        std::cout << "\n";
        std::cout << "Choose parameters to configure histogram" << std::endl;
        int i = 0;
        std::cout << "(" << i++ << ") without" << "\n";
        if (usedPar.baseline.has_value()) std::cout << "(" << i++ << ") baseline" << "\n";
        if (usedPar.charge.has_value()) std::cout << "(" << i++ << ") charge" << "\n";
        if (usedPar.amplitude.has_value()) std::cout << "(" << i++ << ") amplitude" << "\n";
        if (usedPar.scaler.has_value()) std::cout << "(" << i++ << ") scaler" << "\n";

        std::string val;
        std::cin >> val;

        for (int k = 0; k < val.length(); k++) {
            std::string tmp(1, val[k]);
            int f = 0;
            if (tmp == std::to_string(f++)) break;
            if (tmp == std::to_string(f++)) SetHistogramVector("baseline");
            if (tmp == std::to_string(f++)) SetHistogramVector("charge");
            if (tmp == std::to_string(f++)) SetHistogramVector("amplitude");
            if (tmp == std::to_string(f++)) SetHistogramVector("scaler");
        }
    }
}

void Parser::DRSConsoleParser::SetHistogramVector(std::string parameter) {
    std::cout << "\n";
    std::cout << "Set number of bins for " << parameter << std::endl;
    int16_t Nbins;
    std::cin >> Nbins;

    std::cout << "Set minimal value for " << parameter << std::endl;
    double min;
    std::cin >> min;
    std::cout << "Set maximal value for " << parameter << std::endl;
    double max;
    std::cin >> max;

    Global::IHist Hist{"DRS", parameter, Nbins, min, max};
    if (!usedPar.hist.has_value()) {
        usedPar.hist = {Hist};
    } else {
        auto& v = *usedPar.hist;
        v.push_back(Hist);
    }
}

void Parser::DRSConsoleParser::ReadSignal() {
    std::cout << "\n";
    std::cout << "Choose which form of signal:" << "\n";
    int i = 1;
    std::cout << "(" << i++ << ") up" << "\n";
    std::cout << "(" << i++ << ") down" << "\n";

    int val;
    std::cin >> val;
    i = 1;
    if (val == i++) usedPar.signal = "up";
    if (val == i++) usedPar.signal = "down";

    std::cout << "Enter left range for amplitude fit" << "\n";
    int lRange;
    std::cin >> lRange;

    std::cout << "Enter right range for amplitude fit" << "\n";
    int rRange;
    std::cin >> rRange;

    usedPar.signalRange = std::make_pair(lRange, rRange);
}