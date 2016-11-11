/* -----------  Galen Helfter ------------ */
/* -----------    CPSC 4040   ------------ */
/* -----------  Final Project ------------ */
/* -----------    config.h    ------------ */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define DEFAULT_WIDTH 788
#define DEFAULT_HEIGHT 1000
#define WINDOW_NAME "The Cagifier"

#define DEFAULT_LBUFF 256

#define DEFAULT_CAGE "./images/default_cage.png"
#define DEFAULT_INPUT "./images/default_input.png"
#define DEFAULT_OUTPUT "./images/default_output.png"
#define DEFAULT_PTS_FNAME "./config/default_points.txt"
#define DEFAULT_C_PTS "./config/default_in_points.txt"
#define PROMPT_FNAME "./config/prompt.txt"

#define FLAG_OUTPUT_POINTS "-op"
#define FLAG_OUTPUT_POINTS_LEN 3
#define FLAG_CAGE_FILE "-c"
#define FLAG_CAGE_FILE_LEN 2
#define FLAG_OUT_FILE "-of"
#define FLAG_OUT_FILE_LEN 3
#define FLAG_SPECIFY_INPUT "-i"
#define FLAG_SPECIFY_INPUT_LEN 2
#define FLAG_OUTPUT_ONLY "-su"
#define FLAG_OUTPUT_ONLY_LEN 3
#define FLAG_INPUT_POINTS "-ip"
#define FLAG_INPUT_POINTS_LEN 3
#define FLAG_OO_FILE "-so"
#define FLAG_OO_FILE_LEN 3

#define OUT_ONLY 0xf
#define OUTPUT_POINTS 0xf0
#define READ_POINTS 0xf00

namespace Cagifier
{
    const unsigned int dot_count = 12;
    const unsigned int dot_dim = 9;
    const std::string img_dir = "./images";
    const std::string dot_pre = "dot_";
    const std::string dot_ext = ".png";

    const std::string out_pre = "img_";
    const std::string out_ext = ".png";
}

#endif
