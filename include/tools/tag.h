#pragma once

#include "tag_header.h"
#include "frame.h"

#include <list>

class Tag {
public:
    Tag();
    Tag(const Tag& other) = delete;
    Tag& operator=(const Tag& other) = delete;
    ~Tag();

    void AddHeader(TagHeader* header);
    uint32_t GetTagSize() const;
    uint32_t GetExtendedHeaderSize() const;
    void AddFrame(Frame* frame);
private:
    TagHeader* header_;
    std::list<Frame*> frames_;
};
