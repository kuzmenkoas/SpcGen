#include "DRSDevice.h"
#include "TString.h"
#include "TDirectory.h"
#include "TGraph.h"

Device::DRSDevice::DRSDevice() {
}

Device::DRSDevice::~DRSDevice() {
}

void Device::DRSDevice::PrepareDevice() {
    // Make ntuples
    for (std::string writer : GetParser()->GetUsedWriterVector()) {
        if (writer == "Root") ConfigureRoot();
    }

    Global::Parameters usedParameters = GetParser()->GetUsedParameters();
}

void Device::DRSDevice::Start() {
    for (std::filesystem::path path : GetBinaryPathVector()) {
        std::ifstream file(path.string());
        if (file.is_open()) {
            char tmp[4];
            ReadFileHeader(&file, &path);
            // Read Time header
            ReadTimeHeader(&file, &path);
            // Read Events
            ReadEventHeader(&file, &path);
        } else {
            std::cerr << "Error: File not found: " << path.string() << "\n";
            std::cerr << "Stop program." << "\n";
        }
    }

    fRootFile->Write();
}

void Device::DRSDevice::ConfigureRoot() {
    Global::Parameters usedParameters = GetParser()->GetUsedParameters();

    int ch = 1;
    for (bool channel : fChannelMap) {
        if (channel) {
            std::string id = "Channel " + std::to_string(ch);
            TString idString = TString(id);

            TDirectory* dir = fRootFile->mkdir(idString);
            dir->cd();

            if (usedParameters.time.has_value()) {
                TTree* fTreeTime = new TTree("Time", "Time");
                fTreeTime->Branch("time", &fEvent.time, "time/F");
                fChannelTimeTreeMap[ch-1] = fTreeTime;
            }

            if (usedParameters.baseline.has_value() || usedParameters.charge.has_value()) {
                TTree* fTree = new TTree("Events", "Events");
                if (usedParameters.baseline.has_value()) fTree->Branch("baseline", &fEvent.baseline, "baseline/D");
                if (usedParameters.charge.has_value()) fTree->Branch("charge", &fEvent.charge, "charge/D");
                if (usedParameters.scaler.has_value()) fTree->Branch("scaler", &fEvent.scaler, "scaler/I");
                fChannelEventsTreeMap[ch-1] = fTree;
            }

            if (usedParameters.hist.has_value()) {
                TDirectory* dirHist = dir->mkdir("Histograms");
                dirHist->cd();
            }
        }
        ch++;
    }
}

void Device::DRSDevice::ReadFileHeader(std::ifstream* file, std::filesystem::path* path) {
    // Read File header
    char tmp[4];
    file->read((char*) &tmp, sizeof(tmp));
    if (tmp[0] == fFileHeader[0] && tmp[1] == fFileHeader[1] && tmp[2] == fFileHeader[2]) {
        std::cout << "Reading file: " << path->string() << "\n";
        int16_t version;
        std::memcpy(&version, &tmp[3], sizeof(version));
        std::cout << "DRS version: " << version << "\n";
    } else {
        std::cerr << "Warning: File header mismatch." << "\n";
        std::cerr << "Read without File header." << "\n";
        file->seekg(0);
    }
}

