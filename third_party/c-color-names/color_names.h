/*

############### c-color-names License

MIT License

Copyright (c) 2022 Abhishek Mishra <https://github.com/abhishekmishra>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

############### color-names License

MIT License

Copyright (c) 2017 David Aerne

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

/**
 * \file color_names.h
 * \brief c-color-names main header file. Defines the API of c-color-names.
 */

#ifndef __COLOR_NAMES_H__
#define __COLOR_NAMES_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * check if the name exists in the color list. color name should
 * be lowercase, other wise match might fail.
 *
 * @param color_name name of the color to look for (lowercase chars only)
 * @return int 0 or 1 indicating whether color name found
 */
int color_name_exists(const char *color_name);

/**
 * find the hex string for the given color name if found
 *
 * @param color_name name of the color to look for (lowercase chars only)
 * @param color_hexstr string of length 8. it will contain the hexstring
 *         in format #xxxxxx if color name found, unchanged otherwise.
 * @return int 0 or 1 indicating whether color name found
 */
int color_name_get_rgb_string(const char *color_name, char *color_hex_str);

/**
 * find the rgb values for the given color name if found
 *
 * @param color_name name of the color to look for (lowercase chars only)
 * @param r red value
 * @param g green value
 * @param b blue value
 * @return int 0 or 1 indicating whether color name found.
 */
int color_name_get_rgb(const char *color_name, uint8_t *r, uint8_t *g, 
    uint8_t *b);

/**
 * find the color name for the given r,g,b triplet.
 * returns NULL if color is not found.
 *
 * @param r red value
 * @param g green value
 * @param b blue value
 * @return color name (NULL if not found)
 */
const char* color_name_find_rgb(uint8_t r, uint8_t g, uint8_t b);

/**
 * find the hsv values for the given color name if found
 *
 * @param color_name name of the color to look for (lowercase chars only)
 * @param h hue
 * @param s saturation
 * @param v value
 * @return int 0 or 1 indicating whether color name found.
 */
int color_name_get_hsv(const char *color_name, float *h, float *s, 
    float *v);

/**
 * find the color name for the given h,s,v triplet.
 * returns NULL if color is not found.
 *
 * @param h hue
 * @param s saturation
 * @param v value
 * @return color name (NULL if not found)
 */
const char* color_name_find_hsv(float h, float s, float v);

/**
 * find the color name for the given color hex string in format #rrggbb.
 * returns NULL if color is not found.
 *
 * @param hex hex string (format #rrggbb) of the color to be looked up. 
 *      must be exactly 7 chars in length.
 * @return color name (NULL if not found)
 */
const char* color_name_find_hex(const char* hex);

#ifdef __cplusplus
}
#endif

#endif //__COLOR_NAMES_H__