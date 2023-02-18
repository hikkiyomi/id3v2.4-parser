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

class TXXX : public Frame {
public:
    TXXX(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};

class URLFrame : public Frame {
public:
    URLFrame(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};

class WXXX : public Frame {
public:
    WXXX(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};

class MCDI : public Frame {
public:
    MCDI(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};

class ETCO : public Frame {
public:
    ETCO(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};

class USLT : public Frame {
public:
    USLT(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};

class SYLT : public Frame {
public:
    SYLT(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};
