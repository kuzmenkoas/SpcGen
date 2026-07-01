#include "DRSConfigParser.h"
using namespace std::string_literals;

Parser::DRSConfigParser::DRSConfigParser(std::filesystem::path path) : fPath(path) {
}

Parser::DRSConfigParser::~DRSConfigParser() {
}

void Parser::DRSConfigParser::Start() {
    ReadWriter();
    ReadData();
    ReadCharge();
    ReadHistograms();
    ReadAmplitude();
    ReadConfig();

    SetUsedParameters(usedPar);
}

std::ifstream Parser::DRSConfigParser::OpenFile() {
    std::ifstream file(fPath.string());
    try {
        if (!file.is_open()) throw std::runtime_error("Cannot open config file " + fPath.string());
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << '\n';
        abort();
    }
    return file;
}

void Parser::DRSConfigParser::ReadWriter(std::string key) {
    std::ifstream file = OpenFile();
    std::string CurStr;
    std::vector<std::string> writers;
    try {
        while(getline (file,CurStr)){
            if (ClearString(CurStr).compare(0, key.size(), key) == 0) {
                while (getline (file, CurStr)) {
                    if (CurStr.c_str()[0]=='+') {
                        size_t found = CurStr.find_first_of(" ");
                        CurStr = CurStr.substr(found+1);
                        if (ClearString(CurStr) == "Root"s) writers.push_back("Root"s);
                        if (ClearString(CurStr) == "Txt"s) writers.push_back("Txt"s);
                    } else break;
                }
            }
        }
    } catch (const std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      abort();
    }
    SetUsedWriterVector(writers);
    file.close();
}

void Parser::DRSConfigParser::ReadData(std::string key) {
    std::ifstream file = OpenFile();
    std::string CurStr;
    try {
        while(getline (file,CurStr)){
            if (ClearString(CurStr).compare(0, key.size(), key) == 0) {
                while (getline (file, CurStr)) {
                    if (CurStr.c_str()[0]=='+') {
                        size_t found = CurStr.find_first_of(" ");
                        CurStr = CurStr.substr(found+1);
                        if (ClearString(CurStr) == "baseline"s) usedPar.baseline = DEFAULT_VALUE;
                        if (ClearString(CurStr) == "charge"s) usedPar.charge = DEFAULT_VALUE;
                        if (ClearString(CurStr) == "amplitude"s) usedPar.amplitude = DEFAULT_VALUE;
                        if (ClearString(CurStr) == "waveform"s) usedPar.waveform = {DEFAULT_VALUE};
                        if (ClearString(CurStr) == "scaler"s) usedPar.scaler = DEFAULT_VALUE;
                        if (ClearString(CurStr) == "time"s) usedPar.time = DEFAULT_VALUE;
                    } else break;
                }
            }
        }
    } catch (const std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      abort();
    }
    file.close();
}

void Parser::DRSConfigParser::ReadCharge(std::string key) {
    std::ifstream file = OpenFile();
    std::string CurStr;
    try {
        while(getline (file,CurStr)){
            if (ClearString(CurStr).compare(0, key.size(), key) == 0) {
                while (getline (file, CurStr)) {
                    if (CurStr.c_str()[0]=='+') {
                        size_t found = CurStr.find_first_of(" ");
                        CurStr = CurStr.substr(found+1);
                        std::string parameter = CurStr.substr(0, CurStr.find_first_of(" "));
                        std::string tmp = CurStr.substr(CurStr.find_first_of(" ")+1);
                        std::string min = tmp.substr(0, tmp.find_first_of(" "));

                        if (ClearString(parameter) == "factor"s) usedPar.factorCharge = std::stod(min);
                        if (ClearString(parameter) == "shift"s) usedPar.shiftCharge = std::stod(min);
                    } else break;
                }
            }
        }
    } catch (const std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      abort();
    }
    file.close();
}

