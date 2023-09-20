#include "../../include/tools/tag_header.h"
#include "utils.h"

#include <iostream>
#include <string>

TagHeader::TagHeader(
    char version,
    char revision,
    char flags,
    uint32_t tag_size,
    uint32_t extended_header_size
)
    : version_(version)
    , revision_(revision)
    , flags_(flags)
    , tag_size_(tag_size)
    , extended_header_size_(extended_header_size)
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

uint32_t TagHeader::GetTagSize() const {
    return tag_size_;
}

uint32_t TagHeader::GetExtendedHeaderSize() const {
    return extended_header_size_;
}

bool HasExtendedHeader(const char flags) {
    return (flags >> 6) & 1;
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

    uint32_t tag_size = SynchsafeToUInt32(BufferToUInt32(size_buffer, kTagSizeBytes));
    uint32_t extended_header_size = 0;

    if (HasExtendedHeader(flags)) {
        char extended_header_size_buffer[kExtendedHeaderSizeBytes];
        stream.read(extended_header_size_buffer, kExtendedHeaderSizeBytes);
        extended_header_size = SynchsafeToUInt32(BufferToUInt32(extended_header_size_buffer, kExtendedHeaderSizeBytes));
    }

    return new TagHeader(version, revision, flags, tag_size, extended_header_size);
}
