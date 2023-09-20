#include "../include/argument_parser.h"

#include <cstring>
#include <iostream>
#include <stdexcept>

void ConsoleParser::Parse(int argc, char** argv) {
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        Help();
        return;
    }

    if (argc < 3) {
        throw std::runtime_error("Not enough arguments.");
    }

    std::string input_path;
    std::string output_path;

    if (strncmp(argv[1], "--input", 7) == 0) {
        for (size_t i = 8; argv[1][i] != '\0'; ++i) {
            input_path.push_back(argv[1][i]);
        }
    } else {
        throw std::runtime_error("Wrong arguments. Try --help for more information.");
    }

    if (strncmp(argv[2], "--output", 8) == 0) {
        for (size_t i = 9; argv[2][i] != '\0'; ++i) {
            output_path.push_back(argv[2][i]);
        }
    } else {
        throw std::runtime_error("Wrong arguments. Try --help for more information.");
    }

    input = input_path;
    output = output_path;

    for (size_t i = 3; i < argc; ++i) {
        required_frames.insert(std::string(argv[i]));
    }
}

void ConsoleParser::Help() const {
    std::cout << "First argument is --input. It should be equal to the path to mp3 file. Example: --input=tag.mp3\n";
    std::cout << "Second argument is --output. It should be equal to the name of the output file, where all the information about frames will be stored. Example: --output=frames.txt\n";
    std::cout << "The next arguments are required frames. Provide no frame to get every frame in the file. Example: COMM USLT\n";

    std::cout << "\n";

    std::cout << "Detailed example: --input=ps.mp3 --output=metadata.txt COMM UFID ETCO\n";
    std::cout << "FILE PATHS SHOULD NOT CONTAIN SPACES";
}
