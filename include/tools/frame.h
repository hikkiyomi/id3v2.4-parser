#pragma once

#include "frame_header.h"

#include <fstream>

class Frame {
public:
    Frame();
    Frame(FrameHeader* header, char* data);
    Frame(const Frame& other) = delete;
    Frame& operator=(const Frame& other) = delete;
    ~Frame();

    void AddFrameHeader(FrameHeader* header);
    void AddData(char* data);
    std::string GetStringIdentifier() const;

    virtual void PrintTo(std::ofstream& stream) const;
protected:
    FrameHeader* header_;
    char* data_;
};

Frame* ReadFrame(std::ifstream& stream);
