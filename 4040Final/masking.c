/* ===============  Galen Helfter =============== */
/* ===============    CPSC 4040   =============== */
/* ===============  Final Project =============== */
/* ===============    masking.c   =============== */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "./headers/masking.h"

/* to_given_channels
 *   Takes a buffer with a given amount of channels and spaces it out
 *   to have a new amount of channels.
 * Inputs:
 *   out_buffer - Buffer to store new image information
 *   in_buffer - Current buffer storing the image information
 *   new_channels - Amount of channels to convert to
 *   current_channels - Amount of channels to convert from
 *   new_size - Size of the out_buffer
 *   old_size - Size of the in_buffer
 * Returns:
 *   Nothing
 */
void to_given_channels(uint8_t *out_buffer, const uint8_t *in_buffer,
                       uint32_t new_channels, uint32_t current_channels,
                       uint32_t new_size, uint32_t old_size)
{
    uint32_t old_i = 0, new_i = 0;
    uint32_t max_copy = current_channels < new_channels ? current_channels :
                        new_channels;
    for(old_i = 0; old_i < old_size && new_i < new_size;
        old_i += current_channels)
    {
        memcpy((out_buffer + new_i), (in_buffer + old_i), max_copy);
        new_i += new_channels;
    }
}

/* to_given_channels_f
 *   Takes a buffer with a given amount of channels and spaces it out
 *   to have a new amount of channels. Uses floating point numbers.
 * Inputs:
 *   out_buffer - Buffer to store new image information
 *   in_buffer - Current buffer storing the image information
 *   new_channels - Amount of channels to convert to
 *   current_channels - Amount of channels to convert from
 *   new_size - Size of the out_buffer
 *   old_size - Size of the in_buffer
 * Returns:
 *   Nothing
 */
void to_given_channels_f(float *out_buffer, const float *in_buffer,
                         uint32_t new_channels, uint32_t current_channels,
                         uint32_t new_size, uint32_t old_size)
{
    uint32_t old_i = 0, new_i = 0;
    uint32_t max_copy = current_channels < new_channels ? current_channels :
                        new_channels;
    for(old_i = 0; old_i < old_size && new_i < new_size;
        old_i += current_channels)
    {
        memcpy((out_buffer + new_i), (in_buffer + old_i), max_copy*sizeof(float));
        new_i += new_channels;
    }
}

/* binary_mask
 *   Creates a binary alpha mask of the given image. Can be told to invert
 *   the mask with the invert boolean flag.
 * Inputs:
 *   buffer - Buffer storing the image data
 *   buffer_len - Length of the buffer
 *   hue - Hue value to be masked
 *   sat - Saturation value to be masked, does nothing here
 *   val - Value to be masked, does nothing here
 *   h_eps - Epsilon value for the hue
 *   s_eps - Epsilon value for the saturation
 *   v_eps - Epsilon value for the value
 *   invert - Whether or not to invert the mask
 * Returns:
 *   Nothing
 */
void binary_mask(uint8_t *buffer, uint32_t buffer_len, double hue, double sat,
                 double val, double h_eps, double s_eps, double v_eps, bool invert)
{
    double H = 0.0, S = 0.0, V = 0.0;
    uint8_t red = 0, green = 0, blue = 0;

    /*  Might just wanna make it crash if they screw up badly, but whatever.  */
    if(buffer == NULL)
    {
        return;
    }

    for(int i = 0; i < buffer_len; i += FINAL_CHANNELS)
    {
        red = *(buffer + i + R_LOC);
        green = *(buffer + i + G_LOC);
        blue = *(buffer + i + B_LOC);

        /*  Must calculate HSV version of our pixel  */
        convert_rgb_hsv(&H, &S, &V, red, green, blue);

        /*  Now, compare it to our configurated epsilon and color value  */
        if(H > (hue - h_eps) && H < (hue + h_eps))
        {
            if(invert)
                *(buffer + i + (FINAL_CHANNELS - 1)) = BINARY_OPAQUE;
            else
                *(buffer + i + (FINAL_CHANNELS - 1)) = BINARY_TRANSPARENT;
        }
        else
        {
            if(invert)
                *(buffer + i + (FINAL_CHANNELS - 1)) = BINARY_TRANSPARENT;
            else
                *(buffer + i + (FINAL_CHANNELS - 1)) = BINARY_OPAQUE;
        }
    }
}

/* fancy_mask
 *   Creates an alpha mask of the image based on the hue, saturation, and value
 *   of each pixel.
 * Inputs:
 *   buffer - Buffer storing the image data
 *   buffer_len - Length of the buffer
 *   hue - Hue value to be masked
 *   sat - Saturation value to be masked
 *   val - Value to be masked
 *   h_eps - Epsilon value for the hue
 *   s_eps - Epsilon value for the saturation
 *   v_eps - Epsilon value for the value
 *   invert - Whether or not to invert the mask, Not Supported for this function
 * Returns:
 *   Nothing
 */
