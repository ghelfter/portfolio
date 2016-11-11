/* ------------ Galen Helfter ------------ */
/* ------------   CPSC 4040   ------------ */
/* ------------ Final Project ------------ */
/* ------------ img_convert.h ------------ */

#ifndef _IMG_CONVERT_H_
#define _IMG_CONVERT_H_

#include <cstdint>

#include <SDL2/SDL.h>

#include "g_float.h"

/*  Be able to create a new SDL surface from my image format after
 *  reading in from OpenImageIO
 */
namespace Initialization
{
    /*  Static constants  */

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const uint32_t red_mask   = 0xff000000;
    const uint32_t green_mask = 0x00ff0000;
    const uint32_t blue_mask  = 0x0000ff00;
    const uint32_t alpha_mask = 0x000000ff;

    const uint8_t red_shift   =  0;
    const uint8_t green_shift =  8;
    const uint8_t blue_shift  = 16;
    const uint8_t alpha_shift = 24;

    const uint32_t r_red_mask   = 0x000000ff;
    const uint32_t r_green_mask = 0x0000ff00;
    const uint32_t r_blue_mask  = 0x00ff0000;
    const uint32_t r_alpha_mask = 0xff000000;

    const uint8_t r_red_shift   = 24;
    const uint8_t r_green_shift = 16;
    const uint8_t r_blue_shift  =  8;
    const uint8_t r_alpha_shift =  0;
#else
    const uint32_t red_mask   = 0x000000ff;
    const uint32_t green_mask = 0x0000ff00;
    const uint32_t blue_mask  = 0x00ff0000;
    const uint32_t alpha_mask = 0xff000000;

    const uint8_t red_shift   = 24;
    const uint8_t green_shift = 16;
    const uint8_t blue_shift  =  8;
    const uint8_t alpha_shift =  0;

    const uint32_t r_red_mask   = 0xff000000;
    const uint32_t r_green_mask = 0x00ff0000;
    const uint32_t r_blue_mask  = 0x0000ff00;
    const uint32_t r_alpha_mask = 0x000000ff;

    const uint8_t r_red_shift   =  0;
    const uint8_t r_green_shift =  8;
    const uint8_t r_blue_shift  = 16;
    const uint8_t r_alpha_shift   = 24;
#endif

    const uint8_t red_loss   = 0;
    const uint8_t blue_loss  = 0;
    const uint8_t green_loss = 0;
    const uint8_t alpha_loss = 0;

    const uint32_t clear_mask = 0x000000ff;
    const uint32_t default_flags = 0;
    const int depth = 32;

    SDL_Surface* create_surface_image(g_image::F_image &img);
    void create_image_surface(g_image::F_image &img, SDL_Surface &surf);
}

#endif
