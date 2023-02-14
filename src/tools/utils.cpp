#include "utils.h"

int32_t BufferToInt32(const char* buffer, size_t buffer_size) {
    int32_t result = 0;

    for (size_t block = 0; block < buffer_size; ++block) {
        result = (result << 8) | buffer[block];
    }

    return result;
}

int32_t SynchsafeToInt32(const int32_t size_sync) {
    int32_t block1 = size_sync & 0xFF;
    int32_t block2 = (size_sync >> 8) & 0xFF;
    int32_t block3 = (size_sync >> 16) & 0xFF;
    int32_t block4 = (size_sync >> 24) & 0xFF;
    int32_t result = 0;

    result |= block1;
    result |= (block2 << 7);
    result |= (block3 << 14);
    result |= (block4 << 21);

    return result;
}
