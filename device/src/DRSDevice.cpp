#include "DRSDevice.h"
#include "TDirectory.h"
#include "TGraph.h"
#include "TString.h"
#include <TF1.h>
#include <algorithm>
// #include <cassert>

Device::DRSDevice::DRSDevice() { fEvent.resize(4); }

Device::DRSDevice::~DRSDevice() {}

void Device::DRSDevice::PrepareDevice() {
    DefineChannels();
    if (this->GetIsCut())
        ReadPreAverageWaveform();

    // Make ntuples
    for (std::string writer : GetParser()->GetUsedWriterVector()) {
        if (writer == "Root")
            ConfigureRoot();
        if (writer == "Txt")
            ConfigureTxt();
    }
}

void Device::DRSDevice::Start() {
    Global::Parameters usedParameters = GetParser()->GetUsedParameters();

    for (std::filesystem::path path : GetBinaryPathVector()) {
        std::ifstream file(path.string(), std::ios::binary);
        if (file.is_open()) {
            // char tmp[4];
            ReadFileHeader(&file, &path, true);
            // Read Time header
            ReadTimeHeader(&file, &path, true);
            // Read Events
            ReadEventHeader(&file, &path, true);
        } else {
            std::cerr << "Error: File not found: " << path.string() << "\n";
            std::cerr << "Stop program." << "\n";
        }
    }
    if (usedParameters.hist.has_value()) {
        for (int i = 0; i < 4; i++) {
            if (fDirectoryMap[i])
                fDirectoryMap[i]->cd();
            for (size_t k = 0; k < size(fChannelHist[i]); k++) {
                TString name(usedParameters.hist.value()[k].parameter.c_str(),
                             usedParameters.hist.value()[k].parameter.length());
                fChannelHist[i][k]->Write(name);
            }
        }
    }

    for (std::string writer : GetParser()->GetUsedWriterVector()) {
        if (writer == "Root")
            fRootFile->Write();
        if (writer == "Txt")
            fTxtFile.close();
    }
}

void Device::DRSDevice::ConfigureRoot() {
    Global::Parameters usedParameters = GetParser()->GetUsedParameters();

    int ch = 1;
    if (usedParameters.date_time.has_value()) {
        TTree *fDateTime = new TTree("Date_time", "Date_time");
        fDateTime->Branch("date_time", &time_event_diff_, "date_time/L");
        fDateTimeTree = fDateTime;
    }
    for (bool channel : fChannelMap) {
        if (channel) {
            std::string id = "Channel " + std::to_string(ch);
            TString idString = TString(id.c_str(), id.length());

            TDirectory *dir = fRootFile->mkdir(idString);
            dir->cd();

            if (usedParameters.time.has_value()) {
                TTree *fTreeTime = new TTree("Time", "Time");
                fTreeTime->Branch("time", &fEvent[ch - 1].time, "time/F");
                fChannelTimeTreeMap[ch - 1] = fTreeTime;
            }

            if (usedParameters.baseline.has_value() || usedParameters.charge.has_value()) {
                TTree *fTree = new TTree("Events", "Events");
                if (usedParameters.baseline.has_value())
                    fTree->Branch("baseline", &fEvent[ch - 1].baseline, "baseline/D");
                if (usedParameters.charge.has_value())
                    fTree->Branch("charge", &fEvent[ch - 1].charge, "charge/D");
                if (usedParameters.amplitude.has_value())
                    fTree->Branch("amplitude", &fEvent[ch - 1].amplitude, "amplitude/D");
                if (usedParameters.scaler.has_value())
                    fTree->Branch("scaler", &fEvent[ch - 1].scaler, "scaler/I");
                fChannelEventsTreeMap[ch - 1] = fTree;
            }

            if (usedParameters.hist.has_value() || usedParameters.waveform.has_value()) {
                TDirectory *dirHist = dir->mkdir("Histograms");
                dirHist->cd();
                fDirectoryMap[ch - 1] = dirHist;
                if (usedParameters.hist.has_value()) {
                    for (auto &hist : *usedParameters.hist) {
                        TString name = TString(hist.parameter.c_str(), hist.parameter.length());
                        TH1 *h1 = new TH1D(name, name, hist.Nbins, hist.min, hist.max);
                        fChannelHist[ch - 1].push_back(h1);
                    }
                }
            }
        }
        ch++;
    }
}

