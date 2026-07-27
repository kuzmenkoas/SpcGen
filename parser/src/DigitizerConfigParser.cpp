#include "DigitizerConfigParser.h"
using namespace std::string_literals;

Parser::DigitizerConfigParser::DigitizerConfigParser(std::filesystem::path path, std::vector<std::string> aTypes) : fPath(path), fTypes(aTypes) {
}

Parser::DigitizerConfigParser::~DigitizerConfigParser() {
}

void Parser::DigitizerConfigParser::Start() {
    ReadWriter();
    if (fTypes.empty()) ReadFileType();
    for (std::string file : fTypes) {
        if (file == "PSD"s) ReadDataPSD();
        if (file == "Waveform"s) {
            ReadDataWaveform();
            ReadCharge();
            ReadAmplitude();
            ReadConfig();
        }
    }
    ReadHistograms();

    SetUsedParameters(usedPar);
}

void Parser::DigitizerConfigParser::ReadFileType() {
    std::string keyPSD = "DataPSD";
    std::string keyWaveform = "DataWaveform";

    std::ifstream file = OpenFile();
    std::string CurStr;
    try {
        while(getline (file,CurStr)) {
            if (ClearString(CurStr).compare(0, keyPSD.size(), keyPSD) == 0) {
                fTypes.push_back("PSD");
            } else if (ClearString(CurStr).compare(0, keyWaveform.size(), keyWaveform) == 0) {
                fTypes.push_back("Waveform");
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        abort();
    }
    SetDigitizerTypes(fTypes);
    file.close();
}

std::ifstream Parser::DigitizerConfigParser::OpenFile() {
    std::ifstream file(fPath.string());
    try {
        if (!file.is_open()) throw std::runtime_error("Cannot open config file " + fPath.string());
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << '\n';
        abort();
    }
    return file;
}

void Parser::DigitizerConfigParser::ReadWriter(std::string key) {
    std::ifstream file = OpenFile();
    std::string CurStr;
    std::vector<std::string> writers;
    try {
        while(getline(file,CurStr)){
            if (ClearString(CurStr).compare(0, key.size(), key) == 0) {
                while (getline (file, CurStr)) {
                    if (CurStr.c_str()[0]=='+') {
                        size_t found = CurStr.find_first_of(" ");
                        CurStr = CurStr.substr(found+1);

                        if (ClearString(CurStr) == "Root"s) writers.push_back("Root");
                        if (ClearString(CurStr) == "Txt"s) writers.push_back("Txt");
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

void Parser::DigitizerConfigParser::ReadDataPSD(std::string key) {
    std::ifstream file = OpenFile();
    std::string CurStr;
    try {
        while(getline (file,CurStr)){
            if (ClearString(CurStr).compare(0, key.size(), key) == 0) {
                while (getline (file, CurStr)) {
                    if (CurStr.c_str()[0]=='+') {
                        size_t found = CurStr.find_first_of(" ");
                        CurStr = CurStr.substr(found+1);
                        if (ClearString(CurStr) == "qShort"s) usedPar.qShort = DEFAULT_VALUE;
                        if (ClearString(CurStr) == "qLong"s) usedPar.qLong = DEFAULT_VALUE;
                        if (ClearString(CurStr) == "cfd_y1"s) usedPar.cfd_y1 = DEFAULT_VALUE;
                        if (ClearString(CurStr) == "cfd_y2"s) usedPar.cfd_y2 = DEFAULT_VALUE;
                        if (ClearString(CurStr) == "baseline"s) usedPar.baselinePSD = DEFAULT_VALUE;
                        if (ClearString(CurStr) == "height"s) usedPar.height = DEFAULT_VALUE;
                        if (ClearString(CurStr) == "eventCounter"s) usedPar.eventCounter = DEFAULT_VALUE;
                        if (ClearString(CurStr) == "eventCounterPSD"s) usedPar.eventCounterPSD = DEFAULT_VALUE;
                        if (ClearString(CurStr) == "psdValue"s) usedPar.psdValue = DEFAULT_VALUE;
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

void Parser::DigitizerConfigParser::ReadDataWaveform(std::string key) {
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

void Parser::DigitizerConfigParser::ReadHistograms(std::string key) {
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
                        std::string file = CurStr.substr(0, CurStr.find_first_of(" "));
                        std::string tmp = CurStr.substr(CurStr.find_first_of(" ")+1);
                        std::string parameter = tmp.substr(0, tmp.find_first_of(" "));
                        tmp = tmp.substr(tmp.find_first_of(" ")+1);
                        std::string Nbins = tmp.substr(0, tmp.find_first_of(" "));
                        tmp = tmp.substr(tmp.find_first_of(" ")+1);
                        std::string min = tmp.substr(0, tmp.find_first_of(" "));
                        tmp = tmp.substr(tmp.find_first_of(" ")+1);
                        std::string max = tmp.substr(0, tmp.find_first_of(" "));

                        if (ClearString(parameter) == "qShort"s) hist.push_back(Global::IHist{file, ClearString(parameter), std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (ClearString(parameter) == "qLong"s) hist.push_back(Global::IHist{file, ClearString(parameter), std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (ClearString(parameter) == "cfd_y1"s) hist.push_back(Global::IHist{file, ClearString(parameter), std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (ClearString(parameter) == "cfd_y2"s) hist.push_back(Global::IHist{file, ClearString(parameter), std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (ClearString(parameter) == "height"s) hist.push_back(Global::IHist{file, ClearString(parameter), std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (ClearString(parameter) == "eventCounter"s) hist.push_back(Global::IHist{file, ClearString(parameter), std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (ClearString(parameter) == "eventCounterPSD"s) hist.push_back(Global::IHist{file, ClearString(parameter), std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (ClearString(parameter) == "psdValue"s) hist.push_back(Global::IHist{file, ClearString(parameter), std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (ClearString(parameter) == "baseline"s) hist.push_back(Global::IHist{file, ClearString(parameter), std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (ClearString(parameter) == "charge"s) hist.push_back(Global::IHist{file, ClearString(parameter), std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (ClearString(parameter) == "amplitude"s) hist.push_back(Global::IHist{file, ClearString(parameter), std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (ClearString(parameter) == "scaler"s) hist.push_back(Global::IHist{file, ClearString(parameter), std::stoi(Nbins), std::stod(min), std::stod(max)});
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

void Parser::DigitizerConfigParser::ReadCharge(std::string key) {
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

void Parser::DigitizerConfigParser::ReadConfig(std::string key) {
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
                        if (ClearString(parameter) == "charge_range"s) usedPar.chargeLimits = std::make_pair(std::stoi(val), std::stoi(val2));
                        if (ClearString(parameter) == "amplitude_range"s) usedPar.amplitudeLimits = std::make_pair(std::stoi(val), std::stoi(val2));
                        if (ClearString(parameter) == "baseline"s) usedPar.baselineLimits = std::make_pair(std::stoi(val), std::stoi(val2));
                        if (ClearString(parameter) == "wavelength"s) usedPar.wavelength = std::stod(val);
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

void Parser::DigitizerConfigParser::ReadAmplitude(std::string key) {
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

                        if (ClearString(range) == "up"s || range == "down"s) usedPar.signal = ClearString(CurStr);
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