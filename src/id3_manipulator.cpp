#include "../include/id3_manipulator.h"

Manipulator::Manipulator(const std::filesystem::path& file)
    : file_(file)
{}

Manipulator::~Manipulator() {
    delete header_;
}

void Manipulator::ReadTagHeader() {
    std::ifstream stream(file_);

    if (!stream.is_open()) {
        throw std::runtime_error("Cannot open the file.");
    }

    header_ = ParseTagHeader(stream);
}

void Manipulator::Debug() const {
    header_->Output();
}
