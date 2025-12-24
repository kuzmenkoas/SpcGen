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
    SetDigitizerTypes(fTypes);
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
    for (std::string file : fTypes) {
        if (usedPar.baseline.has_value() || usedPar.charge.has_value() || 
            usedPar.amplitude.has_value() || usedPar.scaler.has_value() ) {
            std::cout << "\n";
            std::cout << "Choose parameters to configure histogram" << std::endl;
            int i = 0;
            std::cout << "(" << i++ << ") without" << "\n";
            if (file == "PSD") {
                std::cout << "PSD configuration:" << std::endl;
                if (usedPar.qShort.has_value()) std::cout << "(" << i++ << ") qShort" << "\n";
                if (usedPar.qLong.has_value()) std::cout << "(" << i++ << ") qLong" << "\n";
                if (usedPar.cfd_y1.has_value()) std::cout << "(" << i++ << ") cfd_y1" << "\n";
                if (usedPar.cfd_y2.has_value()) std::cout << "(" << i++ << ") cfd_y2" << "\n";
                if (usedPar.baseline.has_value()) std::cout << "(" << i++ << ") baseline" << "\n";
                if (usedPar.height.has_value()) std::cout << "(" << i++ << ") height" << "\n";
                if (usedPar.eventCounter.has_value()) std::cout << "(" << i++ << ") eventCounter" << "\n";
                if (usedPar.eventCounterPSD.has_value()) std::cout << "(" << i++ << ") eventCounterPSD" << "\n";
                if (usedPar.psdValue.has_value()) std::cout << "(" << i++ << ") psdValue" << "\n";

                std::string val;
                std::cin >> val;

                for (int k = 0; k < val.length(); k++) {
                    std::string tmp(1, val[k]);
                    int f = 0;
                    if (tmp == std::to_string(f++)) break;
                    if (tmp == std::to_string(f++)) SetHistogramVector(file, "qShort");
                    if (tmp == std::to_string(f++)) SetHistogramVector(file, "qLong");
                    if (tmp == std::to_string(f++)) SetHistogramVector(file, "cfd_y1");
                    if (tmp == std::to_string(f++)) SetHistogramVector(file, "cfd_y2");
                    if (tmp == std::to_string(f++)) SetHistogramVector(file, "baseline");
                    if (tmp == std::to_string(f++)) SetHistogramVector(file, "height");
                    if (tmp == std::to_string(f++)) SetHistogramVector(file, "eventCounter");
                    if (tmp == std::to_string(f++)) SetHistogramVector(file, "eventCounterPSD");
                    if (tmp == std::to_string(f++)) SetHistogramVector(file, "psdValue");
                }
            } else if (file == "Waveform") {
                std::cout << "Waveform configuration:" << std::endl;
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
                    if (tmp == std::to_string(f++)) SetHistogramVector(file, "baseline");
                    if (tmp == std::to_string(f++)) SetHistogramVector(file, "charge");
                    if (tmp == std::to_string(f++)) SetHistogramVector(file, "amplitude");
                    if (tmp == std::to_string(f++)) SetHistogramVector(file, "scaler");
                }
            }
        }
    }
}

void Parser::DigitizerConsoleParser::SetHistogramVector(std::string file, std::string parameter) {
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

    Global::IHist Hist{file, parameter, Nbins, min, max};
    if (!usedPar.hist.has_value()) {
        usedPar.hist = {Hist};
    } else {
        auto& v = *usedPar.hist;
        v.push_back(Hist);
    }
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

        std::cout << "Enter factor for charge" << "\n";
        double factor;
        std::cin >> factor;
        usedPar.factorCharge = factor;

        std::cout << "Enter shift for charge" << "\n";
        double shift;
        std::cin >> shift;
        usedPar.shiftCharge = shift;
    }
    if (usedPar.amplitude.has_value()) {
        std::cout << "Enter factor for amplitude" << "\n";
        double factor;
        std::cin >> factor;
        usedPar.factorAmplitude = factor;

        std::cout << "Enter shift for amplitude" << "\n";
        double shift;
        std::cin >> shift;
        usedPar.shiftAmplitude = shift;
    }
}