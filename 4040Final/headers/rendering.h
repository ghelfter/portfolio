/* -----------  Galen Helfter ------------ */
/* -----------    CPSC 4040   ------------ */
/* -----------  Final Project ------------ */
/* -----------   rendering.h  ------------ */

#ifndef _RENDERING_H_
#define _RENDERING_H_

#include <cstdint>

#include <SDL2/SDL.h>

#include "g_float.h"
#include "ghelfter_vector.h"

namespace Cagifier
{
    enum State
    {
        point_selection,
        first_point,
        second_point,
        calculating
    };

    const unsigned int total_lines = 12;
    const unsigned int steps = 10;
    const char cycle_next = 'f';
        
    void blit_dot(SDL_Surface *img, SDL_Surface *dot, Matrix::Vector3 &point);
    bool main_loop(g_image::F_image *image, g_image::F_image *cage,
                   SDL_Window *main_window, SDL_Surface *screen,
                   const std::string &output, g_image::F_image *dots,
                   const std::string &out_pts,
                   uint32_t flags, const std::string &in_pts,
                   const std::string &c_in_pts);
}

#endif
