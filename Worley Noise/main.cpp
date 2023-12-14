#include <iostream>

#define STB_IMAGE_IMPLEMENTATION   // use of stb functions once and for all
#include "stb_image.h"

#include "Worley3D.h"
#include "Worley.h"

int main() {
    //Worley3D<unsigned char> w(128, 128, 128, { {16,16,16}, {32,32,32}, {64,64,64} });
    Worley<unsigned char> w(128, 128, { {8, 8}, {16,16}, {7,7} });
    
    std::vector<unsigned char> b = w.get_channel(2);
    //w.set_channel(0, b);
    w.set_channel(1, b);

    w.print();

    return 0;
}