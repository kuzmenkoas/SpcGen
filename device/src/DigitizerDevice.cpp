#include "DigitizerDevice.h"

Device::DigitizerDevice::DigitizerDevice() {
}

Device::DigitizerDevice::~DigitizerDevice() {
}

void Device::DigitizerDevice::PrepareDevice() {
    for (std::string writer : GetParser()->GetUsedWriterVector()) {
        if (writer == "Root") ConfigureRoot();
        if (writer == "Txt") ConfigureTxt();
    }
}

void Device::DigitizerDevice::Start() {
    int i = 0;
    for (std::string file : GetDigitizerTypes()) {
        if (file == "PSD") ProcessPSD(GetBinaryPathVector()[i++]);
        if (file == "Waveform") ProcessWaveform(GetBinaryPathVector()[i++]);
    }

    for (std::string writer : GetParser()->GetUsedWriterVector()) {
        if (writer == "Root") fRootFile->Write();
        // if (writer == "Txt") fTxtFile.close();
    }
}

void Device::DigitizerDevice::ProcessPSD(std::filesystem::path path) {
    Global::Parameters usedParameters = GetParser()->GetUsedParameters();
    std::ifstream file(path.string(), std::ios::binary | std::ios::ate);
    file.seekg(0, std::ios::beg);

    int parNumber = 0;
    if (usedParameters.qShort.has_value()) parNumber++;
    if (usedParameters.qLong.has_value()) parNumber++;
    if (usedParameters.cfd_y1.has_value()) parNumber++;
    if (usedParameters.cfd_y2.has_value()) parNumber++;
    if (usedParameters.baselinePSD.has_value()) parNumber++;
    if (usedParameters.height.has_value()) parNumber++;
    if (usedParameters.eventCounter.has_value()) parNumber++;
    if (usedParameters.eventCounterPSD.has_value()) parNumber++;
    if (usedParameters.psdValue.has_value()) parNumber++;

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
    }
    file.close();
}

void Device::DigitizerDevice::ProcessWaveform(std::filesystem::path path) {
}

void Device::DigitizerDevice::ConfigureRoot() {
    // 1 channel
    Global::Parameters usedParameters = GetParser()->GetUsedParameters();

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
        if (usedParameters.baseline.has_value()) fTreeWaveform->Branch("baseline", &fEvent.baseline, "baseline/I");
        if (usedParameters.charge.has_value()) fTreeWaveform->Branch("charge", &fEvent.charge, "charge/I");
        if (usedParameters.amplitude.has_value()) fTreeWaveform->Branch("amplitude", &fEvent.amplitude, "amplitude/I");
    }

    if (usedParameters.hist.has_value() || usedParameters.waveform.has_value()) {
        TDirectory* dir = fRootFile->mkdir("Histograms");
        dir->cd();
    }
    
}

void Device::DigitizerDevice::ConfigureTxt() {
}