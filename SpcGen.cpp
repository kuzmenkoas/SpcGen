#include <iostream>

#include "ApplicationManager.h"
#include "ArgReader.h"
#include <memory>

#include <IDRSParser.h>

int main(int argc, char *argv[]) {
    std::shared_ptr<Core::ArgReader> argReader = std::make_shared<Core::ArgReader>(argc, argv);

    Core::ApplicationManager appManager(argReader);
    appManager.Run();

    return 0;
}