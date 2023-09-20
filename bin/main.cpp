#include "../include/argument_parser.h"
#include "../include/id3_manipulator.h"

int main(int argc, char** argv) {
    ConsoleParser parser;
    parser.Parse(argc, argv);

    Manipulator manipulator(parser.input, parser.output, parser.required_frames);
    
    manipulator.ReadTag();
    manipulator.PrintFrames();

    return 0;
}
