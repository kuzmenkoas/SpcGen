#include "ArgReader.h"
#include <iostream>
#include <filesystem>

Core::ArgReader::ArgReader(int argc, char *argv[]) {
        ParseThreads(argc, argv);
        if (ParseDRSBinaryFile(argc, argv)) fDeviceType = Global::DeviceType::DRS;
        else if (ParseDigitizerBinaryFile(argc, argv)) {
            fDeviceType = Global::DeviceType::Digitizer;
        }
        ParseConfigFile(argc, argv);
}

Core::ArgReader::~ArgReader() {
}

// Parse number of threads if exists in argument with key
// input must be as -key+Int, key is threadKey (now j)
void Core::ArgReader::ParseThreads(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        std::string name = argv[i];
        if ((name.substr(0,threadKey.size()) == threadKey) && (name.size() > threadKey.size())) {
            try {
                // fConfigurator->SetThreadNumber(std::stoi(name.substr(threadKey.size(), name.size())));
            } catch (...) {
                std::cerr << "Warning: Invalid thread number. Use default thread number (1)." << std::endl;
            }
            break;
        }
    }
}

bool Core::ArgReader::ParseDRSBinaryFile(int argc, char *argv[]) {
    bool is = false;
    for (int i = 1; i < argc; i++) {
        std::string name = argv[i];
        if (name.size() > binaryDRSExtension.size()+1) {
            if (name.substr(name.size()-binaryDRSExtension.size(), name.size()) == binaryDRSExtension) {
                fBinaryPathVector.push_back(name);
                SetDRSBinaryFileName(name);
                is = true;
                break;
            }
        }
    }
    return is;
}

bool Core::ArgReader::ParseDigitizerBinaryFile(int argc, char *argv[]) {
    bool is = false;
    int counter = 0;
    for (int i = 1; i < argc; i++) {
        std::string name = argv[i];
        if (name.size() > binaryDigitizerExtension.size()+1) {
            if (name.substr(name.size()-binaryDigitizerExtension.size(), name.size()) == binaryDigitizerExtension) {
                fBinaryPathVector.push_back(name);
                std::call_once(initDigitizerFlag, [this, name](){SetDigitizerBinaryFileName(name);});
                is = true;
                counter++;
            }
        }
    }
    if (counter == 2) fDigitizerTypes = {"PSD", "Waveform"};
    return is;
}

void Core::ArgReader::ParseConfigFile(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        std::string name = argv[i];
        if (name.size() > (configExtension.size()+1)) {
            if (name.substr(name.size()-configExtension.size(), name.size()) == configExtension) {
                fConfigPath = name;
                break;
            }
        }
    }
}

void Core::ArgReader::SetDRSBinaryFileName(std::string name) {
    name = GetBinaryFileName(name);
    name = name.substr(0, name.size()-(binaryDRSExtension.size()+1));
    fFileName = name;
}

void Core::ArgReader::SetDigitizerBinaryFileName(std::string name) {
    name = GetBinaryFileName(name);
    name = name.substr(0, name.size()-(binaryDigitizerExtension.size()+1));
    fFileName = name;
}

std::string Core::ArgReader::GetBinaryFileName(std::string name) {
    std::filesystem::path path(name);
    return path.filename().string();
}