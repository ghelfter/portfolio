/*  --------------------     Galen Helfter    --------------------  */
/*  --------------------   ghelfter_matrix.h  --------------------  */
/*  --------------------  ghelfter@gmail.com  --------------------  */
#ifndef _GHELFTER_MATRIX_H_
#define _GHELFTER_MATRIX_H_

#include "ghelfter_vector.h"

/*  Matrix is in column-major order  */
namespace Matrix
{
   const unsigned int Matrix3_3_dim = 3;
   class Matrix3_3
   {
     protected:
       Vector3 columns[3];
     public:
       /*  Constructors  */
       Matrix3_3();
       Matrix3_3(const Matrix3_3 &mat);
       Matrix3_3(Vector3 v1, Vector3 v2, Vector3 v3);
       Matrix3_3(double a00, double a01, double a02,
                 double a10, double a11, double a12,
                 double a20, double a21, double a22);

       double get(unsigned int row, unsigned int col) const;
       void set(unsigned int row, unsigned int col, double value);
       void set(double a00, double a01, double a02,
                double a10, double a11, double a12,
                double a20, double a21, double a22);

       Matrix3_3 transpose() const;
       double determinant() const;
       Matrix3_3 invert() const;

       Vector3& operator[](int pos);
       const Vector3& operator[](int pos) const;

       /*  Mathematical operators  */
       friend Matrix3_3 operator+(const Matrix3_3 &m1, const Matrix3_3 &m2);
       friend Matrix3_3 operator-(const Matrix3_3 &m1, const Matrix3_3 &m2);
       friend Matrix3_3 operator*(const Matrix3_3 &m1, const Matrix3_3 &m2);
       friend Vector3 operator*(const Matrix3_3 &mat, const Vector3 &vec);
       friend bool operator==(const Matrix3_3 &m1, const Matrix3_3 &m2);

       friend Matrix3_3 operator*(const Matrix3_3 &mat, double scale);
       friend Matrix3_3 operator*(double scale, const Matrix3_3 &mat);
       friend Matrix3_3 operator/(const Matrix3_3 &mat, double scale);

       friend std::ostream &operator<<(std::ostream &out, const Matrix3_3 &mat);
   };

   const Matrix3_3 Identity(1.0, 0.0, 0.0,
                            0.0, 1.0, 0.0,
                            0.0, 0.0, 1.0);

   Matrix3_3 operator+(const Matrix3_3 &m1, const Matrix3_3 &m2);
   Matrix3_3 operator-(const Matrix3_3 &m1, const Matrix3_3 &m2);
   Matrix3_3 operator*(const Matrix3_3 &m1, const Matrix3_3 &m2);
   Vector3 operator*(const Matrix3_3 &mat, const Vector3 &vec);
   bool operator==(const Matrix3_3 &m1, const Matrix3_3 &m2);
   Matrix3_3 operator*(const Matrix3_3 &mat, double scale);
   Matrix3_3 operator*(double scale, const Matrix3_3 &mat);
   Matrix3_3 operator/(const Matrix3_3 &mat, double scale);
   Matrix3_3 operator/(double scale, const Matrix3_3 &mat);
   std::ostream &operator<<(std::ostream &out, const Matrix3_3 &mat);
}

#endif
