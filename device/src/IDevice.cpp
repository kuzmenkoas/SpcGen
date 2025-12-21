#include "IDevice.h"
#include "TString.h"

Device::IDevice::IDevice() {
}

Device::IDevice::~IDevice() {
}

void Device::IDevice::Prepare() {
    Parser::ParserFactory parserFactory;
    if (GetConfigPath().empty()) fParser = parserFactory.BuildParser(Global::DeviceType::DRS);
    else fParser = parserFactory.BuildParser(Global::DeviceType::DRS);
    
    for (std::string writer : fParser->GetUsedWriterVector()) {
        if (writer == "Root") ConfigureRoot();
    }

    this->PrepareDevice();
}

void Device::IDevice::ConfigureRoot() {
    std::string fileName = GetFileName()+".root";
    TString rFileName = TString(fileName);
    fRootFile = new TFile(rFileName, "recreate");
}