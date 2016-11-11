/* -----------  Galen Helfter ------------ */
/* -----------    CPSC 4040   ------------ */
/* -----------  Final Project ------------ */
/* -----------    warping.h   ------------ */

#ifndef _WARPING_H_
#define _WARPING_H_

#include "warpline.h"
#include "g_float.h"

namespace Cagifier
{
    Matrix::Vector3 perpindicular(const Matrix::Vector3 &v1,
                                  const Matrix::Vector3 &v2);
    double get_u_pos(const Matrix::Vector3 &X, const Warpline &line);
    double get_v_pos(const Matrix::Vector3 &X, const Warpline &line);
    Matrix::Vector3 get_x_prime(double u, double v, const Warpline &line);
    double calc_weight(double a, double b, double p, double dist, double len);
    double get_weight(double x, double y, Matrix::Vector3 &dsum,
                     const Warpline &line, const Warpline &p_line);
    Matrix::Vector3 warp_location(double x, double y, Warpline *lines,
                                  Warpline *p_lines, unsigned int nlines);

    void warp_image(const g_image::F_image &source, const g_image::F_image &dest,
                    g_image::F_image &res, Warpline *lines, Warpline *plines,
                    unsigned int nlines);

    void warp_at_time(const g_image::F_image &source, const g_image::F_image &dest,
                      g_image::F_image &res, Warpline *lines, Warpline *plines,
                      unsigned int nlines, double t);


    /* For each pixel X in the destination
     *     DSUM = (0,0)
     *     weightsum = 0 
     *     For each line Pi Qi
     *         calculate u,v based on Pi Qi
     *         calculate X'i based on u,v       and Pi'Qi'
     *         calculate displacement Di = Xi' - Xi for this line
     *         dist = shortest distance from X to Pi Qi
     *         weight = (lengthp / (a + dist))b
     *         DSUM += Di * weight * weightsum += weight
     *     X' = X + DSUM / weightsum
     *     destinationImage(X) = sourceImage(X')
     * */
}

#endif
