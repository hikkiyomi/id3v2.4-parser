#include "../../include/tools/frame_list.h"
#include "utils.h"

#include <map>
#include <string>
#include <vector>

constexpr char kFalseSyncByte = static_cast<char>(0xFF);
std::map<char, std::string> kEventType;

const std::string kContentType[] = {
    "is other",
    "is lyrics",
    "is text transcription",
    "is movement/part name",
    "is events",
    "is chord",
    "is trivia/\'pop up\' information",
    "is URLs to webpages",
    "is URLs to images"
};

const std::string kTypeOfChannel[] = {
    "Other",
    "Master volume",
    "Front right",
    "Front left",
    "Back right",
    "Back left",
    "Front centre",
    "Back centre",
    "Subwoofer"
};

void CalculateEventType() {
    kEventType[0x00] = "padding (has no meaning)";
    kEventType[0x01] = "end of initial silence";
    kEventType[0x02] = "intro start";
    kEventType[0x03] = "main part start";
    kEventType[0x04] = "outro start";
    kEventType[0x05] = "outro end";
    kEventType[0x06] = "verse start";
    kEventType[0x07] = "refrain start";
    kEventType[0x08] = "interlude start";
    kEventType[0x09] = "theme start";
    kEventType[0x0A] = "variation start";
    kEventType[0x0B] = "key change";
    kEventType[0x0C] = "time change";
    kEventType[0x0D] = "momentary unwanted noise (Snap, Crackle & Pop)";
    kEventType[0x0E] = "sustained noise";
    kEventType[0x0F] = "sustained noise end";
    kEventType[0x10] = "intro end";
    kEventType[0x11] = "main part end";
    kEventType[0x12] = "verse end";
    kEventType[0x13] = "refrain end";
    kEventType[0x14] = "theme end";
    kEventType[0x15] = "profanity";
    kEventType[0x16] = "profanity end";

    for (uint8_t x = 0x17; x <= 0xDF; ++x) {
        kEventType[static_cast<char>(x)] = "reserved for future use";
    }

    for (uint8_t x = 0xE0; x <= 0xEF; ++x) {
        kEventType[static_cast<char>(x)] = "not predefined synch 0-F";
    }

    for (uint8_t x = 0xF0; x <= 0xFC; ++x) {
        kEventType[static_cast<char>(x)] = "reserved for future use";
    }

    kEventType[0xFD] = "audio end (start of silence)";
    kEventType[0xFE] = "audio file ends";
    kEventType[0xFF] = "one more byte of events follows (all the following bytes with the value $FF have the same function)";
}

UFID::UFID(FrameHeader* header, char* data)
    : Frame(header, data)
{}

