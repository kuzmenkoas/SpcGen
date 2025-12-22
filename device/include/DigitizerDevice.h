#pragma once

#include "IDevice.h"

namespace Device {
    class DigitizerDevice : public IDevice {
    public:
        DigitizerDevice();
        ~DigitizerDevice();

        void PrepareDevice() final;
        void Start() final;
    private:
    };
}