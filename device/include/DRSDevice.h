#pragma once

#include "IDevice.h"
#include "TH1.h"
#include "TTree.h"
#include "Time.h"
#include <fstream>
#include <mutex>
#include <utility>

namespace Device {
struct DRSEvent {
    int32_t time;
    double baseline;
    double charge;
    double amplitude;
    int16_t range;
    int16_t trigger;
    uint32_t scaler;
    std::vector<double> waveform;
};

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

    bool fChannelMap[4] = {0, 0, 0, 0};
    TTree *fChannelEventsTreeMap[4] = {};
    TTree *fChannelTimeTreeMap[4] = {};
    std::vector<TH1 *> fChannelHist[4] = {};
    TDirectory *fDirectoryMap[4] = {};

    void ConfigureRoot();
    void ConfigureTxt();
    void WriteTxtEvent();
    void DefineChannels();
    void ReadChannels(std::ifstream *file, std::filesystem::path *path);
    void ReadFileHeader(std::ifstream *file, std::filesystem::path *path, bool save);
    void ReadTimeHeader(std::ifstream *file, std::filesystem::path *path, bool save);
    void ReadEventHeader(std::ifstream *file, std::filesystem::path *path, bool save);
    void ReadDate(std::ifstream *file, std::filesystem::path *path);

    void ReadPreAverageWaveform();

    std::vector<Device::DRSEvent> fEvent{};
    std::ofstream fTxtFile;

    bool is_first_date_ = false;
    Global::Time first_date_event_;
    Global::Time last_date_event_;
    uint64_t time_event_diff_ = 0;
    TTree *fDateTimeTree = nullptr;

    uint64_t EstimateTimeDifference(Global::Time t_base, Global::Time t_in);
};
}  // namespace Device