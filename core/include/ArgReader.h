#pragma once

#include <string>
#include <memory>
#include <mutex>
#include <filesystem>
#include <vector>

#include "Types.h"

namespace Core {
    class ArgReader {
    public:
        ArgReader(int argc, char *argv[]);
        ~ArgReader();

        std::filesystem::path GetConfigPath() {return fConfigPath;};
        std::vector<std::filesystem::path> GetBinaryPathVector() {return fBinaryPathVector;};
        std::string GetFileName() {return fFileName;};

        Global::DeviceType GetDeviceType() {return fDeviceType;};
        std::vector<std::string> GetDigitizerTypes() {return fDigitizerTypes;};

        bool IsCut() {return isCut;};
        bool IsDebug() {return isDebug;};
    private:
        void ParseThreads(int argc, char *argv[]);
        bool ParseDRSBinaryFile(int argc, char *argv[]);
        bool ParseDigitizerBinaryFile(int argc, char *argv[]);
        void ParseConfigFile(int argc, char *argv[]);

        void SetDRSBinaryFileName(std::string name);
        void SetDigitizerBinaryFileName(std::string name);
        std::string GetBinaryFileName(std::string name);

        void ParseCut(int argc, char *argv[]);
        void ParseDebug(int argc, char *argv[]);

        std::string threadKey = "-j";
        std::string cutKey = "-cut";
        std::string debugKey = "-debug";
        std::string binaryDRSExtension = "dat";
        std::string binaryDigitizerExtension = "bin";
        std::string configExtension = "cfg";

        std::filesystem::path fConfigPath = "";
        std::vector<std::filesystem::path> fBinaryPathVector{};
        std::string fFileName = "";
        mutable std::once_flag initDigitizerFlag;
        Global::DeviceType fDeviceType;
        std::vector<std::string> fDigitizerTypes = {};

        bool isCut = false;
        bool isDebug = false;
    };
}