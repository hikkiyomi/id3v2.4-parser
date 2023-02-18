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
    }

    return new Frame(header, data);
}
