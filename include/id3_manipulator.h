#pragma once

#include "tools/tag.h"
#include "tools/frame.h"

#include <filesystem>
#include <set>
#include <string>

class Manipulator {
public:
    explicit Manipulator(const std::filesystem::path& file_in, const std::filesystem::path& file_out, const std::set<std::string>& required_frames);
    Manipulator(const Manipulator& other) = delete;
    Manipulator& operator=(const Manipulator& other) = delete;
    ~Manipulator();

    void ReadTag();
    void PrintFrames() const;
private:
    std::filesystem::path file_in_;
    std::filesystem::path file_out_;
    std::set<std::string> required_frames_;
    Tag* tag_;
};
