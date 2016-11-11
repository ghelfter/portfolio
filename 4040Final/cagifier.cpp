/* ------------ Galen Helfter ------------ */
/* ------------   CPSC 4040   ------------ */
/* ------------ Final Project ------------ */
/* ------------  cagifier.cpp ------------ */

#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <SDL2/SDL.h>

#include "./headers/config.h"
#include "./headers/initialize.h"
#include "./headers/g_float.h"
#include "./headers/rendering.h"
#include "./headers/masking.h"

/*  Main function  */
int main(int argc, char **argv)
{
    bool result = false;
    unsigned int w = DEFAULT_WIDTH, h = DEFAULT_HEIGHT;
    uint32_t flags = 0x00;

    std::string cage_fname = DEFAULT_CAGE;
    std::string input_fname = DEFAULT_INPUT;
    std::string output_fname = DEFAULT_OUTPUT;
    std::string output_pts = DEFAULT_PTS_FNAME;
    std::string c_in_pts = DEFAULT_C_PTS;
    std::string in_pts;
    std::string pfile = PROMPT_FNAME;

    SDL_Window *main_window = 0;
    SDL_Surface *screen = 0;

    /*  Check minimum command line arguments  */
    /*
    if(argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " [input image] {output image}\n";
        std::exit(1);
    }
    */

    /*  Process command line arguments  */
    for(int i = 1; i < argc; ++i)
    {
        /*  Specify new Nicholas Cage file  */
        if(!std::strncmp(argv[i], FLAG_CAGE_FILE, FLAG_CAGE_FILE_LEN))
        {
            if(argc == i + 1)
            {
                std::cerr << "Must specify new Cage image.\n";
                std::exit(1);
            }

            cage_fname = argv[i+1];
            i += 1;
        }
        else if(!std::strncmp(argv[i], FLAG_OUT_FILE, FLAG_OUT_FILE_LEN))
        {
            if(argc == i + 1)
            {
                std::cerr << "Must specify output image file.\n";
                std::exit(1);
            }
            std::cerr << "New output image file is " << argv[i+1] << std::endl;

            output_fname = argv[i+1];
            i += 1;
        }
        else if(!std::strncmp(argv[i], FLAG_OUTPUT_POINTS, FLAG_OUTPUT_POINTS_LEN))
        {
            if(argc == i + 1)
            {
                std::cerr << "Must specify output points file.\n";
                std::exit(1);
            }
            std::cerr << "Outputting points to " << argv[i+1] << std::endl;

            output_pts = argv[i+1];
            flags |= OUTPUT_POINTS;
            i += 1;
        }
        /*  If we are set to only work for the first one, we need to know. Mainly
         *  used for setting up the program initially.
         */
        else if(!std::strncmp(argv[i], FLAG_OUTPUT_ONLY, FLAG_OUTPUT_ONLY_LEN))
        {
            flags |= OUT_ONLY;
            std::cout << "Only outputting points file.\n";
        }
        else if(!std::strncmp(argv[i], FLAG_OO_FILE, FLAG_OO_FILE_LEN))
        {
            flags |= OUT_ONLY;
            if(argc == i + 1)
            {
                std::cerr << "Must specify new points file.\n";
                std::exit(1);
            }

            output_pts = argv[i+1];
            std::cout << "Only outputting points file " << output_pts << std::endl;
            i += 1;
        }
        else if(!std::strncmp(argv[i], FLAG_INPUT_POINTS, FLAG_INPUT_POINTS_LEN))
        {
            flags |= READ_POINTS;
            if(argc == i + 1)
            {
                std::cerr << "Must specify input points file.\n";
                std::exit(1);
            }

            in_pts = argv[i+1];
            std::cout << "Using points file " << in_pts << std::endl;
            i += 1;
        }
        /*  Otherwise, we have our new input image  */
        else if(!std::strncmp(argv[i], FLAG_SPECIFY_INPUT, FLAG_SPECIFY_INPUT_LEN))
        {
            if(argc == i + 1)
            {
                std::cerr << "Must specify input image.\n";
                std::exit(1);
            }

            input_fname = argv[i+1];
            i += 1;
        }
    }

    /*  Display prompt  */
    char buff[DEFAULT_LBUFF];
    std::ifstream prompt(pfile, std::ifstream::in);
    if(!prompt.is_open())
    {
        std::cerr << "Failed to open prompt.\n";
        std::exit(1);
    }
    while(prompt.getline(buff, DEFAULT_LBUFF))
    {
        std::cout << buff << std::endl;
    }
    prompt.close();

    /*  Initialize SDL subsystems  */
    result = Initialization::initialize_subsystems();
    if(result == false)
    {
        std::cerr << "Failed to initialize. Exiting...\n";
        std::exit(1);
    }

    /*  Read in all of the images we need  */
    std::unique_ptr<g_image::F_image> cage_image(new g_image::F_image(
                                                 cage_fname, &result));
    if(!result)
    {
        std::cout << "Failed to read in image " << cage_fname << ". Exiting...\n";
        std::exit(1);
    }

    w = cage_image->get_x_dim(), h = cage_image->get_y_dim();

    std::unique_ptr<g_image::F_image> their_image(new g_image::F_image(
                                                  input_fname, &result));
    if(!result)
    {
        std::cout << "Failed to read in image " << input_fname << ". Exiting...\n";
        std::exit(1);
    }

    /*  Read in our small dots  */
    std::unique_ptr<g_image::F_image[]> dots(new g_image::F_image[Cagifier::dot_count]);
    for(unsigned int i = 0; i < Cagifier::dot_count; ++i)
    {
        std::stringstream dot_fname;
        dot_fname << Cagifier::img_dir << "/" << Cagifier::dot_pre << i <<
                     Cagifier::dot_ext;
        g_image::F_image img(dot_fname.str(), &result);
        if(!result)
        {
            std::cout << "Failed to read in image " << dot_fname.str() <<
                         ". Exiting...\n";
            std::exit(1);
        }
        dots[i].replicate(img);
    }

    /*  Convert to a four channel image  */
    //std::unique_ptr<float[]> t_buff(new float[their_image->get_total_size()]);
    unsigned int new_size = their_image->get_total_size() / their_image->get_channels()
                            * 4;
    std::unique_ptr<float[]> f_buff(new float[new_size]);
    std::fill(f_buff.get(), f_buff.get() + new_size, 1.f);
    masking::to_given_channels_f(f_buff.get(), their_image->data,
                                 4, their_image->get_channels(),
                                 new_size, their_image->get_total_size());

    uint32_t n_w = their_image->get_x_dim(), n_h = their_image->get_y_dim();

    std::unique_ptr<g_image::F_image> w_im(new g_image::F_image(n_w, n_h, 4,
                                           f_buff.get(), new_size));
    /*  Create main SDL image  */
    main_window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED, w,
                                   h, SDL_WINDOW_SHOWN);
    if(main_window == 0)
    {
        std::cerr << "Failed to create window. Exiting...\n";
        std::exit(1);
    }

    screen = SDL_GetWindowSurface(main_window);

    /*  Call our main loop  */
    result = Cagifier::main_loop(w_im.get(), cage_image.get(),
                                 main_window, screen, output_fname,
                                 dots.get(), output_pts,
                                 flags, in_pts, c_in_pts);

    /*  Clear window and exit.  */
    SDL_FreeSurface(screen);
    SDL_DestroyWindow(main_window);

    /*  Close SDL subsystems  */
    Initialization::close_subsystems();

    return 0;
}
