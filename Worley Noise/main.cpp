#include <iostream>

#define STB_IMAGE_IMPLEMENTATION   // use of stb functions once and for all
#include "stb_image.h"

#include "Worley3D.h"

int main() {
    Worley3D<unsigned char> w(128, 128, 128, { {16,16,16}, {32,32,32}, {64,64,64} });


    return 0;
}