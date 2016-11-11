/* -----------  Galen Helfter ------------ */
/* -----------    CPSC 4040   ------------ */
/* -----------  Final Project ------------ */
/* ----------- user_input.cpp ------------ */

#include <iostream>

#include "./headers/rendering.h"
#include "./headers/user_input.h"
#include "./headers/img_convert.h"
#include "./headers/ghelfter_vector.h"

/*  Process user input, such as mouse events and keyboard events  */
/*  Should be passes all information that is needed by the handler  */
/*  Behavior is dependent on the current state of the program.  */
void Event::handle_mouse(SDL_Event &e, Cagifier::State &state,
                         unsigned int &lines, Cagifier::Warpline *warplines)
{
    std::cout << "Handling mouse.\n";
    uint16_t x = e.button.x, y = e.button.y;
    std::cout << "X is " << x << " and Y is " << y << std::endl;
    Cagifier::State new_state = state;

    switch(state)
    {
      case Cagifier::point_selection:
        break;
      case Cagifier::first_point:
        new_state = Cagifier::second_point;
        warplines[lines][0].set((double) x, (double) y, 1.0);
        break;
      case Cagifier::second_point:
        warplines[lines][1].set((double) x, (double) y, 1.0);

        lines++;

        /*  Completed selecting the lines.  */
        if(lines == Cagifier::total_lines)
        {
            new_state = Cagifier::calculating;
        }
        else
        {
            new_state = Cagifier::first_point;
        }
        break;
      case Cagifier::calculating:
        break;
      default:
        break;
    }
    state = new_state;
}

/* handle_keyboard
 *  Handles the keyboard events in the main loop. Behavior is dependent on the
 *  current state of the program.
 * */
void Event::handle_keyboard(SDL_Event &e, Cagifier::State &state, bool &running,
                            unsigned int &current)
{
    
    switch(state)
    {
      case Cagifier::point_selection:
        break;
      case Cagifier::first_point:
        switch(e.key.keysym.sym)
        {
          case 'q':  /*  Add logic to */
          case 'Q':
            running = false;
            break;
        }
        break;
      case Cagifier::second_point:
        switch(e.key.keysym.sym)
        {
          case 'q':  /*  Add logic to */
          case 'Q':
            running = false;
            break;
        }
        break;
      case Cagifier::calculating:
        switch(e.key.keysym.sym)
        {
          case Cagifier::cycle_next:
            current = (current + 1) % Cagifier::steps;
            break;
          case 'q':  /*  Add logic to */
          case 'Q':
            running = false;
            break;
          case 'w':  /*  Add logic to write the image  */
          case 'W':
            break;
        }
      default:
        break;
    }
}
