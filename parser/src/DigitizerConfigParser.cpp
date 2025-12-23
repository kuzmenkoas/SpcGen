#include "DigitizerConfigParser.h"

Parser::DigitizerConfigParser::DigitizerConfigParser(std::filesystem::path path, std::vector<std::string> aTypes) : fPath(path), fTypes(aTypes) {
}

Parser::DigitizerConfigParser::~DigitizerConfigParser() {
}

void Parser::DigitizerConfigParser::Start() {
    ReadWriter();
    ReadDataPSD();
    ReadDataWaveform();
    ReadHistograms();
    ReadConfig();
    ReadSignal();

    SetUsedParameters(usedPar);
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
        while(getline (file,CurStr)){
            if (CurStr.compare(0, key.size(), key) == 0) {
                while (getline (file, CurStr)) {
                    if (CurStr.c_str()[0]=='+') {
                        size_t found = CurStr.find_first_of(" ");
                        CurStr = CurStr.substr(found+1);

                        if (CurStr == "Root") writers.push_back("Root");
                        if (CurStr == "Txt") writers.push_back("Txt");
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
            if (CurStr.compare(0, key.size(), key) == 0) {
                while (getline (file, CurStr)) {
                    if (CurStr.c_str()[0]=='+') {
                        size_t found = CurStr.find_first_of(" ");
                        CurStr = CurStr.substr(found+1);
                        if (CurStr == "qShort") usedPar.qShort = DEFAULT_VALUE;
                        if (CurStr == "qLong") usedPar.qLong = DEFAULT_VALUE;
                        if (CurStr == "cfd_y1") usedPar.cfd_y1 = DEFAULT_VALUE;
                        if (CurStr == "cfd_y2") usedPar.cfd_y2 = DEFAULT_VALUE;
                        if (CurStr == "baseline") usedPar.baselinePSD = DEFAULT_VALUE;
                        if (CurStr == "height") usedPar.height = DEFAULT_VALUE;
                        if (CurStr == "eventCounter") usedPar.eventCounter = DEFAULT_VALUE;
                        if (CurStr == "eventCounterPSD") usedPar.eventCounterPSD = DEFAULT_VALUE;
                        if (CurStr == "psdValue") usedPar.psdValue = DEFAULT_VALUE;
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
            if (CurStr.compare(0, key.size(), key) == 0) {
                while (getline (file, CurStr)) {
                    if (CurStr.c_str()[0]=='+') {
                        size_t found = CurStr.find_first_of(" ");
                        CurStr = CurStr.substr(found+1);
                        if (CurStr == "baseline") usedPar.baseline = DEFAULT_VALUE;
                        if (CurStr == "charge") usedPar.charge = DEFAULT_VALUE;
                        if (CurStr == "amplitude") usedPar.amplitude = DEFAULT_VALUE;
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
            if (CurStr.compare(0, key.size(), key) == 0) {
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

                        if (parameter == "qShort") hist.push_back(Global::IHist{file, parameter, std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (parameter == "qLong") hist.push_back(Global::IHist{file, parameter, std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (parameter == "cfd_y1") hist.push_back(Global::IHist{file, parameter, std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (parameter == "cfd_y2") hist.push_back(Global::IHist{file, parameter, std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (parameter == "height") hist.push_back(Global::IHist{file, parameter, std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (parameter == "eventCounter") hist.push_back(Global::IHist{file, parameter, std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (parameter == "eventCounterPSD") hist.push_back(Global::IHist{file, parameter, std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (parameter == "psdValue") hist.push_back(Global::IHist{file, parameter, std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (parameter == "baseline") hist.push_back(Global::IHist{file, parameter, std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (parameter == "charge") hist.push_back(Global::IHist{file, parameter, std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (parameter == "amplitude") hist.push_back(Global::IHist{file, parameter, std::stoi(Nbins), std::stod(min), std::stod(max)});
                        if (parameter == "scaler") hist.push_back(Global::IHist{file, parameter, std::stoi(Nbins), std::stod(min), std::stod(max)});
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

void Parser::DigitizerConfigParser::ReadConfig(std::string key) {
    std::ifstream file = OpenFile();
    std::string CurStr;
    try {
        while(getline (file,CurStr)){
            if (CurStr.compare(0, key.size(), key) == 0) {
                while (getline (file, CurStr)) {
                    if (CurStr.c_str()[0]=='+') {
                        size_t found = CurStr.find_first_of(" ");
                        CurStr = CurStr.substr(found+1);
                        std::string parameter = CurStr.substr(0, CurStr.find_first_of(" "));
                        std::string tmp = CurStr.substr(CurStr.find_first_of(" ")+1);
                        std::string min = tmp.substr(0, tmp.find_first_of(" "));
                        tmp = tmp.substr(tmp.find_first_of(" ")+1);
                        std::string max = tmp.substr(0, tmp.find_first_of(" "));
                        tmp = tmp.substr(tmp.find_first_of(" ")+1);
                        
                        if (parameter == "baseline") usedPar.baselineLimits = std::make_pair(std::stoi(min), std::stoi(max));
                        if (parameter == "charge") usedPar.chargeLimits = std::make_pair(std::stoi(min), std::stoi(max));
                        if (parameter == "wavelength") usedPar.wavelength = std::stoi(min);
                        if (parameter == "factor") usedPar.factor = std::stod(min);
                        if (parameter == "shift") usedPar.shift = std::stod(min);
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

void Parser::DigitizerConfigParser::ReadSignal(std::string key) {
    std::ifstream file = OpenFile();
    std::string CurStr;
    try {
        while(getline (file,CurStr)){
            if (CurStr.compare(0, key.size(), key) == 0) {
                while (getline (file, CurStr)) {
                    if (CurStr.c_str()[0]=='+') {
                        size_t found = CurStr.find_first_of(" ");
                        CurStr = CurStr.substr(found+1);
                        if (CurStr == "up" || CurStr == "down") usedPar.signal = CurStr;
                        else {
                            std::string range = CurStr.substr(0, CurStr.find_first_of(" "));
                            if (range == "range") {
                                std::string tmp = CurStr.substr(CurStr.find_first_of(" ")+1);
                                std::string lRange = tmp.substr(0, tmp.find_first_of(" "));
                                tmp = tmp.substr(tmp.find_first_of(" ")+1);
                                std::string rRange = tmp.substr(0, tmp.find_first_of(" "));
                                usedPar.signalRange = std::make_pair(std::stoi(lRange), std::stoi(rRange));
                            } else {
                                std::cerr << "Warning" << std::endl;
                            }
                        }
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