/*  --------------------     Galen Helfter    --------------------  */
/*  --------------------  ghelfter_matrix.cpp --------------------  */
/*  --------------------  ghelfter@gmail.com  --------------------  */

#include <iomanip>

// #include "ghelfter_matrix.h"
#include "./headers/ghelfter_matrix.h"

/*
 * Constructors
 */
Matrix::Matrix3_3::Matrix3_3()
{

}

Matrix::Matrix3_3::Matrix3_3(const Matrix3_3 &mat)
{
    columns[0] = mat.columns[0];
    columns[1] = mat.columns[1];
    columns[2] = mat.columns[2];
}

Matrix::Matrix3_3::Matrix3_3(Vector3 v1, Vector3 v2, Vector3 v3)
{
    columns[0] = v1;
    columns[1] = v2;
    columns[2] = v3;
}

Matrix::Matrix3_3::Matrix3_3(double a00, double a01, double a02,
                             double a10, double a11, double a12,
                             double a20, double a21, double a22)
{
    columns[0][0] = a00;
    columns[0][1] = a10;
    columns[0][2] = a20;

    columns[1][0] = a01;
    columns[1][1] = a11;
    columns[1][2] = a21;

    columns[2][0] = a02;
    columns[2][1] = a12;
    columns[2][2] = a22;
}

/* Matrix3_3.get
 * Acquires the element of the matrix given a row and column
 * Inputs:
 *   row - Row of the element
 *   col - Column of the element
 * Returns:
 *   value of the element at the given row and column
 */
double Matrix::Matrix3_3::get(unsigned int row, unsigned int col) const
{
    return columns[col][row];
}

/* Matrix3_3.set
 * Sets the element at the given position to the given value
 * Inputs:
 *   row - Row of the element
 *   col - Column of the element
 *   value - Value to set the element to
 * Returns:
 *   nothing
 */
void Matrix::Matrix3_3::set(unsigned int row, unsigned int col, double value)
{
    columns[col][row] = value;
}

/* Matrix3_3.set
 * Sets each element in the matrix to the equivalently given value
 * Inputs:
 *   aCR - Where C is the column and R is the row, is the value that
 *         the element at that location is set to.
 * Returns:
 *   nothing
 */
void Matrix::Matrix3_3::set(double a00, double a01, double a02,
                            double a10, double a11, double a12,
                            double a20, double a21, double a22)
{
    columns[0][0] = a00;
    columns[0][1] = a10;
    columns[0][2] = a20;

    columns[1][0] = a01;
    columns[1][1] = a11;
    columns[1][2] = a21;

    columns[2][0] = a02;
    columns[2][1] = a12;
    columns[2][2] = a22;
}

/* Matrix3_3.transpose()
 * Computes and returns the transpose of the matrix
 * Inputs:
 *   None
 * Returns:
 *   Transpose of the matrix
 */
Matrix::Matrix3_3 Matrix::Matrix3_3::transpose() const
{
    Matrix3_3 res;

    for(unsigned int col = 0; col < Matrix::Matrix3_3_dim; ++col)
    {
        for(unsigned int row = 0; row < Matrix::Matrix3_3_dim; ++row)
        {
            res.columns[row][col] = columns[col][row];
        }
    }

    return res;
}

/* Matrix3_3.determinant()
 * Computes and returns the determinant of the matrix
 * Inputs:
 *   None
 * Returns:
 *   Determinant of the matrix
 */
double Matrix::Matrix3_3::determinant() const
{
    double A = columns[1][1] * columns[2][2] - columns[2][1] * columns[1][2];
    double D = columns[1][0] * columns[2][2] - columns[2][0] * columns[1][2];
    double C = columns[0][1] * columns[1][2] - columns[1][1] * columns[0][2];
    return (columns[0][0] * A) - (columns[1][0] * D) + (columns[2][0] * C); 
}

/* Matrix3_3.invert()
 * Computes and returns the inverse of the matrix
 * Inputs:
 *   None
 * Returns:
 *   Inverse of the matrix
 */
Matrix::Matrix3_3 Matrix::Matrix3_3::invert() const
{
    Matrix::Matrix3_3 result;

    double A = columns[1][1] * columns[2][2] - columns[2][1] * columns[1][2];
    double B = -(columns[0][1] * columns[2][2] - columns[2][1] * columns[0][2]);
    double C = columns[0][1] * columns[1][2] - columns[1][1] * columns[0][2];
    double D = -(columns[1][0] * columns[2][2] - columns[2][0] * columns[1][2]);
    double E = columns[0][0] * columns[2][2] - columns[2][0] * columns[0][2];
    double F = -(columns[0][0] * columns[1][2] - columns[1][0] * columns[0][2]);
    double G = columns[1][0] * columns[2][1] - columns[2][0] * columns[1][1];
    double H = -(columns[0][0] * columns[2][1] - columns[2][0] * columns[0][1]);
    double I = columns[0][0] * columns[1][1] - columns[1][0] * columns[0][1];

    result.set(A, D, G,
               B, E, H,
               C, F, I);

    result = result / determinant(); 

    /*  Normalize the result  */
    result = result / result[2][2];

    return result;
}

/* Matrix3_3 operator[]
 * Returns the vector (column) given the position specified.
 * Inputs:
 *   pos - Column specifying the vector to be returned
 * Returns:
 *   Column vector stored at position pos
 */
Matrix::Vector3& Matrix::Matrix3_3::operator[](int pos)
{
    return columns[pos];
}

/* Matrix3_3 operator[]
 * Returns the vector (column) given the position specified.
 * Inputs:
 *   pos - Column specifying the vector to be returned
 * Returns:
 *   Column vector stored at position pos
 */
