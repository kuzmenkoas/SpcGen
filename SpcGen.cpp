#include <iostream>

#include <memory>
#include "ArgReader.h"
#include "ApplicationManager.h"

#include <IDRSParser.h>

int main(int argc, char *argv[]) {
    std::shared_ptr<Core::ArgReader> argReader = std::make_shared<Core::ArgReader>(argc, argv);

    Core::ApplicationManager appManager(argReader);
    appManager.Run();

    return 0;
}