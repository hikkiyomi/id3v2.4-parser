#include "../include/id3_manipulator.h"

#include <iostream>

Manipulator::Manipulator(const std::filesystem::path& file_in, const std::filesystem::path& file_out, const std::set<std::string>& required_frames)
    : file_in_(file_in)
    , file_out_(file_out)
    , required_frames_(required_frames)
    , tag_(new Tag())
{}

Manipulator::~Manipulator()
{
    delete tag_;
}

void Manipulator::ReadTag() {
    std::ifstream stream(file_in_, std::ios::binary);

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

        if (required_frames_.empty() || required_frames_.find(pending->GetStringIdentifier()) != required_frames_.end()) {
            tag_->AddFrame(pending);
        }
    }
}

void Manipulator::PrintFrames() const {
    std::ofstream stream(file_out_, std::ios::binary);
    tag_->PrintFrames(stream);
}

// void Manipulator::Debug() const {
//     header_->Output();
// }
