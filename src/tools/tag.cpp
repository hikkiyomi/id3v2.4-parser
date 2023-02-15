#include "../../include/tools/tag.h"

Tag::Tag()
    : header_(nullptr)
{}

Tag::~Tag() {
    delete header_;

    for (auto& frame : frames_) {
        delete frame;
    }
}

void Tag::AddHeader(TagHeader* header) {
    header_ = header;
}

uint32_t Tag::GetTagSize() const {
    return header_->GetTagSize();
}

uint32_t Tag::GetExtendedHeaderSize() const {
    return header_->GetExtendedHeaderSize();
}

void Tag::AddFrame(Frame* frame) {
    frames_.push_back(frame);
}
