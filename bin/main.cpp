#include "../include/id3_manipulator.h"

int main() {
    Manipulator manipulator("D:/Downloads/Hamidreza Ghorbani - Shay Watson And Hamidreza Ghorbani - After The Last Tear Falls.mp3");
    
    manipulator.ReadTagHeader();
    manipulator.Debug();

    return 0;
}
