#pragma once

#include "IDevice.h"
#include "TTree.h"

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

        void ConfigureRoot();
        void ConfigureTxt();
        Device::DigitizerEvent fEvent{};

        TTree* fTreePSD = nullptr;
        TTree* fTreeWaveform = nullptr;
    };
}