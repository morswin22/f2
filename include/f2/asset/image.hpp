#pragma once
#include <cstddef>

namespace f2 {
  struct image {
    int width, height, channels;
    unsigned char* pixels;

    image(const char* data, size_t size);  
    image(const char* path);
    ~image();
  };
}
