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
    if (parser) parser->Start();
    return parser;
}

std::shared_ptr<Parser::IParser> Parser::ParserFactory::BuildParser(Global::DeviceType aType, std::filesystem::path path) {
    std::shared_ptr<Parser::IParser> parser = nullptr;
    switch (aType) {
        case Global::DeviceType::DRS:
            parser = std::make_shared<DRSConfigParser>(path);
            break;
        case Global::DeviceType::Digitizer:
            break;
    }
    if (parser) parser->Start();
    return parser;
}