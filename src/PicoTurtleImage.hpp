#pragma once

#include <core/SkImage.h>

namespace turtle
{
    class PicoTurtleImage
    {
        private:
            const char* image_path;
            sk_sp<SkImage> image;

        public:
            PicoTurtleImage(const char* img);
            ~PicoTurtleImage();

            const sk_sp<SkImage> &get_image();
            const char *get_image_path();
    };
}