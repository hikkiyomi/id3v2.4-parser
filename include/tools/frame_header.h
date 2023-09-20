#pragma once

#include <cinttypes>
#include <cstring>
#include <fstream>

constexpr size_t kFrameHeaderIdSize = 4;
constexpr size_t kFrameHeaderSizeBuffer = 4;
constexpr size_t kFrameHeaderFlagsSize = 2;

class FrameHeader {
public:
    explicit FrameHeader(
        const char* id,
        uint32_t size,
        const char* flags
    );

    uint32_t GetFrameSize() const;
    void PrintInfo(std::ofstream& stream) const;
    std::string GetStringIdentifier() const;
    
    const char* GetFlags() const;
    bool CompareID(char* id, size_t n) const;
private:
    char id_[kFrameHeaderIdSize];
    uint32_t frame_size_;
    char flags_[kFrameHeaderFlagsSize];
};

FrameHeader* ReadFrameHeader(std::ifstream& stream);
