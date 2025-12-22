#include "ApplicationManager.h"
#include <iostream>

Core::ApplicationManager::ApplicationManager(std::shared_ptr<ArgReader> argReader)
 : fArgReader(argReader)  {
}

Core::ApplicationManager::~ApplicationManager() {
}

void Core::ApplicationManager::Run() {
    Device::IDevice device;
    if (fArgReader->GetDeviceType() == Global::DeviceType::DRS) device = Device::DRSDevice();
    // if (fArgReader->GetDeviceType() == Global::DeviceType::Digitizer) device = Device::DRSDevice();
    device.SetConfigPath(fArgReader->GetConfigPath());
    device.SetBinaryPathVector(fArgReader->GetBinaryPathVector());
    device.SetFileName(fArgReader->GetFileName());
    
    device.Prepare();
    device.Start();
}