#pragma once

#include "frame_header.h"

#include <fstream>

class Frame {
public:
    Frame();
    Frame(const Frame& other) = delete;
    Frame& operator=(const Frame& other) = delete;
    ~Frame();

    void AddFrameHeader(FrameHeader* header);
    void AddData(char* data);
private:
    FrameHeader* header_;
    char* data_;
};

Frame* ReadFrame(std::ifstream& stream);
