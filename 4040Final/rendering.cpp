/* -----------  Galen Helfter ------------ */
/* -----------    CPSC 4040   ------------ */
/* -----------  Final Project ------------ */
/* -----------  rendering.cpp ------------ */

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include "./headers/config.h"
#include "./headers/rendering.h"
#include "./headers/user_input.h"
#include "./headers/img_convert.h"
#include "./headers/warping.h"

/* blit_dot
 * Blits a given dot onto the given image.
 *
 * Inputs:
 *  - img - Image to place the dot on
 *  - dot - Image representing the dot
 *  - point - Vector representing where the user clicked
 * Returns:
 *  - Nothing
 * */
void Cagifier::blit_dot(SDL_Surface *img, SDL_Surface *dot,
                        Matrix::Vector3 &point)
{
    SDL_Rect box;
    box.x = (int) point[0] - (Cagifier::dot_dim / 2);
    box.y = (int) point[1] - (Cagifier::dot_dim / 2);
    box.w = Cagifier::dot_dim;
    box.h = Cagifier::dot_dim;

    SDL_BlitSurface(dot, 0, img, &box);
}

/* main_loop
 * Main loop of the program, written in a similar way to a main application
 * loop or a main game loop. Does all of the heavy lifting, including processing
 * user input and calling function for calculations.
 *
 * Inputs:
 *  - Image - Image given by the user
 *  - Cage - Image of Nicholas Cage
 * */
