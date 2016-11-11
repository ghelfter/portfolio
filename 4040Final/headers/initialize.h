/* ------------ Galen Helfter ------------ */
/* ------------   CPSC 4040   ------------ */
/* ------------ Final Project ------------ */
/* ------------  initialize.h ------------ */

#ifndef _INITIALIZE_H_
#define _INITIALIZE_H_

namespace Initialization
{

    bool initialize_subsystems();

    void close_subsystems();

    bool setup_window(const char *title, int w, int h);

}

#endif