void Device::DRSDevice::ConfigureTxt() {
    Global::Parameters usedParameters = GetParser()->GetUsedParameters();
    fTxtFile = std::ofstream(GetFileName() + ".txt");
    if (usedParameters.baseline.has_value() || usedParameters.charge.has_value() ||
        usedParameters.amplitude.has_value() || usedParameters.scaler.has_value())
        fTxtFile << "channel ";
    if (usedParameters.baseline.has_value())
        fTxtFile << "baseline ";
    if (usedParameters.charge.has_value())
        fTxtFile << "charge ";
    if (usedParameters.amplitude.has_value())
        fTxtFile << "amplitude ";
    if (usedParameters.scaler.has_value())
        fTxtFile << "scaler ";
    fTxtFile << "\n";
}

void Device::DRSDevice::WriteTxtEvent() {
    Global::Parameters usedParameters = GetParser()->GetUsedParameters();
    size_t ch = 0;
    for (auto is_channel : fChannelMap) {
        if (is_channel) {
            if (usedParameters.baseline.has_value() || usedParameters.charge.has_value() ||
                usedParameters.amplitude.has_value() || usedParameters.scaler.has_value())
                fTxtFile << ch << " ";
            if (usedParameters.baseline.has_value())
                fTxtFile << fEvent[ch].baseline << " ";
            if (usedParameters.charge.has_value())
                fTxtFile << fEvent[ch].charge << " ";
            if (usedParameters.amplitude.has_value())
                fTxtFile << fEvent[ch].amplitude << " ";
            if (usedParameters.scaler.has_value())
                fTxtFile << fEvent[ch].scaler << " ";
            ++ch;
        }
        fTxtFile << "\n";
    }
}

void Device::DRSDevice::DefineChannels() {
    for (std::filesystem::path path : GetBinaryPathVector()) {
        std::ifstream file(path.string(), std::ios::binary);
        ReadFileHeader(&file, &path, false);
        ReadChannels(&file, &path);
        file.close();
    }
}

