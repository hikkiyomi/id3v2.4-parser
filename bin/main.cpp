#include "../include/id3_manipulator.h"

int main() {
    Manipulator manipulator("D:/Downloads/tag.mp3");
    
    manipulator.ReadTag();
    // manipulator.Debug();

    return 0;
}
