#pragma once

#include <string>
#include <memory>
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
    private:
        void ParseThreads(int argc, char *argv[]);
        void ParseDRSBinaryFile(int argc, char *argv[]);
        void ParseConfigFile(int argc, char *argv[]);

        void SetDRSBinaryFileName(std::string name);
        // std::string GetDigitizerBinaryFileName(std::string name);
        std::string GetBinaryFileName(std::string name);

        std::string threadKey = "-j";
        std::string binaryDRSExtension = "dat";
        std::string configExtension = "cfg";

        std::filesystem::path fConfigPath = "";
        std::vector<std::filesystem::path> fBinaryPathVector{};
        std::string fFileName = "";
    };
}