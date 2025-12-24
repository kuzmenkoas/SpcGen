#include "DigitizerDevice.h"
#include "TGraph.h"
#include <TF1.h>
#include <algorithm>

Device::DigitizerDevice::DigitizerDevice() {
}

Device::DigitizerDevice::~DigitizerDevice() {
}

void Device::DigitizerDevice::PrepareDevice() {
    usedParameters = GetParser()->GetUsedParameters();
    for (std::string writer : GetParser()->GetUsedWriterVector()) {
        if (writer == "Root") ConfigureRoot();
        if (writer == "Txt") ConfigureTxt();
    }
}

void Device::DigitizerDevice::Start() {
    int i = 0;
    if (GetDigitizerTypes().size() == 2) PreProcessWaveform(GetBinaryPathVector());
    for (std::string file : GetDigitizerTypes()) {
        if (file == "PSD") ProcessPSD(GetBinaryPathVector()[i++]);
        if (file == "Waveform") ProcessWaveform(GetBinaryPathVector()[i++]);
    }

    for (std::string writer : GetParser()->GetUsedWriterVector()) {
        if (writer == "Root") fRootFile->Write();
        // if (writer == "Txt") fTxtFile.close();
    }
}

void Device::DigitizerDevice::PreProcessWaveform(std::vector<std::filesystem::path> pathVector) {
    // define wavelength by file size
    uint32_t psdFileSize = std::filesystem::file_size(pathVector[0].string());
    uint32_t waveformFileSize = std::filesystem::file_size(pathVector[1].string());
    int rubbishBytes = 2+2*CountUsedParameters();
    int dataBytes = CountUsedParametersBytes();
    usedParameters.wavelength = waveformFileSize/(psdFileSize/(dataBytes+rubbishBytes))/2;
}

int Device::DigitizerDevice::CountUsedParameters() {
    int i = 0;
    if (usedParameters.qShort.has_value()) i++;
    if (usedParameters.qLong.has_value()) i++;
    if (usedParameters.cfd_y1.has_value()) i++;
    if (usedParameters.cfd_y2.has_value()) i++;
    if (usedParameters.baselinePSD.has_value()) i++;
    if (usedParameters.height.has_value()) i++;
    if (usedParameters.eventCounter.has_value()) i++;
    if (usedParameters.eventCounterPSD.has_value()) i++;
    if (usedParameters.psdValue.has_value()) i++;

    return i;
}

int Device::DigitizerDevice::CountUsedParametersBytes() {
    int i = 0;
    if (usedParameters.qShort.has_value()) i += sizeof(fEvent.qShort);
    if (usedParameters.qLong.has_value()) i += sizeof(fEvent.qLong);
    if (usedParameters.cfd_y1.has_value()) i += sizeof(fEvent.cfd_y1);
    if (usedParameters.cfd_y2.has_value()) i += sizeof(fEvent.cfd_y2);
    if (usedParameters.baselinePSD.has_value()) i += sizeof(fEvent.baselinePSD);
    if (usedParameters.height.has_value()) i += sizeof(fEvent.height);
    if (usedParameters.eventCounter.has_value()) i += sizeof(fEvent.eventCounter);
    if (usedParameters.eventCounterPSD.has_value()) i += sizeof(fEvent.eventCounterPSD);
    if (usedParameters.psdValue.has_value()) i += sizeof(fEvent.psdValue);

    return i;
}

