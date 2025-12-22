#pragma once

#include <memory>

#include "ArgReader.h"
#include "DRSDevice.h"
#include "DigitizerDevice.h"

namespace Core {
    class ApplicationManager {
    public:
        ApplicationManager(std::shared_ptr<ArgReader> argReader);
        ~ApplicationManager();
        void Run();
    private:
        std::shared_ptr<ArgReader> fArgReader = nullptr;
    };
}