bool Cagifier::main_loop(g_image::F_image *image, g_image::F_image *cage,
                         SDL_Window *main_window, SDL_Surface *screen,
                         const std::string &output, g_image::F_image *dots,
                         const std::string &out_pts,
                         uint32_t flags, const std::string &in_pts,
                         const std::string &c_in_pts)
{
    /*  Working image copy  */
    std::unique_ptr<g_image::F_image> working(new g_image::F_image(*image));
    std::unique_ptr<g_image::F_image[]> workings(new g_image::F_image[steps]);
    for(unsigned int i = 0; i < steps; ++i)
    {
        workings[i].replicate(*image);
    }

    bool running = true;                   /*  Keep running state  */
    bool rendered = false;
    Cagifier::State state = first_point;   /*  Keep current state  */
    SDL_Event event;
    unsigned int lines = 0;
    unsigned int current = 0;
    image->print(std::cout);

    /*  Make an array to represent our dots  */
    SDL_Surface *dot_surfaces[Cagifier::dot_count];
    for(unsigned int i = 0; i < Cagifier::dot_count; ++i)
    {
        dot_surfaces[i] = Initialization::create_surface_image(dots[i]);
    }

    /*  Make an array representing our lines  */
    Cagifier::Warpline warplines[Cagifier::total_lines];
    Cagifier::Warpline c_warplines[Cagifier::total_lines];

    SDL_Surface *img = Initialization::create_surface_image(*image);
    SDL_Surface *temp = Initialization::create_surface_image(*image);
    SDL_Surface *result = 0;
    SDL_Surface* results[steps];
    for(unsigned int i = 0; i < steps; ++i)
        results[i] = 0;

    /*  If we're reading in the points, need to not allow them to click  */
    /*  This is for reading in the points for their image  */
    if(flags & READ_POINTS)
    {
        state = calculating;
        std::ifstream pts(in_pts, std::ifstream::in);
        if(!pts.is_open())
        {
            std::cerr << "Failed to open points file " << in_pts << ". Exiting...\n";
            std::exit(1);
        }
        for(unsigned int i = 0; i < Cagifier::total_lines; ++i)
        {
            warplines[i].read(pts);
        }
        pts.close();
    }

    /*  Read in the predefined Cage image lines  */
    if(!(flags & OUT_ONLY))
    {
        std::ifstream pts(c_in_pts, std::ifstream::in);
        if(!pts.is_open())
        {
            std::cerr << "Failed to open points file " << c_in_pts << ". Exiting...\n";
            std::exit(1);
        }
        for(unsigned int i = 0; i < Cagifier::total_lines; ++i)
        {
            c_warplines[i].read(pts);
        }
        pts.close();
    }

    /*  Main application loop  */
    while(running)
    {
        /*  Poll for events  */
        while(SDL_PollEvent(&event) != 0)
        {
            if(event.type == SDL_QUIT)
            {
                running = false;
            }

            /*  Handle keyboard input  */
            else if(event.type == SDL_KEYDOWN)
            {
                Event::handle_keyboard(event, state, running, current);
            }

            /*  Handle mouse input  */
            else if(event.type == SDL_MOUSEBUTTONUP)
            {
                Event::handle_mouse(event, state, lines, warplines);
            }

            /*  Stop processing input if we've stopped running  */
            else if(!running)
                break;

            /*  Otherwise, continue and skip event  */
            else
                continue;
        }

        SDL_BlitSurface(img, 0, temp, 0);
        if(state == calculating)
        {

            /*  If we are only doing output, we need to now print out our 
             *  different warplines so they can be used for later  */
            if(flags & OUT_ONLY)
            {
                std::ofstream ofile(out_pts);
                for(unsigned int i = 0; i < Cagifier::total_lines; ++i)
                {
                    warplines[i].output(ofile);
                }
                ofile.close();
                running = false;
            }

            /*  Do the calculation  */
            else
            {
                if(!rendered)
                {
                    rendered = true;
                    /*  Render through the different images. Print out to file.  */
                    /*
                    std::cout << "Boop boop calculating.\n";
                    Cagifier::warp_image(*image, *cage, *working, c_warplines, warplines,
                                         Cagifier::total_lines);
                    result = Initialization::create_surface_image(*working);
                    */
                    for(unsigned int j = 0; j < Cagifier::steps; ++j)
                    {
                        double increment = 1.0 / (double) Cagifier::steps;
                        increment *= (j+1);
                        std::cout << "Warping at time " << increment << std::endl;
                        Cagifier::warp_at_time(*image, *cage, workings[j], c_warplines,
                                               warplines, Cagifier::total_lines,
                                               increment);
                        std::cout << "Finished warping at time " << increment << std::endl;
                        results[j] = Initialization::create_surface_image(workings[j]);
                        /*  Output to file  */
                        std::stringstream outs;
                        outs << img_dir << "/" << out_pre << j << out_ext;
                        workings[j].write(outs.str());
                    }
                    current = steps / 2;
                }
                
                SDL_BlitSurface(results[current], 0, temp, 0);
            }
            //std::cout << "Finished accepting points.\n";

        }
        else if(state == first_point || state == second_point)
        {
            /*  Apply dots onto the screen  */
            for(unsigned int i = 0; i < lines + 1; ++i)
            {
                if((i >= lines && state != first_point) || i < lines)
                {
                    blit_dot(temp, dot_surfaces[i], warplines[i][0]);
                }
                if(i < lines)
                {
                    blit_dot(temp, dot_surfaces[i], warplines[i][1]);
                }
            }
        }
        SDL_BlitSurface(temp, 0, screen, 0);

        SDL_UpdateWindowSurface(main_window);

        /*  Delay to cut down on some CPU usage  */
        SDL_Delay(50);
    }
    /*  Output the points if designated  */
    if(flags & OUTPUT_POINTS && !(flags & OUT_ONLY))
    {
        std::ofstream ofile(out_pts);
        for(unsigned int i = 0; i < Cagifier::total_lines; ++i)
        {
            warplines[i].output(ofile);
        }

        ofile.close();
    }

    for(unsigned int i = 0; i < Cagifier::total_lines; ++i)
    {
        std::cout << "Warpline " << i << ":\n";
        std::cout << warplines[i][0];
        std::cout << warplines[i][1];
        std::cout << std::endl;
    }
    if(img != 0)
        SDL_FreeSurface(img);
    if(temp != 0)
        SDL_FreeSurface(temp);
    if(result != 0)
        SDL_FreeSurface(result);
    for(unsigned int i = 0; i < steps; ++i)
    {
        if(results[i] != 0)
            SDL_FreeSurface(results[i]);
    }

    std::unique_ptr<g_image::F_image> out(new g_image::F_image(*image));
    Initialization::create_image_surface(*out, *img);
    out->write(output);


    return true;
}
