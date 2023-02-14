#pragma once

#include "tools/tag_header.h"

#include <filesystem>

class Manipulator {
public:
    explicit Manipulator(const std::filesystem::path& file);
    Manipulator(const Manipulator& other) = delete;
    Manipulator& operator=(const Manipulator& other) = delete;
    ~Manipulator();

    void ReadTagHeader();
    void Debug() const;
private:
    std::filesystem::path file_;
    TagHeader* header_;
};
