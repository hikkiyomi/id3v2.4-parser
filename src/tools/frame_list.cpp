#include "../../include/tools/frame_list.h"

UFID::UFID(FrameHeader* header, char* data)
    : Frame(header, data)
{}

void UFID::PrintTo(std::ofstream& stream) const {
    header_->PrintInfo(stream);
    
    std::string owner_identifier;
    std::string identifier;
    bool change = false;

    for (size_t i = 0; i < header_->GetFrameSize(); ++i) {
        if (i == 0x00) {
            change = true;
            continue;
        }

        if (!change) {
            owner_identifier += data_[i];
        } else {
            identifier += data_[i];
        }
    }

    stream << "Owner identifier: " << owner_identifier << "\n";
    stream << "Identifier: " << identifier << "\n";

    stream.put('\n');
    stream << "--------------\n";
}

TextFrame::TextFrame(FrameHeader* header, char* data)
    : Frame(header, data)
{}

void TextFrame::PrintTo(std::ofstream& stream) const {
    header_->PrintInfo(stream);

    std::string encoding;
    
    if (data_[0] == 0x00) {
        encoding = "ISO-8859-1";
    } else if (data_[0] == 0x01) {
        encoding = "UTF-16";
    } else if (data_[0] == 0x02) {
        encoding = "UTF-16BE";
    } else if (data_[0] == 0x03) {
        encoding = "UTF-8";
    } else {
        encoding = "Unknown";
    }

    stream << "Encoding: " << encoding << "\n";
    stream << "Data: ";

    for (size_t i = 1; i < header_->GetFrameSize(); ++i) {
        stream.put(data_[i]);
    }

    stream.put('\n');
    stream << "--------------\n";
}
