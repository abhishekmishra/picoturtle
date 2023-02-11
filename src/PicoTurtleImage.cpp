#include "PicoTurtleImage.hpp"
#include <core/SkData.h>

using namespace turtle;

PicoTurtleImage::PicoTurtleImage(const char *img)
{
    this->image = SkImage::MakeFromEncoded(SkData::MakeFromFileName(img));
}

PicoTurtleImage::~PicoTurtleImage()
{
    // nothing to delete for now as image_path is a const char* owned by
    // caller, and image is a member variable which will be deleted with
    // the object
}

const sk_sp<SkImage>& PicoTurtleImage::get_image()
{
    return this->image;
}

const char * PicoTurtleImage::get_image_path()
{
    return (const char *)this->image_path;
}