/* -----------  Galen Helfter ------------ */
/* -----------    CPSC 4040   ------------ */
/* -----------  Final Project ------------ */
/* -----------    warping.h   ------------ */

#include <cmath>

#include "./headers/warping.h"

/* perpindicular
 *  Calculates the vector perpindicular to the vector going through
 *  the two points v1 and v2.
 * Inputs:
 *  v1 - First point of the vector
 *  v2 - Second point of the vector
 * Returns:
 *  - Vector representing the perpindicular line from the line PQ
 * */
Matrix::Vector3 Cagifier::perpindicular(const Matrix::Vector3 &v1,
                                        const Matrix::Vector3 &v2)
{
    Matrix::Vector3 result;
    result[0] = v2[1] - v1[1];
    result[1] = v1[0] - v2[0];
    result[2] = 1.f;

    return result;
}

/* get_u_pos
 *  Gets the u pos using the Beier-Neely algorithm for an inverse warp,
 *  according to the given line.
 * Inputs:
 *  X - Initial position
 *  line - Line
 * Returns:
 *  - Target u value
 * */
double Cagifier::get_u_pos(const Matrix::Vector3 &X, const Warpline &line)
{
    Matrix::Vector3 diff = line[1] - line[0];
    diff[2] = 1.f;
    Matrix::Vector3 v1 = X - line[0];
    v1[2] = 1.f;
    return (v1 * diff) / (diff * diff);
}

/* get_v_pos
 *  Gets the v pos using the Beier-Neely algorithm for an inverse warp,
 *  according to the given line.
 * Inputs:
 *  X - Initial position
 *  line - Line
 * Returns:
 *  - Target v value
 * */
double Cagifier::get_v_pos(const Matrix::Vector3 &X, const Warpline &line)
{
    Matrix::Vector3 origin(0.f, 0.f, 1.f);
    Matrix::Vector3 diff = line[1] - line[0];
    diff[2] = 1.f;
    Matrix::Vector3 v1 = X - line[0];
    v1[2] = 1.f;
    return (v1 * perpindicular(origin, diff)) / (std::sqrt(diff * diff));
}

/* get_x_prime
 *  Calculates the X' value given the line and the u and v values.
 * Inputs:
 *  u - Distance along the line
 *  v - Distance from the line
 *  line - Line to get position from
 * Returns:
 *  - Vector representing new position according to the line
 * */
Matrix::Vector3 Cagifier::get_x_prime(double u, double v, const Warpline &line)
{
    Matrix::Vector3 diff = line[1] - line[0];
    Matrix::Vector3 origin(0.f, 0.f, 1.f);
    diff[2] = 1.f;
    return line[0] + (u * diff) + ((v * perpindicular(origin, diff)) 
                      / std::sqrt(diff * diff));
}

/* calc_weight
 *  Given the a, b, and p constants, calculates the weight according to the
 *  line given the distance and length of the line.
 * Inputs:
 *  a - constant
 *  b - constant
 *  p - constant
 *  dist - Distance from the line
 *  len - Length of the line
 * */
double Cagifier::calc_weight(double a, double b, double p, double dist,
                             double len)
{
    return std::pow((std::pow(len, p) / (a + dist)), b);
}

/* get_weight
 *  Calculates the weighting of the given point, considering the line.
 *  Calaculates everything needed for this and updates dsum to reflect
 *  the new added distances.
 * Inputs:
 *  x - X position
 *  y - Y position
 *  dsum - Distance sum
 *  line - Line to warp around
 *  p_line - Target line to warp to
 * Returns:
 *  - Weight calculated according to the line with the given x,y
 * */
double Cagifier::get_weight(double x, double y, Matrix::Vector3 &dsum,
                            const Warpline &line, const Warpline &p_line)
{
    double a = 0.001, b = 2.0, p = 0.0;
    Matrix::Vector3 pos(x, y, 1.f);

    /*  Calculate u and v  */
    double u = Cagifier::get_u_pos(pos, line);
    double v = Cagifier::get_v_pos(pos, line);

    /*  Now calculate x'i  */
    Matrix::Vector3 x_prime = get_x_prime(u, v, p_line);

    /*  Calculate displacement  */
    Matrix::Vector3 disp = x_prime - pos;
    disp[2] = 1.f;

    double dist = 0.0;
    /*  Calculate distance  */
    if(0.0 < u && u < 1.0)
    {
        dist = abs(v);
    }
    else if(u < 0.0)        /*  Distance from P to the point  */
    {
        Matrix::Vector3 temp = (pos - line[0]);
        temp[2] = 1.f;
        dist = std::sqrt(temp * temp);
    }
    else                    /*  Distance from Q to the point  */
    {
        Matrix::Vector3 temp = (pos - line[1]);
        temp[2] = 1.f;
        dist = std::sqrt(temp * temp);
    }
    Matrix::Vector3 diff = line[1] - line[0];
    double len = std::sqrt(diff * diff);

    /*  Calculate weight  */
    double weight  = calc_weight(a, b, p, dist, len);

    dsum = dsum + (disp * weight);
    dsum[2] = 1.f;

    return weight;
}

