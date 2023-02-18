#include "../../include/tools/frame_header.h"
#include "utils.h"

#include <string>
#include <vector>

FrameHeader::FrameHeader(
    const char* id,
    uint32_t size,
    const char* flags
)
    : frame_size_(size)
{
    for (size_t i = 0; i < kFrameHeaderIdSize; ++i) {
        id_[i] = id[i];
    }

    for (size_t i = 0; i < kFrameHeaderFlagsSize; ++i) {
        flags_[i] = flags[i];
    }
}

uint32_t FrameHeader::GetFrameSize() const {
    return frame_size_;
}

std::vector<std::string> GetFlags(const char* flags) {
    char c = (flags[0] >> 4) & 1;

    char h = (flags[1] >> 6) & 1;
    char k = (flags[1] >> 3) & 1;
    char m = (flags[1] >> 2) & 1;
    char n = (flags[1] >> 1) & 1;
    char p = (flags[1] >> 0) & 1;

    std::vector<std::string> result;

    if (c) {
        result.push_back("Read-only flag");
    }

    if (h) {
        result.push_back("Contains group info");
    }

    if (k) {
        result.push_back("Compressed");
    }

    if (m) {
        result.push_back("Encrypted");
    }

    if (n) {
        result.push_back("Unsynchronized");
    }

    if (p) {
        result.push_back("Has DLI");
    }

    return result;
}

void FrameHeader::PrintInfo(std::ofstream& stream) const {
    stream << "Frame ID: ";
    
    for (size_t i = 0; i < kFrameHeaderIdSize; ++i) {
        stream << id_[i];
    }

    stream << "\n";
    stream << "Frame size: " << frame_size_ << "\n";
    stream << "Flags: ";

    std::vector<std::string> flags = GetFlags(flags_);

    if (flags.size() == 0) {
        stream << "No flags";
    }

    for (size_t i = 0; i < flags.size(); ++i) {
        stream << flags[i];

        if (i != flags.size() - 1) {
            stream << ", ";
        }
    }

    stream << "\n\n";
}

FrameHeader* ReadFrameHeader(std::ifstream& stream) {
    char id[kFrameHeaderIdSize];
    char size_buffer[kFrameHeaderSizeBuffer];
    char flags[kFrameHeaderFlagsSize];

    stream.read(id, kFrameHeaderIdSize);

    if (memcmp(id, "\0\0\0\0", kFrameHeaderIdSize) == 0) {
        stream.seekg(-kFrameHeaderIdSize, stream.cur);
        
        return nullptr;
    }

    stream.read(size_buffer, kFrameHeaderSizeBuffer);
    stream.read(flags, kFrameHeaderFlagsSize);

    uint32_t size = SynchsafeToUInt32(BufferToUInt32(size_buffer, kFrameHeaderSizeBuffer));

    return new FrameHeader(id, size, flags);
}

bool FrameHeader::CompareID(char* id, size_t n) const {
    return strncmp(id, id_, n) == 0;
}
