#pragma once

#include "frame.h"

class UFID : public Frame {
public:
    UFID(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};

class TextFrame : public Frame {
public:
    TextFrame(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};
