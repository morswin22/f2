#include <f2/asset/image.hpp>
#include <stb_image.h>

f2::image::image(const char* data, size_t size)
{
  pixels = stbi_load_from_memory(
    (const unsigned char*)data, size,
    &width, &height, &channels, STBI_default);
}

f2::image::image(const char* path)
  : width(0), height(0), channels(0), pixels(nullptr)
{
  pixels = stbi_load(path,
     &width, &height, &channels, STBI_default);
}

f2::image::~image() {
  stbi_image_free(pixels);
}
