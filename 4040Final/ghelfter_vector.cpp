/*  --------------------     Galen Helfter    --------------------  */
/*  --------------------  ghelfter_vector.cpp --------------------  */
/*  --------------------  ghelfter@gmail.com  --------------------  */

#include <iomanip>

// #include "ghelfter_vector.h"
#include "./headers/ghelfter_vector.h"

/*
 * Constructors
 */
Matrix::Vector3::Vector3()
{
    this->elements[0] = this->elements[1] = this->elements[2] = 0.0;
}

Matrix::Vector3::Vector3(const Matrix::Vector3 &vec)
{
    this->elements[0] = vec.elements[0];
    this->elements[1] = vec.elements[1];
    this->elements[2] = vec.elements[2];
}

Matrix::Vector3::Vector3(double v0, double v1, double v2)
{
    this->elements[0] = v0;
    this->elements[1] = v1;
    this->elements[2] = v2;
}

/* Vector3.get
 * Acquires the element of the vector at the given position.
 * Inputs:
 *   position - element of the vector to acquire
 * Returns:
 *   element at the given position
 */
double Matrix::Vector3::get(unsigned int position) const
{
    return elements[position];
}

/* Vector3.set
 * Sets the element at the given position to the given value
 * Inputs:
 *   pos - position to alter the element at
 *   value - value to set the element to
 * Returns:
 *   nothing
 */
void Matrix::Vector3::set(unsigned int pos, double value)
{
    elements[pos] = value;
}

/* Vector3.set
 * Sets the vector to the given values
 * Inputs:
 *   v1, v2, v3 - Values to set elements to
 * Returns:
 *   nothing
 */
void Matrix::Vector3::set(double v1, double v2, double v3)
{
    elements[0] = v1;
    elements[1] = v2;
    elements[2] = v3;
}

/* Vector3.normalize_w
 * Divides every element in the vector by its last position, normalizing it
 * after perspective warping.
 * Inputs:
 *   nothing
 * Returns:
 *   nothing
 */
void Matrix::Vector3::normalize_w()
{
    std::cout  << elements[0] << " / " << elements[2] << " = " << (elements[0] / elements[2]) << std::endl;;
    elements[0] /= elements[2];
    elements[1] /= elements[2];
    std::cout  << elements[1] << " / " << elements[2] << " = " << (elements[1] / elements[2]) << std::endl;;
    elements[2] /= elements[2];
    std::cout  << elements[2] << " / " << elements[2] << " = " << (elements[2] / elements[2]) << std::endl;;
}

/* Vector3 operator[]
 * Accesses the element at the given position
 * Inputs:
 *   pos - position to acquire element at
 * Returns:
 *   reference to the element at position pos
 */
double& Matrix::Vector3::operator[](int pos)
{
    return elements[pos];
}

/* Vector3 operator[]
 * Accesses the element at the given position
 * Inputs:
 *   pos - position to acquire element at
 * Returns:
 *   reference to the element at position pos
 */
const double& Matrix::Vector3::operator[](int pos) const
{
    return elements[pos];
}

/* Vector3 operator+
 * Adds two vectors together element by element.
 * Inputs:
 *   v1 - first vector to be added
 *   v2 - second vector to be added
 * Returns:
 *   vector containing the sum of the elements of v1 and v2
 */
Matrix::Vector3 Matrix::operator+(const Matrix::Vector3 &v1,
                                  const Matrix::Vector3 &v2)
{
    Matrix::Vector3 vec;

    vec[0] = v1[0] + v2[0];
    vec[1] = v1[1] + v2[1];
    vec[2] = v1[2] + v2[2];

    return vec;
}

/* Vector3 operator-
 * Subtracts two vectors element by element.
 * Inputs:
 *   v1 - first vector to be subtracted
 *   v2 - second vector to be subtracted
 * Returns:
 *   vector containing the difference of the elements of v1 and v2
 */
Matrix::Vector3 Matrix::operator-(const Matrix::Vector3 &v1,
                                  const Matrix::Vector3 &v2)
{
    Matrix::Vector3 vec;

    vec[0] = v1[0] - v2[0];
    vec[1] = v1[1] - v2[1];
    vec[2] = v1[2] - v2[2];

    return vec;
}

/* Vector3 operator*
 * Multiplies every element in the vector by the given scalar.
 * Inputs:
 *   vec - vector to be scaled
 *   scale - scalar value
 * Returns:
 *   vector containing the elements of vec scaled by scale
 */
Matrix::Vector3 Matrix::operator*(const Matrix::Vector3 &vec, double scale)
{
    Matrix::Vector3 res;

    res[0] = vec[0] * scale;
    res[1] = vec[1] * scale;
    res[2] = vec[2] * scale;

    return res;
}

/* Vector3 operator*
 * Multiplies every element in the vector by the given scalar.
 * Inputs:
 *   scale - scalar value
 *   vec - vector to be scaled
 * Returns:
 *   vector containing the elements of vec scaled by scale
 */
Matrix::Vector3 Matrix::operator*(double scale, const Matrix::Vector3 &vec)
{
    Matrix::Vector3 res;

    res[0] = vec[0] * scale;
    res[1] = vec[1] * scale;
    res[2] = vec[2] * scale;

    return res;
}

/* Vector3 operator/
 * Divides every element in the vector by the given scalar.
 * Inputs:
 *   vec - vector to be scaled
 *   scale - scalar value
 * Returns:
 *   vector containing the elements of vec divided by scale
 */
Matrix::Vector3 Matrix::operator/(const Matrix::Vector3 &vec, double scale)
{
    Matrix::Vector3 res;

    res[0] = vec[0] / scale;
    res[1] = vec[1] / scale;
    res[2] = vec[2] / scale;

    return res;
}

/* Vector3 operator==
 * Checks to see if every element in the two vectors are the same.
 * Inputs:
 *   v1 - first vector to be compared
 *   v2 - second vector to be compared
 * Returns:
 *   true if every element of v1 equals every element of v2, false otherwise
 */
bool Matrix::operator==(const Matrix::Vector3 &v1, const Matrix::Vector3 &v2)
{
    bool result = true;
    result = result && (v1[0] == v2[0]);
    result = result && (v1[1] == v2[1]);
    result = result && (v1[2] == v2[2]);

    return result;
}

/* Vector3 operator*
 * Computes the dot product of two vectors
 * Inputs:
 *   v1 - first vector
 *   v2 - second vector
 * Returns:
 *   Scalar representing the dot procuct of v1 and v2
 */
double Matrix::operator*(const Matrix::Vector3 &v1,
                         const Matrix::Vector3 &v2)
{
    return (v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]);
}

/* <<
 * Printing operator for the vector
 * Inputs:
 *   out - ostream to write out to
 *   vec - vector to be written
 * Returns:
 *   out - ostream writen to
 */
std::ostream& Matrix::operator<<(std::ostream &out, const Matrix::Vector3 &vec)
{
    for(unsigned int i = 0; i < Matrix::Vector3_dim; ++i)
    {
        out << std::setw(8) << std::setprecision(4) << vec[i] << " ";
    }
    out << std::endl;

    return out;
}