void Device::DRSDevice::ReadTimeHeader(std::ifstream* file, std::filesystem::path* path) {
    Global::Parameters usedParameters = GetParser()->GetUsedParameters();
    // Read Time header
    char tmp[4];
    file->read((char*) &tmp, sizeof(tmp));
    if (tmp[0] == fTimeHeader[0] && tmp[1] == fTimeHeader[1] && tmp[2] == fTimeHeader[2] && tmp[3] == fTimeHeader[3]) {
        // Read board id
        file->read((char*) &tmp, sizeof(tmp));
        if (tmp[0] == fBoardNumberHeader[0] && tmp[1] == fBoardNumberHeader[1]) {
            char idConv[2];
            idConv[0] = tmp[2];
            idConv[1] = tmp[3];

            int16_t id = static_cast<uint8_t>(tmp[2]) | (static_cast<uint8_t>(tmp[3]) << 8);

            std::cout << "Board serial number: " << id << "\n";
            
            // Read Channel header
            file->read((char*) &tmp, sizeof(tmp));
            if (tmp[0] == fChannelHeader[0] && tmp[1] == fChannelHeader[1] && tmp[2] == fChannelHeader[2]) {
                char conv[3] = {tmp[1], tmp[2], tmp[3]};
                int16_t channel = std::atoi(conv);

                while (true) {
                    if (file->read((char*) &tmp, sizeof(tmp))) {
                        if (tmp[0] == fChannelHeader[0] && tmp[1] == fChannelHeader[1] && tmp[2] == fChannelHeader[2]) {
                            char conv2[3] = {tmp[1], tmp[2], tmp[3]};
                            int16_t channel = std::atoi(conv2);
                        }

                        if (tmp[0] == fEventHeader[0] && tmp[1] == fEventHeader[1] && tmp[2] == fEventHeader[2] && tmp[3] == fEventHeader[3]) {
                            file->seekg(-4, std::ios_base::cur);
                            break;
                        }
                        fEvent.time = DEFAULT_VALUE;
                        std::memcpy(&(*fEvent.time), &tmp, sizeof(tmp));
                        fTimeVector[channel-1].push_back(fEvent.time.value());
                        
                        if (usedParameters.time.has_value()) fChannelTimeTreeMap[channel-1]->Fill();
                    } else {
                        // Error
                        break;
                        // exit
                    }
                }
            } else {
                std::cerr << "Error: Channel header mismatch." << "\n";
            }
        } else {
            std::cerr << "Error: Board number header mismatch." << "\n";
            std::cerr << "Stop program." << "\n";
        }
    } else {
        std::cerr << "Error: Time header mismatch." << "\n";
        std::cerr << "Stop program." << "\n";
    }
}

void Device::DRSDevice::ReadEventHeader(std::ifstream* file, std::filesystem::path* path) {
    Global::Parameters usedParameters = GetParser()->GetUsedParameters();
    int eventCounter = 0;
    while (true) {
        char tmp[4];
        if (file->read((char*) &tmp, sizeof(tmp))) {
            if (tmp[0] == fEventHeader[0] && tmp[1] == fEventHeader[1] && tmp[2] == fEventHeader[2] && tmp[3] == fEventHeader[3]) {
                int32_t eventNumber;
                file->read((char*) &tmp, sizeof(tmp));
                std::memcpy(&eventNumber, &tmp, sizeof(eventNumber));
                ReadDate(file, path);

                char tmp2[2];
                int16_t rangeCenter;
                file->read((char*) &tmp2, sizeof(tmp2));
                std::memcpy(&rangeCenter, &tmp2, sizeof(rangeCenter));

                char tmp[4];
                int16_t version;
                file->read((char*) &tmp, sizeof(tmp));
                std::memcpy(&version, &tmp, sizeof(version));
                file->read((char*) &tmp, sizeof(tmp));
                std::memcpy(&version, &tmp, sizeof(version));

                std::vector<double> waveform;
                int16_t channel;
                while (file->read((char*) &tmp, sizeof(tmp))) {
                    // file->read((char*) &tmp, sizeof(tmp));
                    if (tmp[0] == fChannelHeader[0] && tmp[1] == fChannelHeader[1] && tmp[2] == fChannelHeader[2]) {
                        // char conv2[3] = {tmp[1], tmp[2], tmp[3]};
                        // std::cout << tmp << std::endl;
                        char* conv3 = &tmp[3];
                        channel = std::atoi(conv3);
                        int32_t scaler;
                        file->read((char*) &tmp, sizeof(tmp));
                        std::memcpy(&scaler, &tmp, sizeof(scaler));
                        fEvent.scaler = scaler;
                    }
                    if (tmp[0] == fEventHeader[0] && tmp[1] == fEventHeader[1] && tmp[2] == fEventHeader[2] && tmp[3] == fEventHeader[3]) {
                        file->seekg(-4, std::ios_base::cur);
                        break;
                    }
                    int16_t voltage1;
                    int16_t voltage2;
                    
                    // TODO: change seekg to tmp decode
                    file->seekg(-4, std::ios_base::cur);
                    file->read((char*) &tmp2, sizeof(tmp2));
                    std::memcpy(&voltage1, &tmp2, sizeof(voltage1));

                    file->read((char*) &tmp2, sizeof(tmp2));
                    std::memcpy(&voltage2, &tmp2, sizeof(voltage2));

                    double wave1 = voltage1/65536. + rangeCenter/1000. - 0.5;
                    double wave2 = voltage2/65536. + rangeCenter/1000. - 0.5;

                    waveform.push_back(wave1);
                    waveform.push_back(wave2);
                }
                CalculateWaveform(waveform);
                CalculateBaseline(waveform);

                if (usedParameters.charge.has_value()) fEvent.charge = CalculateCharge(waveform);
                if (usedParameters.baseline.has_value() || usedParameters.charge.has_value()) fChannelEventsTreeMap[channel-1]->Fill();
                // Process event
                eventCounter++;
            } else {
                // Error
            }
        } else {
            auto& v = *fEvent.waveform;
            TGraph* gr = new TGraph();
            int counter = 0;
            for (auto event : v) {
                gr->AddPoint(counter++, event/eventCounter);
            }
            gr->Write("waveform");
            std::cout << "Reading success!" << "\n";
            break;
        }
    }
}

