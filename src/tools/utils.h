#pragma once

#include <cinttypes>

uint32_t BufferToUInt32(const char* buffer, size_t buffer_size);
uint32_t SynchsafeToUInt32(const uint32_t size_sync);
