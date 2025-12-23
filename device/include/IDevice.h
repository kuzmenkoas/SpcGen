#pragma once

#include <memory>
#include <vector>
#include "TFile.h"
#include "ParserFactory.h"

namespace Device {
    class IDevice {
    public:
        IDevice();
        ~IDevice();

        void SetConfigPath(std::filesystem::path path) {fConfigPath = path;};
        std::filesystem::path GetConfigPath() {return fConfigPath;};

        void SetBinaryPathVector(std::vector<std::filesystem::path> pathVector) {fBinaryPathVector = pathVector;};
        std::vector<std::filesystem::path> GetBinaryPathVector() {return fBinaryPathVector;};
        
        void SetFileName(std::string name) {fFileName = name;};
        std::string GetFileName() {return fFileName;};

        void SetDeviceType(Global::DeviceType aType) {fDeviceType = aType;};

        std::shared_ptr<Parser::IParser> GetParser() {return fParser;};
        void Prepare();
        virtual void PrepareDevice() {};
        virtual void Start() {};

        void SetDigitizerTypes(std::vector<std::string> aDigitizerTypes) {fDigitizerTypes = aDigitizerTypes;};
        std::vector<std::string> GetDigitizerTypes() {return fDigitizerTypes;};

        TFile* fRootFile = nullptr;
    private:
        void ConfigureRoot();
        void ConfigureTxt();
        std::filesystem::path fConfigPath;
        std::vector<std::filesystem::path> fBinaryPathVector{};
        std::shared_ptr<Parser::IParser> fParser = nullptr;
        std::string fFileName;
        Global::DeviceType fDeviceType;
        std::vector<std::string> fDigitizerTypes = {};
    };
}