const Matrix::Vector3& Matrix::Matrix3_3::operator[](int pos) const
{
    return columns[pos];
}

/* Matrix3_3 operator+
 * Computes the sum of the elements in each matrix, storing it in a new matrix
 * Inputs:
 *   m1 - First matrix
 *   m2 - Second matrix
 * Returns:
 *   sum of matrices m1 and m2
 */
Matrix::Matrix3_3 Matrix::operator+(const Matrix::Matrix3_3 &m1,
                                    const Matrix::Matrix3_3 &m2)
{
    Matrix::Matrix3_3 result;

    result[0] = m1[0] + m2[0];
    result[1] = m1[1] + m2[1];
    result[2] = m1[2] + m2[2];

    return result;
}

/* Matrix3_3 operator-
 * Computes the difference of the elements in each matrix, storing it in
 * a new matrix
 * Inputs:
 *   m1 - First matrix
 *   m2 - Second matrix
 * Returns:
 *   difference of matrices m1 and m2
 */
Matrix::Matrix3_3 Matrix::operator-(const Matrix::Matrix3_3 &m1,
                                    const Matrix::Matrix3_3 &m2)
{
    Matrix::Matrix3_3 result;

    result[0] = m1[0] - m2[0];
    result[1] = m1[1] - m2[1];
    result[2] = m1[2] - m2[2];

    return result;
}

/* Matrix3_3 operator*
 * Multiplies the matrix by the given vector, returning the result
 * Inputs:
 *   mat - Matrix
 *   vec - Vector
 * Returns:
 *   multiplication of matrix mat and vector vec
 */
Matrix::Vector3 Matrix::operator*(const Matrix::Matrix3_3 &mat,
                                  const Matrix::Vector3 &vec)
{
    Matrix::Vector3 result;
    Matrix::Matrix3_3 tran = mat.transpose();

    for(unsigned int i = 0; i < Matrix::Matrix3_3_dim; ++i)
    {
        result[i] = tran[i] * vec;
    }
    return result;
}

/* Matrix3_3 operator*
 * Multiplies the two matrices together, returning the result
 * Inputs:
 *   m1 - First matrix
 *   m2 - Second matrix
 * Returns:
 *   multiplication of matrices m1 and m2
 */
Matrix::Matrix3_3 Matrix::operator*(const Matrix::Matrix3_3 &m1,
                                    const Matrix::Matrix3_3 &m2)
{
    Matrix::Matrix3_3 result;

    Matrix::Matrix3_3 tran = m1.transpose();

    for(unsigned int row = 0; row < Matrix::Matrix3_3_dim; ++row)
    {
        for(unsigned int col = 0; col < Matrix::Matrix3_3_dim; ++col)
        {
            result[col][row] = tran[row] * m2[col];
        }
    }

    return result;
}

/* Matrix3_3 operator==
 * Checks whether each of the columns in both matrices are equal to each other
 * Inputs:
 *   m1 - First matrix
 *   m2 - Second matrix
 * Returns:
 *   true if every element of m1 is the same as the equivalent in m2
 */
bool Matrix::operator==(const Matrix::Matrix3_3 &m1, const Matrix::Matrix3_3 &m2)
{
    bool result = true;

    result = result && (m1[0] == m2[0]);
    result = result && (m1[1] == m2[1]);
    result = result && (m1[2] == m2[2]);

    return result;
}

/* Matrix3_3 operator*
 * Multiplies every element in the given matrix by the given scalar value
 * Inputs:
 *   mat - Matrix
 *   scale - Scalar value
 * Returns:
 *   Matrix with each element scaled by the scalar value
 */
Matrix::Matrix3_3 Matrix::operator*(const Matrix::Matrix3_3 &mat, double scale)
{
    Matrix::Matrix3_3 result;

    result[0] = mat[0] * scale;
    result[1] = mat[1] * scale;
    result[2] = mat[2] * scale;

    return result;
}

/* Matrix3_3 operator*
 * Multiplies every element in the given matrix by the given scalar value
 * Inputs:
 *   scale - Scalar value
 *   mat - Matrix
 * Returns:
 *   Matrix with each element scaled by the scalar value
 */
Matrix::Matrix3_3 Matrix::operator*(double scale, const Matrix::Matrix3_3 &mat)
{
    Matrix::Matrix3_3 result;

    result[0] = mat[0] * scale;
    result[1] = mat[1] * scale;
    result[2] = mat[2] * scale;

    return result;
}

/* Matrix3_3 operator/
 * Divides every element in the given matrix by the given scalar value
 * Inputs:
 *   mat - Matrix
 *   scale - Scalar value
 * Returns:
 *   Matrix with each element divided by the scalar value
 */
Matrix::Matrix3_3 Matrix::operator/(const Matrix::Matrix3_3 &mat, double scale)
{
    Matrix::Matrix3_3 result;

    result[0] = mat[0] / scale;
    result[1] = mat[1] / scale;
    result[2] = mat[2] / scale;

    return result;
}

/* <<
 * Prints the given matrix out to the given output stream
 * Inputs:
 *   out - Output stream
 *   mat - Matrix to be printed
 * Returns:
 *   out - Output stream
 */
std::ostream& Matrix::operator<<(std::ostream &out, const Matrix::Matrix3_3 &mat)
{
    for(unsigned int row = 0; row < Matrix::Matrix3_3_dim; ++row)
    {
        for(unsigned int col = 0; col < Matrix::Matrix3_3_dim; ++col)
        {
            out << std::setw(8) << std::setprecision(4) << mat[col][row] << " ";
        }
        out << std::endl;
    }
    return out;
}