void Parser::DRSConfigParser::ReadConfig(std::string key) {
    std::ifstream file = OpenFile();
    std::string CurStr;
    try {
        while(getline (file,CurStr)){
            if (ClearString(CurStr).compare(0, key.size(), key) == 0) {
                while (getline (file, CurStr)) {
                    if (CurStr.c_str()[0]=='+') {
                        size_t found = CurStr.find_first_of(" ");
                        CurStr = CurStr.substr(found+1);
                        std::string parameter = CurStr.substr(0, CurStr.find_first_of(" "));
                        std::string tmp = CurStr.substr(CurStr.find_first_of(" ")+1);
                        std::string val = tmp.substr(0, tmp.find_first_of(" "));
                        tmp = tmp.substr(tmp.find_first_of(" ")+1);
                        std::string val2 = tmp.substr(0, tmp.find_first_of(" "));
                        if (ClearString(parameter) == "cut"s) usedPar.cut = std::stod(val);
                        if (ClearString(parameter) == "range"s) usedPar.signalRange = std::make_pair(std::stoi(val), std::stoi(val2));
                        if (ClearString(parameter) == "baseline"s) usedPar.baselineLimits = std::make_pair(std::stoi(val), std::stoi(val2));
                        if (ClearString(parameter) == "threshold"s) usedPar.threshold = std::stod(val);
                    } else break;
                }
            }
        }
    } catch (const std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      abort();
    }
    file.close();
}

void Parser::DRSConfigParser::ReadHistograms(std::string key) {
    std::ifstream file = OpenFile();
    std::string CurStr;
    std::vector<Global::IHist> hist; 
    try {
        while(getline (file,CurStr)){
            if (ClearString(CurStr).compare(0, key.size(), key) == 0) {
                while (getline (file, CurStr)) {
                    if (CurStr.c_str()[0]=='+') {
                        size_t found = CurStr.find_first_of(" ");
                        CurStr = CurStr.substr(found+1);
                        std::string parameter = CurStr.substr(0, CurStr.find_first_of(" "));
                        std::string tmp = CurStr.substr(CurStr.find_first_of(" ")+1);
                        std::string Nbins = tmp.substr(0, tmp.find_first_of(" "));
                        tmp = tmp.substr(tmp.find_first_of(" ")+1);
                        std::string min = tmp.substr(0, tmp.find_first_of(" "));
                        tmp = tmp.substr(tmp.find_first_of(" ")+1);
                        std::string max = tmp.substr(0, tmp.find_first_of(" "));

                        if (ClearString(parameter) == "baseline"s) hist.push_back(Global::IHist{"DRS"s, ClearString(parameter), std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (ClearString(parameter) == "charge"s) hist.push_back(Global::IHist{"DRS"s, ClearString(parameter), std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (ClearString(parameter) == "amplitude"s) hist.push_back(Global::IHist{"DRS"s, ClearString(parameter), std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (ClearString(parameter) == "scaler"s) hist.push_back(Global::IHist{"DRS"s, ClearString(parameter), std::stoi(Nbins), std::stod(min), std::stod(max)});
                    } else break;
                }
            }
        }
    } catch (const std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      abort();
    }
    if (!hist.empty()) usedPar.hist = hist;
    file.close();
}

void Parser::DRSConfigParser::ReadAmplitude(std::string key) {
    std::ifstream file = OpenFile();
    std::string CurStr;
    try {
        while(getline (file,CurStr)){
            if (ClearString(CurStr).compare(0, key.size(), key) == 0) {
                while (getline (file, CurStr)) {
                    if (CurStr.c_str()[0]=='+') {
                        size_t found = CurStr.find_first_of(" ");
                        CurStr = CurStr.substr(found+1);
                        std::string range = CurStr.substr(0, CurStr.find_first_of(" "));
                        std::string tmp = CurStr.substr(CurStr.find_first_of(" ")+1);
                        std::string lRange = tmp.substr(0, tmp.find_first_of(" "));
                        if (ClearString(range) == "up"s || ClearString(range) == "down"s) usedPar.signal = ClearString(CurStr);
                        if (ClearString(range) == "factor"s) usedPar.factorAmplitude = std::stod(lRange);
                        if (ClearString(range) == "shift"s) usedPar.shiftAmplitude = std::stod(lRange);
                    } else break;
                }
            }
        }
    } catch (const std::exception& e) {
      std::cerr << "Exception: " << e.what() << std::endl;
      abort();
    }
    file.close();
}