void Device::DigitizerDevice::ProcessPSD(std::filesystem::path path) {
    std::ifstream file(path.string(), std::ios::binary | std::ios::ate);
    file.seekg(0, std::ios::beg);

    int parNumber = CountUsedParameters();
    int nEvents = 0;
    while (true) {
        // Unused bytes (read and forget)
        const int id = 2+2*parNumber;
        char* tmp = new char[id];
        file.read(tmp, id);
        // Read a parameter to value (struct)
        // if saved parameter order has changes - change it there also 
        if (usedParameters.qShort.has_value()) file.read(reinterpret_cast<char*>(&fEvent.qShort), sizeof(fEvent.qShort));
        if (usedParameters.qLong.has_value()) file.read(reinterpret_cast<char*>(&fEvent.qLong), sizeof(fEvent.qLong));
        if (usedParameters.cfd_y1.has_value()) file.read(reinterpret_cast<char*>(&fEvent.cfd_y1), sizeof(fEvent.cfd_y1));
        if (usedParameters.cfd_y2.has_value()) file.read(reinterpret_cast<char*>(&fEvent.cfd_y2), sizeof(fEvent.cfd_y2));
        if (usedParameters.baselinePSD.has_value()) file.read(reinterpret_cast<char*>(&fEvent.baselinePSD), sizeof(fEvent.baselinePSD));
        if (usedParameters.height.has_value()) file.read(reinterpret_cast<char*>(&fEvent.height), sizeof(fEvent.height));
        if (usedParameters.eventCounter.has_value()) file.read(reinterpret_cast<char*>(&fEvent.eventCounter), sizeof(fEvent.eventCounter));
        if (usedParameters.eventCounterPSD.has_value()) file.read(reinterpret_cast<char*>(&fEvent.eventCounterPSD), sizeof(fEvent.eventCounterPSD));
        if (usedParameters.psdValue.has_value()) file.read(reinterpret_cast<char*>(&fEvent.psdValue), sizeof(fEvent.psdValue));
        if (file.eof()) break;

        nEvents++;
        fTreePSD->Fill();

        if (usedParameters.hist.has_value()) {
            auto& hists = *usedParameters.hist;
            int iHist = 0;
            for (size_t i = 0; i < size(hists); i++) {
                if (hists[i].parameter == "qShort" && hists[i].file == "PSD") fHist[iHist++]->Fill(fEvent.qShort);
                if (hists[i].parameter == "qLong" && hists[i].file == "PSD") fHist[iHist++]->Fill(fEvent.qLong);
                if (hists[i].parameter == "cfd_y1" && hists[i].file == "PSD") fHist[iHist++]->Fill(fEvent.cfd_y1);
                if (hists[i].parameter == "cfd_y2" && hists[i].file == "PSD") fHist[iHist++]->Fill(fEvent.cfd_y2);
                if (hists[i].parameter == "baseline" && hists[i].file == "PSD") fHist[iHist++]->Fill(fEvent.baselinePSD);
                if (hists[i].parameter == "height" && hists[i].file == "PSD") fHist[iHist++]->Fill(fEvent.height);
                if (hists[i].parameter == "eventCounter" && hists[i].file == "PSD") fHist[iHist++]->Fill(fEvent.eventCounter);
                if (hists[i].parameter == "eventCounterPSD" && hists[i].file == "PSD") fHist[iHist++]->Fill(fEvent.eventCounterPSD);
                if (hists[i].parameter == "psdValue" && hists[i].file == "PSD") fHist[iHist++]->Fill(fEvent.psdValue);
            }
        }
    }
    file.close();
}

void Device::DigitizerDevice::ProcessWaveform(std::filesystem::path path) {
    std::ifstream file(path.string(), std::ios::binary | std::ios::ate);
    file.seekg(0, std::ios::beg);

    // Process waveform
    int eventCounter = 0;
    while (true) {
        std::vector<int16_t> eventWaveform;
        for (int i = 0; i < usedParameters.wavelength; i++) {
            int16_t wave;
            file.read(reinterpret_cast<char*>(&wave), sizeof(wave));
            eventWaveform.push_back(wave);
        }
        if (file.eof()) break;
        if (usedParameters.charge.has_value() || usedParameters.baseline.has_value()) CalculateBaseline(eventWaveform);
        if (usedParameters.charge.has_value()) CalculateCharge(eventWaveform);
        if (usedParameters.amplitude.has_value()) CalculateAmplitude(eventWaveform);
        CalculateWaveform(eventWaveform);
        fTreeWaveform->Fill();

        if (usedParameters.hist.has_value()) {
            auto& hists = *usedParameters.hist;
            int iHist = 0;
            for (size_t i = 0; i < size(hists); i++) {
                if (hists[i].parameter == "baseline" && hists[i].file == "Waveform") fHist[iHist++]->Fill(fEvent.baseline);
                if (hists[i].parameter == "charge" && hists[i].file == "Waveform") fHist[iHist++]->Fill(fEvent.charge);
                if (hists[i].parameter == "amplitude" && hists[i].file == "Waveform") fHist[iHist++]->Fill(fEvent.amplitude);
            }
        }
        eventCounter++;
    }

    TGraph* gr = new TGraph();
    int counter = 0;
    for (double event : fEvent.waveform) {
        gr->AddPoint(counter++, event/eventCounter);
    }
    gr->Write("waveform");

    file.close();
}

void Device::DigitizerDevice::CalculateBaseline(std::vector<int16_t> eventWaveform) {
    int min = usedParameters.baselineLimits.value().first;
    int max = usedParameters.baselineLimits.value().second;

    fEvent.baseline = 0;
    double ss = 0;
    for (size_t i = 0; i < size(eventWaveform); i++) {
        if ((i >= min) && (i <= max)) {
            ss += eventWaveform[i];
        }
    }
    fEvent.baseline = ss/(max-min+1.);
}

void Device::DigitizerDevice::CalculateCharge(std::vector<int16_t> eventWaveform) {
    int min = usedParameters.chargeLimits.value().first;
    int max = usedParameters.chargeLimits.value().second;
    double charge = 0;
    int counter = 0;
    for (double waveform : eventWaveform) {
        if ((counter >= min) && (counter <= max)) charge += waveform - fEvent.baseline;
        counter++;
    }
    double factor = 1;
    double shift = 0;
    if (usedParameters.factor.has_value()) factor = usedParameters.factor.value();
    if (usedParameters.shift.has_value()) shift = usedParameters.shift.value();
    charge = charge * factor + shift;
    fEvent.charge = charge;
}