void Device::DRSDevice::ReadDate(std::ifstream* file, std::filesystem::path* path) {
    char tmp2[2];
    int16_t tt;
    file->read((char*) &tmp2, sizeof(tmp2));
    std::memcpy(&tt, &tmp2, sizeof(tt));

    file->read((char*) &tmp2, sizeof(tmp2));
    std::memcpy(&tt, &tmp2, sizeof(tt));

    file->read((char*) &tmp2, sizeof(tmp2));
    std::memcpy(&tt, &tmp2, sizeof(tt));

    file->read((char*) &tmp2, sizeof(tmp2));
    std::memcpy(&tt, &tmp2, sizeof(tt));

    file->read((char*) &tmp2, sizeof(tmp2));
    std::memcpy(&tt, &tmp2, sizeof(tt));

    file->read((char*) &tmp2, sizeof(tmp2));
    std::memcpy(&tt, &tmp2, sizeof(tt));

    file->read((char*) &tmp2, sizeof(tmp2));
    std::memcpy(&tt, &tmp2, sizeof(tt));
}

double Device::DRSDevice::CalculateCharge(std::vector<double> eventWaveform) {
    double charge = 0;
    int min = 500;
    int max = 600;
    int counter = 0;
    for (double waveform : eventWaveform) {
        if ((counter >= min) && (counter <= max)) charge += waveform - fEvent.baseline.value();
        counter++;
    }
    return charge;
}

void Device::DRSDevice::CalculateWaveform(std::vector<double> eventWaveform) {
    std::call_once(initWaveFlag, [this, eventWaveform](){InitializeSumWaveform(eventWaveform);});
    auto& v = *fEvent.waveform;
    for (size_t i = 1; i < size(eventWaveform); i++) v[i] += eventWaveform[i];
}

void Device::DRSDevice::InitializeSumWaveform(std::vector<double> eventWaveform) {
    auto& v = *fEvent.waveform;
    for (double waveform : eventWaveform) v.push_back(waveform);
}

void Device::DRSDevice::CalculateBaseline(std::vector<double> eventWaveform) {
    int min = 16;
    int max = 400;

    fEvent.baseline = 0;
    double ss = 0;
    for (size_t i = 0; i < size(eventWaveform); i++) {
        if ((i >= min) && (i <= max)) {
            ss += eventWaveform[i];
        }
    }
    fEvent.baseline = ss/(max-min+1.);
}