#include "../../include/tools/frame_header.h"
#include "utils.h"

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
