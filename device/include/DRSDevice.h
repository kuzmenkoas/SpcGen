#pragma once

#include <mutex>
#include <fstream>
#include "TH1.h"
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
        std::vector<TH1*> fChannelHist[4] = {};
        TDirectory* fDirectoryMap[4] = {};

        TH1* h1 = new TH1D("charge", "charge", 1500, -32, 160);

        void ConfigureRoot();
        void ConfigureTxt();
        void WriteTxtEvent();
        void ReadFileHeader(std::ifstream* file, std::filesystem::path* path);
        void ReadTimeHeader(std::ifstream* file, std::filesystem::path* path);
        void ReadEventHeader(std::ifstream* file, std::filesystem::path* path);
        void ReadDate(std::ifstream* file, std::filesystem::path* path);

        double CalculateCharge(std::vector<double> eventWaveform);
        double CalculateAmplitude(std::vector<double> eventWaveform);
        void CalculateBaseline(std::vector<double> eventWaveform);
        void CalculateWaveform(std::vector<double> eventWaveform);
        void InitializeSumWaveform(std::vector<double> eventWaveform);
        mutable std::once_flag initWaveFlag;

        std::vector<int32_t> fTimeVector[4] = {{},{},{},{}};
        Global::Parameters fEvent{};
        std::ofstream fTxtFile;
    };
}