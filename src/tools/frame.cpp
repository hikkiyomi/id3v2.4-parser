#include "../../include/tools/frame.h"
#include "../../include/tools/frame_list.h"

#include <iostream>

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

void Frame::Split() const {
    std::cerr << "Cannot split base class.";
}

void Frame::PrintTo(std::ofstream& stream) const {
    header_->PrintInfo(stream);

    stream << "Data: ";

    for (size_t i = 0; i < header_->GetFrameSize(); ++i) {
        stream.put(data_[i]);
    }

    stream.put('\n');
    stream << "--------------\n";
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

    if (header->CompareID("UFID", 4)) {
        return new UFID(header, data);
    } else if (header->CompareID("T", 1)) {
        return new TextFrame(header, data);
    }

    return new Frame(header, data);
}
