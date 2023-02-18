#include "../../include/tools/frame.h"
#include "../../include/tools/frame_list.h"

#include <iostream>
#include <set>

const std::set<std::string> kRestrictedFrames = {
    "AENC",
    "APIC",
    "ASPI",
    "GEOB",
    "MCDI",
    "MLLT",
    "RVRB",
    "SIGN",
    "SYTC",
    "TFLT",
    "TMED"
};

Frame::Frame()
    : header_(nullptr)
    , data_(nullptr)
{}

Frame::Frame(FrameHeader* header, char* data)
    : header_(header)
    , data_(data)
{}

Frame::~Frame() {
    delete header_;
    delete data_;
}

void Frame::AddFrameHeader(FrameHeader* header) {
    header_ = header;
}

void Frame::AddData(char* data) {
    data_ = data;
}

void Frame::PrintTo(std::ofstream& stream) const {
    header_->PrintInfo(stream);

    stream << "This frame is restricted or unknown. Data will not be printed.";

    stream.put('\n');
    stream << "--------------\n";
}

bool IsRestricted(FrameHeader* header) {
    return kRestrictedFrames.count(header->GetStringIdentifier());
}

Frame* ReadFrame(std::ifstream& stream) {
    FrameHeader* header = ReadFrameHeader(stream);

    if (!header) {
        return nullptr;
    }

    const size_t kDataSize = header->GetFrameSize();
    
    char* data = new char[kDataSize + 1];
    stream.read(data, kDataSize);
    data[kDataSize] = '\0';

    if (IsRestricted(header)) {
        return new Frame(header, data);
    }

    if (header->CompareID("UFID", 4)) {
        return new UFID(header, data);
    } else if (header->CompareID("TXXX", 4)) {
        return new TXXX(header, data);
    } else if (header->CompareID("T", 1)) {
        return new TextFrame(header, data);
    } else if (header->CompareID("WXXX", 4)) {
        return new WXXX(header, data);
    } else if (header->CompareID("W", 1)) {
        return new URLFrame(header, data);
    // } else if (header->CompareID("MCDI", 4)) {
    //     return new MCDI(header, data); // ! THIS FRAME WAS NOT NECESSARY, SO I WILL COMMENT IT
    } else if (header->CompareID("ETCO", 4)) {
        return new ETCO(header, data);
    } else if (header->CompareID("USLT", 4)) {
        return new USLT(header, data);
    } else if (header->CompareID("SYLT", 4)) {
        return new SYLT(header, data);
    } else if (header->CompareID("COMM", 4)) {
        return new COMM(header, data);
    } else if (header->CompareID("RVA2", 4)) {
        return new RVA2(header, data);
    } else if (header->CompareID("EQU2", 4)) {
        return new EQU2(header, data);
    } else if (header->CompareID("PCNT", 4)) {
        return new PCNT(header, data);
    } else if (header->CompareID("POPM", 4)) {
        return new POPM(header, data);
    } else if (header->CompareID("RBUF", 4)) {
        return new RBUF(header, data);
    } else if (header->CompareID("LINK", 4)) {
        return new LINK(header, data);
    } else if (header->CompareID("POSS", 4)) {
        return new POSS(header, data);
    } else if (header->CompareID("USER", 4)) {
        return new USER(header, data);
    } else if (header->CompareID("OWNE", 4)) {
        return new OWNE(header, data);
    } else if (header->CompareID("COMR", 4)) {
        return new COMR(header, data);
    } else if (header->CompareID("ENCR", 4) || header->CompareID("GRID", 4)) {
        return new RegistrationFrame(header, data);
    } else if (header->CompareID("PRIV", 4)) {
        return new PRIV(header, data);
    } else if (header->CompareID("SEEK", 4)) {
        return new SEEK(header, data);
    }

    return new Frame(header, data);
}
