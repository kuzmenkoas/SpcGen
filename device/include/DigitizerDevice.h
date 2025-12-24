#pragma once

#include <mutex>
#include <fstream>
#include "IDevice.h"
#include "TTree.h"
#include "TH1.h"

namespace Device {
    struct DigitizerEvent {
        int32_t qShort;
        int32_t qLong;
        int16_t cfd_y1;
        int16_t cfd_y2;
        int16_t baselinePSD;
        int16_t height;
        uint32_t eventCounter;
        uint32_t eventCounterPSD;
        int16_t psdValue;
        double baseline;
        double charge;
        double amplitude;
        std::vector<double> waveform;
    };
    class DigitizerDevice : public IDevice {
    public:
        DigitizerDevice();
        ~DigitizerDevice();

        void PrepareDevice() final;
        void Start() final;
    private:
        void ProcessPSD(std::filesystem::path path);
        void ProcessWaveform(std::filesystem::path path);
        void PreProcessWaveform(std::vector<std::filesystem::path> pathVector);
        int CountUsedParametersBytes();
        int CountUsedParameters();
        void ConfigureRoot();
        void ConfigureTxt();
        void WriteTxtEventPSD();
        void WriteTxtEventWaveform();
        void CalculateBaseline(std::vector<int16_t> eventWaveform);
        void CalculateCharge(std::vector<int16_t> eventWaveform);
        void CalculateAmplitude(std::vector<int16_t> eventWaveform);
        void CalculateWaveform(std::vector<int16_t> eventWaveform);
        void InitializeSumWaveform(std::vector<int16_t> eventWaveform);
        mutable std::once_flag initWaveFlag;
        Device::DigitizerEvent fEvent{};

        TTree* fTreePSD = nullptr;
        TTree* fTreeWaveform = nullptr;
        std::vector<TH1*> fHist = {};
        Global::Parameters usedParameters;
        std::ofstream fTxtFilePSD;
        std::ofstream fTxtFileWaveform;
    };
}