#include "../../include/tools/frame.h"

Frame::Frame()
    : header_(nullptr)
    , data_(nullptr)
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

Frame* ReadFrame(std::ifstream& stream) {
    FrameHeader* header = ReadFrameHeader(stream);

    if (!header) {
        return nullptr;
    }

    Frame* frame = new Frame();
    const size_t kDataSize = header->GetFrameSize();
    
    char* data = new char[kDataSize + 1];
    stream.read(data, kDataSize);
    data[kDataSize] = '\0';

    frame->AddFrameHeader(header);
    frame->AddData(data);

    return frame;
}
