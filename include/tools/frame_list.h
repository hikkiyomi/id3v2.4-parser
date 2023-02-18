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

class COMM : public Frame {
public:
    COMM(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};

class RVA2 : public Frame {
public:
    RVA2(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};

class EQU2 : public Frame {
public:
    EQU2(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};

class PCNT : public Frame {
public:
    PCNT(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};

class POPM : public Frame {
public:
    POPM(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};

class RBUF : public Frame {
public:
    RBUF(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};

class LINK : public Frame {
public:
    LINK(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};

class POSS : public Frame {
public:
    POSS(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};

class USER : public Frame {
public:
    USER(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};

class OWNE : public Frame {
public:
    OWNE(FrameHeader* header, char* data);
    
    void PrintTo(std::ofstream& stream) const;
};

class COMR : public Frame {
public:
    COMR(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};

class RegistrationFrame : public Frame {
public:
    RegistrationFrame(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};

class PRIV : public Frame {
public:
    PRIV(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};

class SEEK : public Frame {
public:
    SEEK(FrameHeader* header, char* data);

    void PrintTo(std::ofstream& stream) const;
};
