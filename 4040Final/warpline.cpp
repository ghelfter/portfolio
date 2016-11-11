/* -----------  Galen Helfter ------------ */
/* -----------    CPSC 4040   ------------ */
/* -----------  Final Project ------------ */
/* -----------  warpline.cpp  ------------ */

#include "./headers/warpline.h"

/* read_vec2
 *  Given an input stream, reads in a vector represented only by the
 *  x and y position. Defaults the third element to 1.0.
 * Inputs:
 *  input - Input stream
 * Outputs:
 *  - Vector read from the input stream
 * */
Matrix::Vector3 Cagifier::read_vec2(std::istream &input)
{
    double x = 0.0, y = 0.0;
    input >> x;
    input >> y;
    Matrix::Vector3 res(x, y, 1.f);

    return res;
}

Cagifier::Warpline::Warpline(const Matrix::Vector3 &v1,
                             const Matrix::Vector3 &v2)
{
    points[0] = v1;
    points[1] = v2;
}

/*  Default constructor  */
Cagifier::Warpline::Warpline()
{
    points[1] = points[0] = Matrix::empty_vector;
}

/* get_point
 *  Given a position, returns the point stored at that position.
 * Inputs:
 *  num - Position to access
 * Returns:
 *  - Point stored at num
 * */
Matrix::Vector3 Cagifier::Warpline::get_point(unsigned int num)
{
   return points[num];
}

/* get_midpoint
 *  Returns the point in-between the two points that represent the
 *  line segment.
 * Inputs:
 *  - Nothing
 * Outputs:
 *  - Midpoint
 * */
Matrix::Vector3 Cagifier::Warpline::get_midpoint()
{
    return (points[0] + points[1]) / 2.0;
}

/* set
 *  Given a set of vectors, reassigns the points stored in the line
 *  to those specified.
 * Inputs:
 *  v1 - First point
 *  v2 - Second point
 * Returns:
 *  - Nothing
 * */
void Cagifier::Warpline::set(const Matrix::Vector3 &v1,
                             const Matrix::Vector3 &v2)
{
    points[0] = v1;
    points[1] = v2;
}

/* output
 *  Given an output stream, writes the stored vectors out to
 *  the stream, only printing the first and second elements (x and y)
 * Inputs:
 *  output - Output stream
 * Returns:
 *  - Nothing
 * */
void Cagifier::Warpline::output(std::ostream &output)
{
    output << points[0][0] << " " << points[0][1] << std::endl;
    output << points[1][0] << " " << points[1][1] << std::endl;
}

/* read
 *  Given an input stream, reads in the vectors from the stream.
 * Inputs:
 *  input - Input stream
 * Returns:
 *  - Nothing
 * */
void Cagifier::Warpline::read(std::istream &input)
{
    points[0] = read_vec2(input);
    points[1] = read_vec2(input);
}

/* operator[]
 *  Accesses the Vectors representing position in the warpline.
 * Inputs:
 *  pos - Vector to access
 * Returns:
 *  - Reference to the Vector stored at pos
 * */
Matrix::Vector3& Cagifier::Warpline::operator[](unsigned int pos)
{
    return points[pos];
}

/* operator[]
 *  Accesses the Vectors representing position in the warpline.
 * Inputs:
 *  pos - Vector to access
 * Returns:
 *  - Reference to the Vector stored at pos
 * */
const Matrix::Vector3& Cagifier::Warpline::operator[](unsigned int pos) const
{
    return points[pos];
}
