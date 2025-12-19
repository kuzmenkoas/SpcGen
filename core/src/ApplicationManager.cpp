#include "ApplicationManager.h"
#include <iostream>

Core::ApplicationManager::ApplicationManager(std::shared_ptr<ArgReader> argReader)
 : fArgReader(argReader)  {
}

Core::ApplicationManager::~ApplicationManager() {
}

void Core::ApplicationManager::Run() {
    Device::DRSDevice drsDevice;
    drsDevice.SetConfigPath(fArgReader->GetConfigPath());
    drsDevice.SetBinaryPathVector(fArgReader->GetBinaryPathVector());
    drsDevice.SetFileName(fArgReader->GetFileName());
    
    drsDevice.Prepare();
    drsDevice.Start();
}