void UFID::PrintTo(std::ofstream& stream) const {
    header_->PrintInfo(stream);
    
    std::string owner_identifier;
    std::string identifier;
    bool change = false;
    bool unsync = IsUnsync(header_->GetFlags());

    for (size_t i = 0; i < header_->GetFrameSize(); ++i) {
        if (i == 0x00) {
            if (unsync && i > 0 && data_[i - 1] == kFalseSyncByte) {
                continue;
            }

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

std::string GetEncoding(const char* data) {
    if (data[0] == 0x00) {
        return "ISO-8859-1";
    } else if (data[0] == 0x01) {
        return "UTF-16";
    } else if (data[0] == 0x02) {
        return "UTF-16BE";
    } else if (data[0] == 0x03) {
        return "UTF-8";
    } 

    return "Unknown";
}

void TextFrame::PrintTo(std::ofstream& stream) const {
    header_->PrintInfo(stream);

    std::string encoding = GetEncoding(data_);
    bool unsync = IsUnsync(header_->GetFlags());

    stream << "Encoding: " << encoding << "\n";
    stream << "Data: ";

    for (size_t i = 1; i < header_->GetFrameSize(); ++i) {
        if (data_[i] == 0x00) {
            if (unsync && data_[i - 1] == kFalseSyncByte) {
                continue;
            }

            stream.put(' ');

            continue;
        }

        stream.put(data_[i]);
    }

    stream.put('\n');
    stream << "--------------\n";
}

TXXX::TXXX(FrameHeader* header, char* data)
    : Frame(header, data)
{}

void PrintSpecialFrame(std::ofstream& stream, FrameHeader* header_, char* data_) {
    header_->PrintInfo(stream);

    std::string encoding = GetEncoding(data_);
    bool unsync = IsUnsync(header_->GetFlags());

    stream << "Encoding: " << encoding << "\n";

    bool change = false;
    std::string description;
    std::string value;

    for (size_t i = 1; i < header_->GetFrameSize(); ++i) {
        if (data_[i] == 0x00) {
            if (unsync && data_[i - 1] == kFalseSyncByte) {
                continue;
            }

            change = true;

            continue;
        }

        if (!change) {
            description += data_[i];
        } else {
            value += data_[i];
        }
    }

    stream << "Description: " << description << "\n";
    stream << "Value: " << value << "\n";

    stream.put('\n');
    stream << "--------------\n";
}

void TXXX::PrintTo(std::ofstream& stream) const {
    PrintSpecialFrame(stream, header_, data_);
}

URLFrame::URLFrame(FrameHeader* header, char* data)
    : Frame(header, data)
{}

void PrintDataWithoutUnsync(std::ofstream& stream, FrameHeader* header, char* data) {
    if (!IsUnsync(header->GetFlags())) {
        stream.write(data, header->GetFrameSize());
        stream.put('\n');
        stream << "--------------\n";

        return;
    }

    for (size_t i = 0; i < header->GetFrameSize(); ++i) {
        if (data[i] == 0x00) {
            if (i > 0 && data[i - 1] == kFalseSyncByte) {
                continue;
            }

            continue;
        }

        stream.put(data[i]);
    }
    
    stream.put('\n');
    stream << "--------------\n";
}

void URLFrame::PrintTo(std::ofstream& stream) const {
    header_->PrintInfo(stream);
    stream << "URL: ";
    PrintDataWithoutUnsync(stream, header_, data_);
}

WXXX::WXXX(FrameHeader* header, char* data)
    : Frame(header, data)
{}

void WXXX::PrintTo(std::ofstream& stream) const {
    PrintSpecialFrame(stream, header_, data_);
}

MCDI::MCDI(FrameHeader* header, char* data)
    : Frame(header, data)
{}

void MCDI::PrintTo(std::ofstream& stream) const {
    header_->PrintInfo(stream);
    stream << "CD TOC: ";
    PrintDataWithoutUnsync(stream, header_, data_);
}

ETCO::ETCO(FrameHeader* header, char* data)
    : Frame(header, data)
{}

void ETCO::PrintTo(std::ofstream& stream) const {
    header_->PrintInfo(stream);
    
    stream << "Time stamp format: ";

    if (data_[0] == 0x01) {
        stream << "Absolute time, 32 bit sized, using MPEG [MPEG] frames as unit\n";
    } else if (data_[0] == 0x02) {
        stream << "Absolute time, 32 bit sized, using milliseconds as unit\n";
    } else {
        stream << "Unknown\n";
    }

    CalculateEventType();

    stream << "Events:\n";
    
    for (size_t i = 1; i < header_->GetFrameSize(); i += 5) {
        std::string type_of_event = kEventType[data_[i]];
        uint32_t size = 0;

        size = (size << 8) | static_cast<uint8_t>(data_[i + 1]);
        size = (size << 8) | static_cast<uint8_t>(data_[i + 2]);
        size = (size << 8) | static_cast<uint8_t>(data_[i + 3]);
        size = (size << 8) | static_cast<uint8_t>(data_[i + 4]);

        stream << "[" << type_of_event << "]: " << size << "\n";
    }

    stream.put('\n');
    stream << "--------------\n";
}

USLT::USLT(FrameHeader* header, char* data)
    : Frame(header, data)
{}

void USLT::PrintTo(std::ofstream& stream) const {
    header_->PrintInfo(stream);

    std::string encoding = GetEncoding(data_);
    std::string language;

    for (size_t i = 1; i <= 3; ++i) {
        language.push_back(data_[i]);
    }

    std::string descriptor;
    std::string lyrics;
    bool change = false;
    bool unsync = IsUnsync(header_->GetFlags());

    for (size_t i = 4; i < header_->GetFrameSize(); ++i) {
        if (data_[i] == 0x00) {
            if (unsync && data_[i - 1] == kFalseSyncByte) {
                continue;
            }

            change = true;
            
            continue;   
        }

        if (!change) {
            descriptor += data_[i];
        } else {
            lyrics += data_[i];
        }
    }

    stream << "Encoding: " << encoding << "\n";
    stream << "Language: " << language << "\n";
    stream << "Content descriptor: " << descriptor << "\n";
    stream << "Lyrics/text: " << lyrics << "\n";

    stream.put('\n');
    stream << "--------------\n";
}

SYLT::SYLT(FrameHeader* header, char* data)
    : Frame(header, data)
{}

void SYLT::PrintTo(std::ofstream& stream) const {
    header_->PrintInfo(stream);

    std::string encoding = GetEncoding(data_);
    std::string language;

    for (size_t i = 1; i <= 3; ++i) {
        language.push_back(data_[i]);
    }

    stream << "Encoding: " << encoding << "\n";
    stream << "Language: " << language << "\n";
    stream << "Time stamp format: ";

    if (data_[4] == 0x01) {
        stream << "Absolute time, 32 bit sized, using MPEG [MPEG] frames as unit";
    } else if (data_[4] == 0x02) {
        stream << "Absolute time, 32 bit sized, using milliseconds as unit";
    } else {
        stream << "Unknown";
    }
    
    stream << "\n";

    std::string content_type = kContentType[data_[5]];
    std::string content_descriptor;
    bool unsync = IsUnsync(header_->GetFlags());
    size_t i = 6;

    for (; i < header_->GetFrameSize(); ++i) {
        if (data_[i] == 0x00) {
            if (unsync && data_[i - 1] == kFalseSyncByte) {
                continue;
            }

            ++i;
        
            break;
        }

        content_descriptor += data_[i];
    }

    stream << content_descriptor << "\n\n";

    while (i < header_->GetFrameSize()) {
        std::string text;

        for (; i < header_->GetFrameSize(); ++i) {
            if (data_[i] == 0x00) {
                if (unsync && data_[i - 1] == kFalseSyncByte) {
                    continue;
                }

                break;
            }

            text += data_[i];
        }

        uint32_t size = 0;

        size = (size << 8) | static_cast<uint8_t>(data_[i + 1]);
        size = (size << 8) | static_cast<uint8_t>(data_[i + 2]);
        size = (size << 8) | static_cast<uint8_t>(data_[i + 3]);
        size = (size << 8) | static_cast<uint8_t>(data_[i + 4]);

        i += 5;

        stream << "[Time: " << size << "]: " << text << "\n";
    }

    stream.put('\n');
    stream << "--------------\n";
}

COMM::COMM(FrameHeader* header, char* data)
    : Frame(header, data)
{}

void COMM::PrintTo(std::ofstream& stream) const {
    header_->PrintInfo(stream);
    
    std::string encoding = GetEncoding(data_);
    std::string language;

    for (size_t i = 1; i <= 3; ++i) {
        language.push_back(data_[i]);
    }

    std::string short_content;
    std::string text;
    bool change = false;
    bool unsync = IsUnsync(header_->GetFlags());

    for (size_t i = 4; i < header_->GetFrameSize(); ++i) {
        if (data_[i] == 0x00) {
            if (unsync && data_[i - 1] == kFalseSyncByte) {
                continue;
            }

            change = true;

            continue;
        }

        if (!change) {
            short_content += data_[i];
        } else {
            text += data_[i];
        }
    }

    stream << "Encoding: " << encoding << "\n";
    stream << "Language: " << language << "\n";
    stream << "Short content description: " << short_content << "\n";
    stream << "Text: " << text << "\n";
    stream.put('\n');
    stream << "--------------\n";
}

RVA2::RVA2(FrameHeader* header, char* data)
    : Frame(header, data)
{}

void RVA2::PrintTo(std::ofstream& stream) const {
    header_->PrintInfo(stream);

    bool unsync = IsUnsync(header_->GetFlags());
    size_t i = 0;

    stream << "Identification: ";

    for (; i < header_->GetFrameSize(); ++i) {
        if (data_[i] == 0x00) {
            if (unsync && i > 0 && data_[i - 1] == kFalseSyncByte) {
                continue;
            }

            ++i;

            break;
        }

        stream.put(data_[i]);
    }

    stream << "\nType of channel: " << kTypeOfChannel[data_[i]] << "\n";

    int16_t volume_adjustment = 0;
    volume_adjustment = (volume_adjustment << 8) | static_cast<uint8_t>(data_[i + 1]);
    volume_adjustment = (volume_adjustment << 8) | static_cast<uint8_t>(data_[i + 2]);

    stream << "Volume adjustment: " << volume_adjustment << "\n";

    uint8_t bits_represent = static_cast<uint8_t>(data_[i + 3]);
    size_t bytes_needed = (bits_represent + 7) / 8;

    i += 4;

    uint32_t peak_volume = 0;

    for (int length = 0; length < bytes_needed; ++length) {
        peak_volume = (peak_volume << 8) | data_[i];
        ++i;
    }

    stream << "Bits representing peak: " << bits_represent << "\n";
    stream << "Peak volume: " << peak_volume << "\n";
    stream.put('\n');
    stream << "--------------\n";
}

EQU2::EQU2(FrameHeader* header, char* data)
    : Frame(header, data)
{}

void EQU2::PrintTo(std::ofstream& stream) const {
    header_->PrintInfo(stream);

    stream << "Interpolation method: ";

    if (data_[0] == 0x00) {
        stream << "Band";
    } else if (data_[0] == 0x01) {
        stream << "Linear";
    } else {
        stream << "Unknown";
    }

    stream << "\nIdentification: ";

    bool unsync = IsUnsync(header_->GetFlags());
    size_t i = 1;

    for (; i < header_->GetFrameSize(); ++i) {
        if (data_[i] == 0x00) {
            if (unsync && data_[i - 1] == kFalseSyncByte) {
                continue;
            }

            ++i;

            break;
        }

        stream.put(data_[i]);
    }

    uint16_t freq = 0;
    int16_t adjustment = 0;

    freq = (freq << 8) | static_cast<uint8_t>(data_[i]);
    freq = (freq << 8) | static_cast<uint8_t>(data_[i + 1]);
    adjustment = (adjustment << 8) | static_cast<uint8_t>(data_[i + 2]);
    adjustment = (adjustment << 8) | static_cast<uint8_t>(data_[i + 3]);

    stream << "Frequency: " << freq << "\n";
    stream << "Volume adjustment: " << adjustment << "\n";
    stream.put('\n');
    stream << "--------------\n";
}

PCNT::PCNT(FrameHeader* header, char* data)
    : Frame(header, data)
{}

void PCNT::PrintTo(std::ofstream& stream) const {
    header_->PrintInfo(stream);

    uint64_t counter = 0;
    bool unsync = IsUnsync(header_->GetFlags());

    for (size_t i = 0; i < header_->GetFrameSize(); ++i) {
        if (data_[i] == 0x00 && unsync && i > 0 && data_[i - 1] == kFalseSyncByte) {
            continue;
        }

        counter = (counter << 8) | static_cast<uint8_t>(data_[i]);
    }

    stream << "Play counter: " << counter << "\n";
    stream.put('\n');
    stream << "--------------\n";
}

POPM::POPM(FrameHeader* header, char* data)
    : Frame(header, data)
{}

void POPM::PrintTo(std::ofstream& stream) const {
    header_->PrintInfo(stream);

    bool unsync = IsUnsync(header_->GetFlags());
    size_t i = 0;

    stream << "Email to user: ";

    for (; i < header_->GetFrameSize(); ++i) {
        if (data_[i] == 0x00) {
            if (unsync && i > 0 && data_[i - 1] == kFalseSyncByte) {
                continue;
            }

            ++i;
            
            break;
        }

        stream.put(data_[i]);
    }

    stream << "\n";

    uint8_t rating = static_cast<uint8_t>(data_[i]);
    uint64_t counter = 0;
    ++i;

    for (; i < header_->GetFrameSize(); ++i) {
        counter = (counter << 8) | static_cast<uint8_t>(data_[i]);
    }

    stream << "Rating: " << rating << "\n";
    stream << "Counter: " << counter << "\n";
    stream.put('\n');
    stream << "--------------\n";
}

RBUF::RBUF(FrameHeader* header, char* data)
    : Frame(header, data)
{}

void RBUF::PrintTo(std::ofstream& stream) const {
    header_->PrintInfo(stream);

    bool unsync = IsUnsync(header_->GetFlags());
    std::vector<uint8_t> good_data;

    if (unsync) {
        for (size_t i = 0; i < header_->GetFrameSize(); ++i) {
            if (data_[i] == 0x00 && i > 0 && data_[i - 1] == kFalseSyncByte) {
                continue;
            }

            good_data.push_back(data_[i]);
        }
    } else {
        for (size_t i = 0; i < header_->GetFrameSize(); ++i) {
            good_data.push_back(data_[i]);
        }
    }

    uint32_t buffer_size = 0;
    uint8_t flag = 0;
    uint32_t offset = 0;

    buffer_size = (buffer_size << 8) | good_data[0];
    buffer_size = (buffer_size << 8) | good_data[1];
    buffer_size = (buffer_size << 8) | good_data[2];
    flag = good_data[3];
    offset = (offset << 8) | good_data[4];
    offset = (offset << 8) | good_data[5];
    offset = (offset << 8) | good_data[6];
    offset = (offset << 8) | good_data[7];

    stream << "Buffer size: " << buffer_size << "\n";
    stream << "Embedded info flag: " << flag << "\n";
    stream << "Offset to next tag: " << offset << "\n";
    stream.put('\n');
    stream << "--------------\n";
}

LINK::LINK(FrameHeader* header, char* data)
    : Frame(header, data)
{}

void LINK::PrintTo(std::ofstream& stream) const {
    header_->PrintInfo(stream);

    bool unsync = IsUnsync(header_->GetFlags());
    std::vector<uint8_t> good_data;

    if (unsync) {
        for (size_t i = 0; i < header_->GetFrameSize(); ++i) {
            if (data_[i] == 0x00 && i > 0 && data_[i - 1] == kFalseSyncByte) {
                continue;
            }

            good_data.push_back(data_[i]);
        }
    } else {
        for (size_t i = 0; i < header_->GetFrameSize(); ++i) {
            good_data.push_back(data_[i]);
        }
    }

    std::string url;
    size_t i = 4;

    for (; i < good_data.size(); ++i) {
        if (good_data[i] == 0x00) {
            ++i;

            break;
        }

        url += good_data[i];
    }

    stream << "Frame identifier: " << good_data[0] << good_data[1] << good_data[2] << good_data[3] << "\n";
    stream << "URL: " << url << "\n";
    stream << "ID and additional data: ";

    for (; i < good_data.size(); ++i) {
        stream.put(good_data[i]);
    }

    stream.put('\n');
    stream << "--------------\n";
}

POSS::POSS(FrameHeader* header, char* data)
    : Frame(header, data)
{}

void POSS::PrintTo(std::ofstream& stream) const {
    header_->PrintInfo(stream);

    bool unsync = IsUnsync(header_->GetFlags());
    std::vector<uint8_t> good_data;

    if (unsync) {
        for (size_t i = 0; i < header_->GetFrameSize(); ++i) {
            if (data_[i] == 0x00 && i > 0 && data_[i - 1] == kFalseSyncByte) {
                continue;
            }

            good_data.push_back(data_[i]);
        }
    } else {
        for (size_t i = 0; i < header_->GetFrameSize(); ++i) {
            good_data.push_back(data_[i]);
        }
    }

    stream << "Time stamp format: ";

    if (good_data[0] == 0x01) {
        stream << "Absolute time, 32 bit sized, using MPEG frames as unit";
    } else if (good_data[0] == 0x02) {
        stream << "Absolute time, 32 bit sized, using milliseconds as unit";
    } else {
        stream << "Unknown";
    }

    stream << "\n";

    uint64_t position = 0;

    for (size_t i = 1; i < good_data.size(); ++i) {
        position = (position << 8) | good_data[i];
    }

    stream << "Position: " << position << "\n";
    stream.put('\n');
    stream << "--------------\n";
}