/* warp_locatation
 *  Warps the given location given the lines for the source image and destination
 *  image.
 * Inputs:
 *  - x - X location of the pixel
 *  - y - Y location of the pixel
 *  - lines - Lines for the source image
 *  - p_lines - Lines for the destination image
 *  - nlines - Number of lines on the image
 * Returns:
 *  - Vector containing the new, resulting position
 * */
Matrix::Vector3 Cagifier::warp_location(double x, double y, Warpline *lines,
                                        Warpline *p_lines, unsigned int nlines)
{
    Matrix::Vector3 result(0.f, 0.f, 1.f);
    Matrix::Vector3 X(x, y, 1.f);
    Matrix::Vector3 dsum(0.f, 0.f, 1.f);
    double weightsum = 0.0;

    for(unsigned int i = 0; i < nlines; ++i)
    {
        weightsum += get_weight(x, y, dsum, lines[i], p_lines[i]); 
    }

    result = X + (dsum / weightsum);
    result[2] = 1.f;

    return result;
}

/* warp_image
 *  Warps the image using the Beier-Neely algorithm. Only moves position.
 * Inputs:
 *  - source - Source image
 *  - dest - Distination image
 *  - res - Resulting image (stores result)
 *  - lines  - Lines of the source image
 *  - plines - Lines for the destination image
 *  - nlines - Number of lines
 * Returns:
 *  - Nothing
 * */
void Cagifier::warp_image(const g_image::F_image &source, const g_image::F_image &dest,
                          g_image::F_image &res, Warpline *lines, Warpline *plines,
                          unsigned int nlines)
{
    /*  Go through the image  */
    for(unsigned int y = 0; y < source.get_y_dim(); ++y)
    {
        for(unsigned int x = 0; x < source.get_x_dim(); ++x)
        {
            Matrix::Vector3 dest_vec = warp_location(x, y, lines, plines, nlines);
            for(unsigned int channel = 0; channel < source.get_channels(); ++channel)
            {
                unsigned int sx = (unsigned int) dest_vec[0];
                unsigned int sy = (unsigned int) dest_vec[1];
                if(sx < source.get_x_dim() && sy < source.get_y_dim())
                    res.set(x, y, channel, source.get(sx, sy, channel));
                else
                    res.set(x, y, channel, 1.f);
            }
        }
    }
}

/* warp_at_time
 *   warps the image using the Beier-Neely algorithm, linearly interpolating both
 *   the warp and the pixel values of the two images, allowing for a cross fade of
 *   sorts.
 * Inputs:
 *  - source - Source image
 *  - dest - Distination image
 *  - res - Resulting image (stores result)
 *  - lines  - Lines of the source image
 *  - plines - Lines for the destination image
 *  - nlines - Number of lines
 *  - t - t value between 0 and 1 allowing for cross-fading
 * Returns:
 *  - Nothing
 * */
void Cagifier::warp_at_time(const g_image::F_image &source, const g_image::F_image &dest,
                            g_image::F_image &res, Warpline *lines, Warpline *plines,
                            unsigned int nlines, double t)
{
    /*  Calculate new warplines  */
    Cagifier::Warpline new_lines[nlines];
    for(unsigned int i = 0; i < nlines; ++i)
    {
        Matrix::Vector3 diff1 = plines[i][0] - lines[i][0];
        Matrix::Vector3 diff2 = plines[i][1] - lines[i][1];
        /*  Scale the vector by the timestep  */
        diff1 = diff1 * t;
        diff2 = diff2 * t;

        diff1[2] = 1.f;
        diff2[2] = 1.f;

        new_lines[i][0] = lines[i][0] + diff1;
        new_lines[i][1] = lines[i][1] + diff2;
        new_lines[i][0][2] = 1.f;
        new_lines[i][1][2] = 1.f;
    }
    for(unsigned int y = 0; y < source.get_y_dim(); ++y)
    {
        for(unsigned int x = 0; x < source.get_x_dim(); ++x)
        {
            Matrix::Vector3 dest_vec = warp_location(x, y, lines, new_lines, nlines);
            for(unsigned int channel = 0; channel < source.get_channels(); ++channel)
            {
                double val = 0.0;
                unsigned int sx = (unsigned int) dest_vec[0];
                unsigned int sy = (unsigned int) dest_vec[1];
                //std::cout << "X is " << x << " and Sx is " << sx << std::endl;
                val = dest.get(x, y, channel) * (t);
                if(sx < dest.get_x_dim() && sy < dest.get_y_dim())
                {
                    val += ((1.f -t) * source.get(sx, sy, channel));
                }
                else
                    val = 1.f;
                res.set(x, y, channel, val);
                if(channel == 3)
                    res.set(x, y, 3, 1.f);
            }
        }
    }
}
