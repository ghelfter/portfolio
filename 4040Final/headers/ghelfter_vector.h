/*  --------------------     Galen Helfter    --------------------  */
/*  --------------------   ghelfter_vector.h  --------------------  */
/*  --------------------  ghelfter@gmail.com  --------------------  */
#ifndef _GHELFTER_VECTOR_H_
#define _GHELFTER_VECTOR_H_

#include <iostream>

namespace Matrix
{
    const unsigned int Vector3_dim = 3;
    class Vector3
    {
      protected:
        double elements[3];
      public:
        /*  Constructors  */
        Vector3();
        Vector3(const Matrix::Vector3 &vec);
        Vector3(double v0, double v1, double v2);

        double get(unsigned int position) const;
        void set(unsigned int pos, double value);
        void set(double v1, double v2, double v3);
        void normalize_w();

        /*  Accessing operators  */
        double& operator[](int pos);
        const double& operator[](int pos) const;

        /*  Mathematical operators  */
        friend Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
        friend Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
        friend Vector3 operator*(const Vector3 &vec, double scale);
        friend Vector3 operator*(double scale, const Vector3 &vec);
        friend Vector3 operator/(const Vector3 &vec, double scale);
        friend bool operator==(const Vector3 &v1, const Vector3 &v2);
        /*  Dot product  */
        friend double operator*(const Vector3 &v1, const Vector3 &v2);
        friend std::ostream& operator<<(std::ostream &out, const Vector3 &vec);
    };

    const Vector3 empty_vector(0.0, 0.0, 0.0);

    Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
    Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
    Vector3 operator*(const Vector3 &vec, double scale);
    Vector3 operator*(double scale, const Vector3 &vec);
    Vector3 operator/(const Vector3 &vec, double scale);
    Vector3 operator/(double scale, const Vector3 &vec);
    bool operator==(const Vector3 &v1, const Vector3 &v2);
    double operator*(const Vector3 &v1, const Vector3 &v2);
    std::ostream& operator<<(std::ostream &out, const Vector3 &vec);
}

#endif