void Device::DigitizerDevice::CalculateAmplitude(std::vector<int16_t> eventWaveform) {
    int index;
    if (usedParameters.signal.value() == "up") {
        auto value = std::max_element(eventWaveform.begin(), eventWaveform.end());
        index = std::distance(eventWaveform.begin(), value);
    } else if (usedParameters.signal.value() == "down") {
        auto value = std::min_element(eventWaveform.begin(), eventWaveform.end());
        index = std::distance(eventWaveform.begin(), value);
    }
    TF1* parabola = new TF1("parabola", "pol2", index-usedParameters.signalRange.value().first, index+usedParameters.signalRange.value().second);
    TGraph* gr = new TGraph();
    for (size_t i = 0; i < eventWaveform.size(); i++) {
        gr->SetPoint(i, i, eventWaveform[i]);
    }
    gr->Fit("parabola", "QR");
    double x = -parabola->GetParameter(1)/(2*parabola->GetParameter(2));
    fEvent.amplitude = parabola->GetParameter(2)*x*x+parabola->GetParameter(1)*x+parabola->GetParameter(0);
}

void Device::DigitizerDevice::CalculateWaveform(std::vector<int16_t> eventWaveform) {
    std::call_once(initWaveFlag, [this, eventWaveform](){InitializeSumWaveform(eventWaveform);});
    for (size_t i = 0; i < size(eventWaveform); i++) fEvent.waveform[i] += eventWaveform[i];
}

void Device::DigitizerDevice::InitializeSumWaveform(std::vector<int16_t> eventWaveform) {
    for (double waveform : eventWaveform) fEvent.waveform.push_back(waveform);
}

void Device::DigitizerDevice::ConfigureRoot() {
    // 1 channel
    // PSD
    if (usedParameters.qShort.has_value() || usedParameters.qLong.has_value() ||
        usedParameters.cfd_y1.has_value() || usedParameters.cfd_y2.has_value() ||
        usedParameters.baselinePSD.has_value() || usedParameters.height.has_value() ||
        usedParameters.eventCounter.has_value() || usedParameters.eventCounterPSD.has_value() ||
        usedParameters.psdValue.has_value()) {
            fTreePSD = new TTree("PSD", "PSD");
            
            if (usedParameters.qShort.has_value()) fTreePSD->Branch("qShort", &fEvent.qShort);
            if (usedParameters.qLong.has_value()) fTreePSD->Branch("qLong", &fEvent.qLong);
            if (usedParameters.cfd_y1.has_value()) fTreePSD->Branch("cfd_y1", &fEvent.cfd_y1);
            if (usedParameters.cfd_y2.has_value()) fTreePSD->Branch("cfd_y2", &fEvent.cfd_y2);
            if (usedParameters.baselinePSD.has_value()) fTreePSD->Branch("baseline", &fEvent.baselinePSD);
            if (usedParameters.height.has_value()) fTreePSD->Branch("height", &fEvent.height);
            if (usedParameters.eventCounter.has_value()) fTreePSD->Branch("eventCounter", &fEvent.eventCounter);
            if (usedParameters.eventCounterPSD.has_value()) fTreePSD->Branch("eventCounterPSD", &fEvent.eventCounterPSD);
            if (usedParameters.psdValue.has_value()) fTreePSD->Branch("psdValue", &fEvent.psdValue);
    }

    if (usedParameters.baseline.has_value() || usedParameters.charge.has_value() || usedParameters.amplitude.has_value()) {
        fTreeWaveform = new TTree("Waveform", "Waveform");
        if (usedParameters.baseline.has_value()) fTreeWaveform->Branch("baseline", &fEvent.baseline);
        if (usedParameters.charge.has_value()) fTreeWaveform->Branch("charge", &fEvent.charge);
        if (usedParameters.amplitude.has_value()) fTreeWaveform->Branch("amplitude", &fEvent.amplitude);
    }

    if (usedParameters.hist.has_value() || usedParameters.waveform.has_value()) {
        TDirectory* dir = fRootFile->mkdir("Histograms");
        dir->cd();

        if (usedParameters.hist.has_value()) {
            for (auto& hist : *usedParameters.hist) {
                std::string sName = hist.parameter + hist.file;
                TString name = TString(sName.c_str(), sName.length());
                TH1* h1 = new TH1D(name, name, hist.Nbins, hist.min, hist.max);
                fHist.push_back(h1);
            }
        }
    }
    
}

void Device::DigitizerDevice::ConfigureTxt() {
}