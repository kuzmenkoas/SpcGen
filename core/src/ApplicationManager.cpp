#include "ApplicationManager.h"
#include <iostream>

Core::ApplicationManager::ApplicationManager(std::shared_ptr<ArgReader> argReader)
 : fArgReader(argReader)  {
}

Core::ApplicationManager::~ApplicationManager() {
}

void Core::ApplicationManager::Run() {
    Device::IDevice* device;
    if (fArgReader->GetDeviceType() == Global::DeviceType::DRS) device = new Device::DRSDevice();
    if (fArgReader->GetDeviceType() == Global::DeviceType::Digitizer) device = new Device::DigitizerDevice();
    if (device) {
        device->SetDeviceType(fArgReader->GetDeviceType());
    
        ce->SetDigitizerTypes(fArgReader->GetDigitizerTypes());
        ce->SetConfigPath(fArgReader->GetConfigPath());
        ce->SetBinaryPathVector(fArgReader->GetBinaryPathVector());
        ce->SetFileName(fArgReader->GetFileName());

        device->SetIsCut(fArgReader->IsCut());
        device->SetIsDebug(fArgReader->IsDebug());
        device->SetIsThreshold(fArgReader->IsThreshold());
    
        device->Prepare();
        device->Start();
    }
}