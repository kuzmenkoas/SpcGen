#pragma once

#include <fstream>
#include "TTree.h"
#include "IDevice.h"

namespace Device {
    class DRSDevice : public IDevice {
    public:
        DRSDevice();
        ~DRSDevice();

        void PrepareDevice() final;
        void Start() final;
    private:
        const char fFileHeader[4] = {'D', 'R', 'S', ' '};
        const char fTimeHeader[4] = {'T', 'I', 'M', 'E'};
        const char fBoardNumberHeader[4] = {'B', '#', ' ', ' '};
        const char fChannelHeader[4] = {'C', '0', '0', ' '};
        const char fEventHeader[4] = {'E', 'H', 'D', 'R'};

        bool fChannelMap[4] ={0, 0, 0, 1};
        TTree* fChannelEventsTreeMap[4] = {};
        TTree* fChannelTimeTreeMap[4] = {};
        TDirectory* fDirectoryMap[4] = {};

        void ConfigureRoot();
        void ReadFileHeader(std::ifstream* file, std::filesystem::path* path);
        void ReadTimeHeader(std::ifstream* file, std::filesystem::path* path);
        void ReadEventHeader(std::ifstream* file, std::filesystem::path* path);
        void ReadDate(std::ifstream* file, std::filesystem::path* path);

        int32_t CalculateCharge(std::vector<int16_t> eventWaveform);

        std::vector<float> fTimeVector[4] = {{},{},{},{}};
        Global::Parameters fEvent{};
    };
}