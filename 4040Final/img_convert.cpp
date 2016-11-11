/* -----------  Galen Helfter  ----------- */
/* -----------    CPSC 4040    ----------- */
/* -----------  Final Project  ----------- */
/* ----------- img_convert.cpp ----------- */

#include <iostream>
#include <iomanip>

#include "./headers/img_convert.h"

/* SDL_Surface_get
 *  Function to interface with an SDL_Surface struct. Acquires the value
 *  of the pixel given the channel at the given location.
 * Inputs:
 *  surf - Surface to get data from
 *  channel - Channel we want data from
 *  location - Location to acquire the pixel data from
 * Returns:
 *  - Unsigned integer representing the value of the pixel given the channel
 */
uint32_t SDL_Surface_get(const SDL_Surface &surf, unsigned int channel,
                         unsigned int location)
{
    uint32_t pixel = *(((uint32_t*) surf.pixels) + location);
    uint32_t result = 0;
    switch(channel)
    {
      case 0: /*  Red channel    */
        result = (pixel & Initialization::red_mask) >> Initialization::r_red_shift;
        break;
      case 1: /*  Green channel  */
        result = (pixel & Initialization::green_mask) >> Initialization::r_green_shift;
        break;
      case 2: /*  Blue channel   */
        result = (pixel & Initialization::blue_mask) >> Initialization::r_blue_shift;
        break;
      case 3: /*  Alpha channel  */
        result = (pixel & Initialization::alpha_mask) >> Initialization::r_alpha_shift;
        break;
      default:
        result = pixel;
    }

    return result;
}

/* SDL_Surface_put
 *  Function to place a pixel into the struct at a given channel on an SDL_Surface
 *  structure.
 * Inputs:
 *  surf - Surface to place data in
 *  value - value to place in the Surface
 *  channel - Channel to place value in
 *  location - location to place value at
 * Returns:
 *  - Nothing
 */
void SDL_Surface_put(SDL_Surface &surf, uint32_t value, unsigned int channel,
                     unsigned int location)
{
    uint32_t *pixel = ((uint32_t*) surf.pixels + location);
    uint32_t result = *pixel;

    switch(channel)
    {
      case 0: /*  Red channel    */
        result = (result & ~Initialization::red_mask) | ((value &
                 Initialization::clear_mask) << Initialization::r_red_shift);
        break;
      case 1: /*  Green channel  */
        result = (result & ~Initialization::green_mask) | ((value &
                 Initialization::clear_mask) << Initialization::r_green_shift);
        break;
      case 2: /*  Blue channel   */
        result = (result & ~Initialization::blue_mask) | ((value &
                 Initialization::clear_mask) << Initialization::r_blue_shift);
        break;
      case 3: /*  Alpha channel  */
        result = (result & ~Initialization::alpha_mask) | ((value &
                 Initialization::clear_mask) << Initialization::r_alpha_shift);
        break;
      default:
        result = 0;
    }
    //std::cout << std::hex << result << std::dec << std::endl;

    *pixel = result;
}

/* create_surface_image
 *  Using an F_image class, constructs an SDL_Surface based on the data
 *  stored inside the class. Assumes that the class should probably be
 *  a 4 channel instance, as the SDL_Surface struct uses RGBA normally.
 * Inputs:
 *  img - Image to get the data from
 * Returns:
 *  - SDL_Surface with the data stored inside it
 * */
SDL_Surface* Initialization::create_surface_image(g_image::F_image &img)
{
    /*  Need to set up SDL Surface  */
    SDL_Surface *result = NULL;
    //SDL_PixelFormat *format = NULL;
    //SDL_Color *color = NULL;
    uint32_t i_pix = 0;

    unsigned int w = img.get_x_dim(), h = img.get_y_dim();

    result = SDL_CreateRGBSurface(Initialization::default_flags, w, h,
                                  Initialization::depth, Initialization::red_mask,
                                  Initialization::green_mask, Initialization::blue_mask,
                                  Initialization::alpha_mask);

    /*  Fill out the image  */
    for(unsigned int y = 0; y < h; ++y)
    {
        for(unsigned int x = 0; x < w; ++x)
        {
            for(unsigned int channel = 0; channel < img.get_channels(); ++channel)
            {
                i_pix = (uint32_t) (255.0 * img.get(x, y, channel));
                //std::cout << "Pixel value: " << i_pix << std::endl;
                SDL_Surface_put(*result, i_pix, channel, x + y*w); 
            }
        }
    }

    return result;
}

/* create_image_surface
 *  Using an SDL_Surface, extracts the information and places it into an F_image
 *  class.
 * Inputs:
 *  img - Image to place the data in
 *  surf - SDL_Surface to get the data from
 * Returns:
 *  - Nothing
 * */
void Initialization::create_image_surface(g_image::F_image &img, SDL_Surface &surf)
{
    unsigned int w = img.get_x_dim(), h = img.get_y_dim();
    for(unsigned int y = 0; y < h; ++y)
    {
        for(unsigned int x = 0; x < w; ++x)
        {
            for(unsigned int channel = 0; channel < img.get_channels(); ++channel)
            {
                uint32_t val = SDL_Surface_get(surf, channel, x + y*w);
                float f_val = (float) val / 255.0;
                img.set(x, y, channel, f_val);
            }
        }
    }
}
