#pragma once

#include "tools/tag.h"
#include "tools/frame.h"

#include <filesystem>

class Manipulator {
public:
    explicit Manipulator(const std::filesystem::path& file);
    Manipulator(const Manipulator& other) = delete;
    Manipulator& operator=(const Manipulator& other) = delete;
    ~Manipulator();

    void ReadTag();
    // void Debug() const;
private:
    std::filesystem::path file_;
    Tag* tag_;
};
