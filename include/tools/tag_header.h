#pragma once

#include <cinttypes>
#include <cstring>
#include <fstream>
#include <stdexcept>

class TagHeader {
public:
    explicit TagHeader(
        char version,
        char revision,
        char flags,
        uint32_t tag_size
    );

    void Output() const;
private:
    char version_;
    char revision_;
    char flags_;
    uint32_t tag_size_;
};

TagHeader* ParseTagHeader(std::ifstream& stream);
