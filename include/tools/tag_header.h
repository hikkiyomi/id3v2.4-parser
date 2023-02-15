#pragma once

#include <cinttypes>
#include <cstring>
#include <fstream>
#include <stdexcept>

constexpr size_t kTagHeaderIdentifierSize = 3;
constexpr size_t kTagSizeBytes = 4;
constexpr size_t kExtendedHeaderSizeBytes = 4;

class TagHeader {
public:
    explicit TagHeader(
        char version,
        char revision,
        char flags,
        uint32_t tag_size,
        uint32_t extended_header_size
    );

    void Output() const;
    uint32_t GetTagSize() const;
    uint32_t GetExtendedHeaderSize() const;
private:
    char version_;
    char revision_;
    char flags_;
    uint32_t tag_size_;
    uint32_t extended_header_size_;
};

TagHeader* ParseTagHeader(std::ifstream& stream);
