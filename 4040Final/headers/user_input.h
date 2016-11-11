/* ------------ Galen Helfter ------------ */
/* ------------   CPSC 4040   ------------ */
/* ------------ Final Project ------------ */
/* ------------  user_input.h ------------ */

/*  Process user input, such as mouse events and keyboard events  */

#ifndef _USER_INPUT_H_
#define _USER_INPUT_H_

#include <SDL2/SDL.h>

#include "rendering.h"
#include "warpline.h"

namespace Event
{
    void handle_mouse(SDL_Event &e, Cagifier::State &state,
                      unsigned int &lines, Cagifier::Warpline *warplines);
    void handle_keyboard(SDL_Event &e, Cagifier::State &state, bool &running,
                         unsigned int &current);
}

#endif
