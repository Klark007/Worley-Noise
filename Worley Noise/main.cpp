#include <iostream>

#define STB_IMAGE_IMPLEMENTATION   // use of stb functions once and for all
#include "stb_image.h"

#include "Worley.h"

int main() {
    Worley<unsigned char> w(1024,1024,16,16);

    return 0;
}