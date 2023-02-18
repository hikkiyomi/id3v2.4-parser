#pragma once

#include "tools/tag.h"
#include "tools/frame.h"

#include <filesystem>

class Manipulator {
public:
    explicit Manipulator(const std::filesystem::path& file_in, const std::filesystem::path& file_out);
    Manipulator(const Manipulator& other) = delete;
    Manipulator& operator=(const Manipulator& other) = delete;
    ~Manipulator();

    void ReadTag();
    // void Debug() const;
    void PrintFrames() const;
private:
    std::filesystem::path file_in_;
    std::filesystem::path file_out_;
    Tag* tag_;
};