void Device::DRSDevice::ReadChannels(std::ifstream *file, std::filesystem::path *path) {
    Global::Parameters usedParameters = GetParser()->GetUsedParameters();
    // Read Time header
    char tmp[4];
    file->read((char *)&tmp, sizeof(tmp));
    if (tmp[0] == fTimeHeader[0] && tmp[1] == fTimeHeader[1] && tmp[2] == fTimeHeader[2] && tmp[3] == fTimeHeader[3]) {
        // Read board id
        file->read((char *)&tmp, sizeof(tmp));

        if (tmp[0] == fBoardNumberHeader[0] && tmp[1] == fBoardNumberHeader[1]) {
            char idConv[2];
            idConv[0] = tmp[2];
            idConv[1] = tmp[3];

            int16_t id = static_cast<uint8_t>(tmp[2]) | (static_cast<uint8_t>(tmp[3]) << 8);

            std::cout << "Board serial number: " << id << "\n";

            // Read Channel header
            file->read((char *)&tmp, sizeof(tmp));
            if ((tmp[0] == fChannelHeader[0]) && (tmp[1] == fChannelHeader[1]) && (tmp[2] == fChannelHeader[2])) {
                char conv[3] = {tmp[1], tmp[2], tmp[3]};
                int16_t channel = std::atoi(conv);

                if (channel == 1)
                    fChannelMap[0] = true;
                if (channel == 2)
                    fChannelMap[1] = true;
                if (channel == 3)
                    fChannelMap[2] = true;
                if (channel == 4)
                    fChannelMap[3] = true;

                while (true) {
                    if (file->read(reinterpret_cast<char *>(&tmp), sizeof(tmp))) {
                        if (tmp[0] == fChannelHeader[0] && tmp[1] == fChannelHeader[1] && tmp[2] == fChannelHeader[2]) {
                            char conv2[3] = {tmp[1], tmp[2], tmp[3]};
                            int16_t channel = std::atoi(conv2);
                            if (channel == 1)
                                fChannelMap[0] = true;
                            if (channel == 2)
                                fChannelMap[1] = true;
                            if (channel == 3)
                                fChannelMap[2] = true;
                            if (channel == 4)
                                fChannelMap[3] = true;
                        }

                        if (tmp[0] == fEventHeader[0] && tmp[1] == fEventHeader[1] && tmp[2] == fEventHeader[2] &&
                            tmp[3] == fEventHeader[3]) {
                            file->seekg(-4, std::ios_base::cur);
                            break;
                        }
                    } else {
                        std::cerr << "Something went wrong" << std::endl;
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

void Device::DRSDevice::ReadFileHeader(std::ifstream *file, std::filesystem::path *path, bool save) {
    // Read File header
    char tmp[4];
    file->read((char *)&tmp, sizeof(tmp));
    if (tmp[0] == fFileHeader[0] && tmp[1] == fFileHeader[1] && tmp[2] == fFileHeader[2]) {
        if (save) {
            std::cout << "Reading file: " << path->string() << "\n";
            std::cout << "DRS version: " << tmp[3] << "\n";
        }
    } else {
        if (save) {
            std::cerr << "Warning: File header mismatch." << "\n";
            std::cerr << "Read without File header." << "\n";
        }
        file->seekg(0);
    }
}

void Device::DRSDevice::ReadTimeHeader(std::ifstream *file, std::filesystem::path *path, bool save) {
    Global::Parameters usedParameters = GetParser()->GetUsedParameters();
    // Read Time header
    char tmp[4];
    file->read((char *)&tmp, sizeof(tmp));
    if (tmp[0] == fTimeHeader[0] && tmp[1] == fTimeHeader[1] && tmp[2] == fTimeHeader[2] && tmp[3] == fTimeHeader[3]) {
        // Read board id
        file->read((char *)&tmp, sizeof(tmp));
        if (tmp[0] == fBoardNumberHeader[0] && tmp[1] == fBoardNumberHeader[1]) {
            char idConv[2];
            idConv[0] = tmp[2];
            idConv[1] = tmp[3];

            int16_t id = static_cast<uint8_t>(tmp[2]) | (static_cast<uint8_t>(tmp[3]) << 8);

            std::cout << "Board serial number: " << id << "\n";

            // Read Channel header
            file->read((char *)&tmp, sizeof(tmp));
            if (tmp[0] == fChannelHeader[0] && tmp[1] == fChannelHeader[1] && tmp[2] == fChannelHeader[2]) {
                char conv[3] = {tmp[1], tmp[2], tmp[3]};
                int16_t channel = std::atoi(conv);

                while (true) {
                    if (file->read((char *)&tmp, sizeof(tmp))) {
                        if (tmp[0] == fChannelHeader[0] && tmp[1] == fChannelHeader[1] && tmp[2] == fChannelHeader[2]) {
                            char conv2[3] = {tmp[1], tmp[2], tmp[3]};
                            channel = std::atoi(conv2);
                            file->read((char *)&tmp, sizeof(tmp));
                        }

                        if (tmp[0] == fEventHeader[0] && tmp[1] == fEventHeader[1] && tmp[2] == fEventHeader[2] &&
                            tmp[3] == fEventHeader[3]) {
                            file->seekg(-4, std::ios_base::cur);
                            break;
                        }
                        if (save) {
                            fEvent[channel - 1].time = DEFAULT_VALUE;
                            std::memcpy(&fEvent[channel - 1].time, &tmp, sizeof(tmp));

                            for (std::string writer : GetParser()->GetUsedWriterVector()) {
                                if (writer == "Root") {
                                    if (usedParameters.time.has_value()) {
                                        fChannelTimeTreeMap[channel - 1]->Fill();
                                    }
                                }
                            }
                        }
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

void Device::DRSDevice::ReadEventHeader(std::ifstream *file, std::filesystem::path *path, bool save) {
    Global::Parameters usedParameters = GetParser()->GetUsedParameters();
    int eventCounter = 0;
    while (true) {
        char tmp[4];
        if (file->read((char *)&tmp, sizeof(tmp))) {
            if (tmp[0] == fEventHeader[0] && tmp[1] == fEventHeader[1] && tmp[2] == fEventHeader[2] &&
                tmp[3] == fEventHeader[3]) {
                int32_t eventNumber;
                file->read((char *)&tmp, sizeof(tmp));
                std::memcpy(&eventNumber, &tmp, sizeof(eventNumber));
                ReadDate(file, path);

                char tmp2[2];
                int16_t rangeCenter;
                file->read((char *)&tmp2, sizeof(tmp2));
                std::memcpy(&rangeCenter, &tmp2, sizeof(rangeCenter));

                char tmp[4];
                int16_t version;
                file->read((char *)&tmp, sizeof(tmp));
                std::memcpy(&version, &tmp, sizeof(version));
                file->read((char *)&tmp, sizeof(tmp));
                std::memcpy(&version, &tmp, sizeof(version));

                std::vector<std::vector<double>> waveform{4};
                int16_t channel;
                while (file->read((char *)&tmp, sizeof(tmp))) {
                    if (tmp[0] == fChannelHeader[0] && tmp[1] == fChannelHeader[1] && tmp[2] == fChannelHeader[2]) {
                        char *conv3 = &tmp[3];
                        channel = std::atoi(conv3);
                        uint32_t scaler;
                        file->read((char *)&tmp, sizeof(tmp));
                        std::memcpy(&scaler, &tmp, sizeof(scaler));
                        if (save)
                            fEvent[channel - 1].scaler = scaler;
                        file->read((char *)&tmp, sizeof(tmp));
                    }
                    if (tmp[0] == fEventHeader[0] && tmp[1] == fEventHeader[1] && tmp[2] == fEventHeader[2] &&
                        tmp[3] == fEventHeader[3]) {
                        file->seekg(-4, std::ios_base::cur);
                        break;
                    }
                    uint16_t voltage1;
                    uint16_t voltage2;

                    char mtmp1[2] = {tmp[0], tmp[1]};
                    char mtmp2[2] = {tmp[2], tmp[3]};

                    std::memcpy(&voltage1, mtmp1, sizeof(voltage1));
                    std::memcpy(&voltage2, mtmp2, sizeof(voltage2));

                    double wave1 = voltage1 / 65536. + rangeCenter / 1000. - 0.5;
                    double wave2 = voltage2 / 65536. + rangeCenter / 1000. - 0.5;

                    (waveform[channel - 1]).push_back(wave1);
                    (waveform[channel - 1]).push_back(wave2);
                }

                eventCounter++;
                if (usedParameters.waveform.has_value() && ((!this->GetIsCut()) || ((this->GetIsCut()) && (!save)))) {
                    size_t ch = 0;
                    for (std::vector<double> wave : waveform) {
                        if (!wave.empty())
                            TemplateCalculateWaveform(wave, &(fEvent[ch].waveform), ch);
                        ++ch;
                    }
                }

                if (save) {
                    for (std::string writer : GetParser()->GetUsedWriterVector()) {
                        if (usedParameters.date_time.has_value()) {
                            fDateTimeTree->Fill();
                        }
                    }
                    channel = 1;
                    for (auto wave : waveform) {
                        if (wave.empty())
                            continue;

                        if (SignalFilter(wave, fEvent[channel - 1].waveform, fEvent[channel - 1].baseline)) {
                            if (usedParameters.charge.has_value() || usedParameters.baseline.has_value())
                                fEvent[channel - 1].baseline = TemplateCalculateBaseline(wave);
                            if (usedParameters.charge.has_value())
                                fEvent[channel - 1].charge =
                                    TemplateCalculateCharge(wave, fEvent[channel - 1].baseline);
                            if (usedParameters.amplitude.has_value())
                                fEvent[channel - 1].amplitude =
                                    TemplateCalculateAmplitude(wave, fEvent[channel - 1].baseline);
                            // Process event
                            for (std::string writer : GetParser()->GetUsedWriterVector()) {
                                if (writer == "Root") {
                                    if (usedParameters.baseline.has_value() || usedParameters.charge.has_value()) {
                                        fChannelEventsTreeMap[channel - 1]->Fill();
                                        int iHist = 0;
                                        if (usedParameters.hist.has_value()) {
                                            auto &hists = *usedParameters.hist;
                                            for (size_t i = 0; i < size(hists); i++) {
                                                if (hists[i].parameter == "baseline")
                                                    fChannelHist[channel - 1][iHist++]->Fill(
                                                        fEvent[channel - 1].baseline);
                                                if (hists[i].parameter == "charge")
                                                    fChannelHist[channel - 1][iHist++]->Fill(
                                                        fEvent[channel - 1].charge);
                                                if (hists[i].parameter == "amplitude")
                                                    fChannelHist[channel - 1][iHist++]->Fill(
                                                        fEvent[channel - 1].amplitude);
                                                if (hists[i].parameter == "scaler")
                                                    fChannelHist[channel - 1][iHist++]->Fill(
                                                        fEvent[channel - 1].scaler);
                                            }
                                        }
                                    }
                                }
                                if (writer == "Txt")
                                    WriteTxtEvent();
                            }
                        } else {
                            if (this->GetIsDebug()) {
                                // add plotting waveform
                                TGraph *gr = new TGraph();
                                int counter = 0;
                                for (double event : wave) {
                                    gr->AddPoint(counter++, event);
                                }
                                std::string nametmp = "waveform" + std::to_string(eventCounter);
                                TString name = TString(nametmp.c_str(), nametmp.length());
                                gr->Write(name);
                            }
                        }
                        ++channel;
                    }
                }
            } else {
                // Error
            }
        } else {
            for (size_t channel = 0; channel < sizeof(fChannelMap); ++channel) {
                if (!fChannelMap[channel])
                    continue;
                if (!usedParameters.signal.has_value())
                    DefineSignalDirection(fEvent[channel].waveform);
                if (!save && (this->GetIsCut())) {
                    for (int i = 0; i < fEvent[channel].waveform.size(); i++)
                        fEvent[channel].waveform[i] /= eventCounter;
                }
                if (save) {
                    // std::cout << channel << " " << fEvent[channel].waveform.size() << std::endl;
                    for (std::string writer : GetParser()->GetUsedWriterVector()) {
                        if (writer == "Root") {
                            // Plot mean waveform
                            if (usedParameters.waveform.has_value()) {
                                fDirectoryMap[channel]->cd();
                                TGraph *gr = new TGraph();
                                int counter = 0;
                                if (this->GetIsCut())
                                    eventCounter = 1;
                                for (double event : fEvent[channel].waveform) {
                                    gr->AddPoint(counter++, event / eventCounter);
                                }
                                gr->Write("waveform");
                            }
                        }
                    }

                    std::cout << "Reading channel " << channel + 1 << " success!" << "\n";
                }
            }
            break;
        }
    }

    // Output about date first and last event
    std::cout << "Event amount: " << eventCounter << std::endl;
    std::cout << "First event date is: " << first_date_event_.hour << ":" << first_date_event_.min << ":"
              << first_date_event_.sec << ":" << first_date_event_.msec << " " << first_date_event_.mday << "."
              << first_date_event_.mon << "." << first_date_event_.year << std::endl;

    std::cout << "Last event date is: " << last_date_event_.hour << ":" << last_date_event_.min << ":"
              << last_date_event_.sec << ":" << last_date_event_.msec << " " << last_date_event_.mday << "."
              << last_date_event_.mon << "." << last_date_event_.year << std::endl;

    std::cout << "Average frequency: " << eventCounter / (static_cast<double>(time_event_diff_) / 1000) << " Hz"
              << std::endl;
}

// returns in msec
uint64_t Device::DRSDevice::EstimateTimeDifference(Global::Time t_base, Global::Time t_in) {
    uint64_t time_diff = 0;
    const int MSEC_PER_SEC = 1000;
    const int SEC_PER_MIN = 60;
    const int MIN_PER_HOUR = 60;
    const int HOUR_PER_DAY = 24;

    const int DAY_PER_YEAR = 365;
    const int DAY_PER_MONTH = 30;

    uint64_t days_diff =
        (t_in.year - t_base.year) * DAY_PER_YEAR + (t_in.mon - t_base.mon) * DAY_PER_MONTH + (t_in.mday - t_base.mday);

    time_diff += days_diff * HOUR_PER_DAY * MIN_PER_HOUR * SEC_PER_MIN * MSEC_PER_SEC;

    if (t_in.hour >= t_base.hour) {
        time_diff += (t_in.hour - t_base.hour) * MIN_PER_HOUR * SEC_PER_MIN * MSEC_PER_SEC;
    } else {
        time_diff += (HOUR_PER_DAY + t_in.hour - t_base.hour) * MIN_PER_HOUR * SEC_PER_MIN * MSEC_PER_SEC;
        time_diff -= HOUR_PER_DAY * MIN_PER_HOUR * SEC_PER_MIN * MSEC_PER_SEC;
    }

    if (t_in.min >= t_base.min) {
        time_diff += (t_in.min - t_base.min) * SEC_PER_MIN * MSEC_PER_SEC;
    } else {
        time_diff += (MIN_PER_HOUR + t_in.min - t_base.min) * SEC_PER_MIN * MSEC_PER_SEC;
        time_diff -= MIN_PER_HOUR * SEC_PER_MIN * MSEC_PER_SEC;
    }

    if (t_in.sec >= t_base.sec) {
        time_diff += t_in.sec - t_base.sec;
    } else {
        time_diff += (SEC_PER_MIN + t_in.sec - t_base.sec) * MSEC_PER_SEC;
        time_diff -= SEC_PER_MIN * MSEC_PER_SEC;
    }

    if (t_in.msec >= t_base.msec) {
        time_diff += t_in.msec - t_base.msec;
    } else {
        time_diff += (MSEC_PER_SEC + t_in.msec - t_base.msec);
        time_diff -= MSEC_PER_SEC;
    }

    return time_diff;
}

void Device::DRSDevice::ReadDate(std::ifstream *file, std::filesystem::path *path) {
    Global::Parameters usedParameters = GetParser()->GetUsedParameters();
    char tmp2[2];
    int16_t tt;
    file->read((char *)&tmp2, sizeof(tmp2));
    std::memcpy(&tt, &tmp2, sizeof(tt));
    last_date_event_.year = tt;

    file->read((char *)&tmp2, sizeof(tmp2));
    std::memcpy(&tt, &tmp2, sizeof(tt));
    last_date_event_.mon = tt;

    file->read((char *)&tmp2, sizeof(tmp2));
    std::memcpy(&tt, &tmp2, sizeof(tt));
    last_date_event_.mday = tt;

    file->read((char *)&tmp2, sizeof(tmp2));
    std::memcpy(&tt, &tmp2, sizeof(tt));
    last_date_event_.hour = tt;

    file->read((char *)&tmp2, sizeof(tmp2));
    std::memcpy(&tt, &tmp2, sizeof(tt));
    last_date_event_.min = tt;

    file->read((char *)&tmp2, sizeof(tmp2));
    std::memcpy(&tt, &tmp2, sizeof(tt));
    last_date_event_.sec = tt;

    file->read((char *)&tmp2, sizeof(tmp2));
    std::memcpy(&tt, &tmp2, sizeof(tt));
    last_date_event_.msec = tt;

    if (!is_first_date_) {
        first_date_event_ = last_date_event_;
        is_first_date_ = true;
    }

    if (usedParameters.date_time.has_value()) {
        time_event_diff_ = time_event_diff_ + EstimateTimeDifference(first_date_event_, last_date_event_);
    }

    // unit tests
    // {
    //     Global::Time base_time;
    //     base_time.hour = 23;
    //     base_time.min = 50;
    //     base_time.sec = 20;
    //     base_time.msec = 0;
    //     base_time.mday = 25;
    //     base_time.mon = 7;
    //     base_time.year = 2026;

    //     Global::Time test_time;
    //     test_time.hour = 0;
    //     test_time.min = 10;
    //     test_time.sec = 10;
    //     test_time.msec = 0;
    //     test_time.mday = 26;
    //     test_time.mon = 7;
    //     test_time.year = 2026;

    //     assert(EstimateTimeDifference(base_time, test_time) == 1190000);
    // }
}

void Device::DRSDevice::ReadPreAverageWaveform() {
    // read and return counter of file
    // need to fill fEvent.waveform
    for (std::filesystem::path path : GetBinaryPathVector()) {
        std::ifstream file(path.string(), std::ios::binary);
        ReadFileHeader(&file, &path, false);
        ReadTimeHeader(&file, &path, false);
        ReadEventHeader(&file, &path, false);

        file.close();
    }
}