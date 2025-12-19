#include "ParserFactory.h"

Parser::ParserFactory::ParserFactory() {
}

Parser::ParserFactory::~ParserFactory() {
}

std::shared_ptr<Parser::IParser> Parser::ParserFactory::BuildParser(Global::DeviceType aType) {
    std::shared_ptr<Parser::IParser> parser = nullptr;
    switch (aType) {
        case Global::DeviceType::DRS:
            parser = std::make_shared<DRSConsoleParser>();
            break;
        case Global::DeviceType::Digitizer:
            break;
    }
    return parser;
}

std::shared_ptr<Parser::IParser> Parser::ParserFactory::BuildParser(Global::DeviceType aType, std::filesystem::path path) {
    std::shared_ptr<Parser::IParser> parser = nullptr;

    return parser;
}