#include "../include/id3_manipulator.h"

#include <iostream>

Manipulator::Manipulator(const std::filesystem::path& file)
    : file_(file)
    , tag_(new Tag())
{}

Manipulator::~Manipulator() {
    delete tag_;
}

void Manipulator::ReadTag() {
    std::ifstream stream(file_, std::ios::binary);

    if (!stream.is_open()) {
        throw std::runtime_error("Cannot open the file.");
    }

    tag_->AddHeader(ParseTagHeader(stream));

    if (tag_->GetExtendedHeaderSize() > 0) {
        uint32_t need_to_skip = tag_->GetExtendedHeaderSize() - kExtendedHeaderSizeBytes;
        stream.seekg(need_to_skip, stream.cur);
    }

    while (stream.tellg() < tag_->GetTagSize()) {
        Frame* pending = ReadFrame(stream);

        if (!pending) {
            break;
        }

        tag_->AddFrame(pending);
    }

    std::cout << "ehe";
}

// void Manipulator::Debug() const {
//     header_->Output();
// }
