#include "IDevice.h"
#include "TString.h"

Device::IDevice::IDevice() {
}

Device::IDevice::~IDevice() {
}

void Device::IDevice::Prepare() {
    Parser::ParserFactory parserFactory;
    parserFactory.SetDigitizerTypes(fDigitizerTypes);
    parserFactory.SetCutFlag(bCut);
    if (GetConfigPath().empty()) fParser = parserFactory.BuildParser(fDeviceType);
    else fParser = parserFactory.BuildParser(fDeviceType, GetConfigPath());
    if (fDigitizerTypes.empty()) fDigitizerTypes = fParser->GetDigitizerTypes();
    for (std::string writer : fParser->GetUsedWriterVector()) {
        if (writer == "Root") ConfigureRoot();
    }

    this->PrepareDevice();
}

void Device::IDevice::ConfigureRoot() {
    std::string fileName = GetFileName()+".root";
    TString rFileName(fileName.c_str(), fileName.length());
    fRootFile = new TFile(rFileName, "recreate");
}

double Device::IDevice::MaxCCF(std::vector<double> ccfvector) {
    auto val = std::min_element(ccfvector.begin(), ccfvector.end());
    auto val2 = std::max_element(ccfvector.begin(), ccfvector.end());
    if (std::abs(*val2) > std::abs(*val)) val = val2;
    return std::abs(*val);
}

void Device::IDevice::DefineSignalDirection(std::vector<double> averageWaveform) {
    Global::Parameters usedParameters = GetParser()->GetUsedParameters();
    double baseline = Device::IDevice::TemplateCalculateBaseline(averageWaveform);
    double amplitude = Device::IDevice::TemplateCalculateAmplitude(averageWaveform, 0);
    if (amplitude > baseline) usedParameters.signal = "up";
    else usedParameters.signal = "down";
}