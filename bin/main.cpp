#include "../include/id3_manipulator.h"

int main() {
    Manipulator manipulator("D:/Downloads/ps.mp3", "output_info.txt");
    
    manipulator.ReadTag();
    // manipulator.Debug();
    manipulator.PrintFrames();

    return 0;
}
