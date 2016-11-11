/* -----------  Galen Helfter ------------ */
/* -----------    CPSC 4040   ------------ */
/* -----------  Final Project ------------ */
/* ----------- initialize.cpp ------------ */

#include <iostream>

#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>

#include "./headers/initialize.h"

/* initialize_subsystems
 *  Initializes all of the SDL systems needed for the program.
 *
 * Inputs:
 *  - Nothing
 * Returns:
 *  - Bool representing success status
 * */
bool Initialization::initialize_subsystems()
{
    /*  Initialize SDL library  */
    if(SDL_Init(SDL_INIT_TIMER) != 0)
    {
        std::cerr << "Failed to initialize SDL. Error " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

/* close_subsystems
 *  Closes all of the SDL systems needed for the program.
 *
 * Inputs:
 *  - Nothing
 * Returns:
 *  - Nothing
 * */
void Initialization::close_subsystems()
{
    SDL_Quit();
}
