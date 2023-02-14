#include "../../include/tools/tag_header.h"
#include "utils.h"

#include <iostream>
#include <string>

constexpr size_t kTagHeaderIdentifierSize = 3;
constexpr size_t kTagSizeBytes = 4;

TagHeader::TagHeader(
    char version,
    char revision,
    char flags,
    uint32_t tag_size
)
    : version_(version)
    , revision_(revision)
    , flags_(flags)
    , tag_size_(tag_size)
{}

std::string GetBitString(const char byte) {
    std::string result;

    for (size_t i = 0; i < 8; ++i) {
        bool bit = (byte >> i) & 1;

        if (bit) {
            result += "1";
        } else {
            result += "0";
        }
    }

    return result;
}

void TagHeader::Output() const {
    std::cout << "Version: " << version_ << "\n";
    std::cout << "Revision: " << revision_ << "\n";
    std::cout << "Flags: " << GetBitString(flags_) << "\n";
    std::cout << "Tag size: " << tag_size_ << "\n";
    std::cout << "-------------\n";
}

TagHeader* ParseTagHeader(std::ifstream& stream) {
    char identifier[kTagHeaderIdentifierSize];
    
    stream.read(identifier, kTagHeaderIdentifierSize);

    if (!strcmp(identifier, "ID3")) {
        throw std::runtime_error("File does not have an ID3 metadata.");
    }

    char version;
    char revision;
    char flags;
    char size_buffer[kTagSizeBytes];

    stream.get(version);
    stream.get(revision);

    if (version != 4) {
        throw std::runtime_error("File does not have an ID3v2.4 metadata");
    }
    
    stream.get(flags);
    stream.read(size_buffer, kTagSizeBytes);

    int32_t tag_size = SynchsafeToInt32(BufferToInt32(size_buffer, kTagSizeBytes));

    return new TagHeader(version, revision, flags, tag_size);
}
