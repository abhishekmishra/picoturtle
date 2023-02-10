#include "PicoTurtleImage.hpp"
#include <core/SkData.h>

using namespace turtle;

PicoTurtleImage::PicoTurtleImage(const char *img)
{
    this->image = SkImage::MakeFromEncoded(SkData::MakeFromFileName(img));
}

PicoTurtleImage::~PicoTurtleImage()
{
    free(this->image_path);
}

const sk_sp<SkImage>& PicoTurtleImage::get_image()
{
    return this->image;
}

const char * PicoTurtleImage::get_image_path()
{
    return (const char *)this->image_path;
}