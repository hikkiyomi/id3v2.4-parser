#pragma once

#include <filesystem>
#include <set>
#include <string>

class ConsoleParser {
public:
    void Parse(int argc, char** argv);
    void Help() const;

    std::filesystem::path input;
    std::filesystem::path output;
    std::set<std::string> required_frames;
};