void fancy_mask(uint8_t *buffer, uint32_t buffer_len, double hue, double sat,
                double val, double h_eps, double s_eps, double v_eps, bool invert)
{
    double H = 0.0, S = 0.0, V = 0.0;
    uint8_t red = 0, green = 0, blue = 0;

    /*  Might just wanna make it crash if they screw up badly, but whatever.  */
    if(buffer == NULL)
    {
        return;
    }

    for(int i = 0; i < buffer_len; i += FINAL_CHANNELS)
    {
        red = *(buffer + i + R_LOC);
        green = *(buffer + i + G_LOC);
        blue = *(buffer + i + B_LOC);

        /*  Must calculate HSV version of our pixel  */
        convert_rgb_hsv(&H, &S, &V, red, green, blue);
        if(H > (hue - h_eps) && H < (hue + h_eps))
        {
            if(S > (sat - s_eps) && S < (sat + s_eps))
            {
                if(V > (val - v_eps) && V < (val + v_eps))
                {
                    uint8_t alpha = (uint8_t) ((V/(2*v_eps)) + ((val - v_eps)
                                    / (2*v_eps)) * 255.0);
                    *(buffer + i + (FINAL_CHANNELS - 1)) = alpha;
                }
                else if(V < (val - v_eps))
                    *(buffer + i + (FINAL_CHANNELS - 1)) = 0;
            }
        }
        else
            *(buffer + i + (FINAL_CHANNELS - 1)) = 0xff;
    }
}

/* new_channel_size
 *   Calculates the new size needed for an image going from c_channels to
 *   n_channels.
 * inputs:
 *   n_channels - Desired channels to be encoded in the image
 *   c_channels - Current channels encoded in the image
 *   len - Current total size of the image
 * returns:
 *   (len / c_channels) * n_channels - New size for an image
 */
uint32_t new_channel_size(uint32_t n_channels, uint32_t c_channels, uint32_t len)
{
    return ((len / c_channels) * n_channels);
}

/* petro_vlahos
 *   Stub for using the petro vlahos algorithm. Not implemented yet.
 * Inputs:
 *   buffer - Buffer storing the image data
 *   buffer_len - Length of the buffer
 *   hue - Hue value to be masked
 *   sat - Saturation value to be masked
 *   val - Value to be masked
 *   h_eps - Epsilon value for the hue
 *   s_eps - Epsilon value for the saturation
 *   v_eps - Epsilon value for the value
 *   invert - Whether or not to invert the mask, Not Supported for this function
 * Returns:
 *   Nothing
 */
void petro_vlahos(uint8_t *buffer, uint32_t buffer_len, double hue, double sat,
                  double val, double h_eps, double s_eps, double v_eps, bool invert)
{
    /*  TODO  */
    return;
}

/* no_spill
 *   Default function for the spill suppression function pointer. Does nothing.
 * inputs:
 *   buffer - Buffer storing the image.
 *   buffer_len - Length of the incoming buffer.
 * returns:
 *   nothing
 */
void no_spill(uint8_t *buffer, uint32_t buffer_len)
{
    return;
}

/* spill_suppression
 *   Not yet implemented.
 * inputs:
 *   buffer - Buffer storing the image.
 *   buffer_len - Length of the incoming buffer.
 * returns:
 *   nothing
 */
void spill_suppression(uint8_t *buffer, uint32_t buffer_len)
{
    /*  TODO  */
    return;
}

/* convert_rgb_hsv
 *   Converts a given set of RGB values to the HSV color space and stores them in
 *   the given H, S, and V double pointers.
 * inputs:
 *   H - Pointer to a double in which to store the calculated Hue
 *   S - Pointer to a double in which to store the calculated Saturation
 *   V - Pointer to a double in which to store the calculated Value
 *   R - Integer representation of the red value of the pixel
 *   G - Integer representation of the green value of the pixel
 *   B - Integer representation of the blue value of the pixel
 * returns:
 *   nothing
 */
#define MAXIMUM(x,y,z) ((x) > (y)? ((x) > (z)? (x) : (z)) : ((y) > (z)? (y) : (z)))
#define MINIMUM(x,y,z) ((x) < (y)? ((x) < (z)? (x) : (z)) : ((y) < (z)? (y) : (z)))

void convert_rgb_hsv(double *H, double *S, double *V, uint8_t R, uint8_t G, uint8_t B)
{
    double red = (double) R / 255.0;
    double green = (double) G / 255.0;
    double blue = (double) B / 255.0;
    double max, min, delta;

    max = MAXIMUM(red,green,blue);
    min = MINIMUM(red,green,blue);
    *V = max;
    if(max == 0)
    {
        *S = 0.0;
        *H = 0.0;
    }
    else
    {
        *S = (max - min) / max;
        delta = max - min;
        if(delta == 0.0)
            *H = 0.0;
        else
        {
            if(red == max)
                *H = (green - blue) / delta;
            else if(green == max)
                *H = 2.0 + (blue - red) / delta;
            else // blue == max
                *H = 4.0 + (red - green) / delta;
        }
        *H = *H * 60.0;
        if(*H < 0.0)
            *H = *H + 360.0;
    }
}
