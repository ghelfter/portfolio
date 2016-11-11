/* -----------  Galen Helfter ------------ */
/* -----------    CPSC 4040   ------------ */
/* -----------  Final Project ------------ */
/* -----------   warpline.h   ------------ */

#ifndef _WARPLINE_H_
#define _WARPLINE_H_

#include "ghelfter_vector.h"

namespace Cagifier
{
    Matrix::Vector3 read_vec2(std::istream &input);
    class Warpline
    {
      protected:
        Matrix::Vector3 points[2];
      public:
        /*  Constructors  */
        Warpline(const Matrix::Vector3 &v1, const Matrix::Vector3 &v2);
        Warpline();

        /*  Accessors  */
        Matrix::Vector3 get_point(unsigned int num);
        Matrix::Vector3 get_midpoint();

        /*  Mutators  */
        void set(const Matrix::Vector3 &v1, const Matrix::Vector3 &v2);

        void output(std::ostream &output);
        void read(std::istream &input);

        /*  Operators  */
        Matrix::Vector3& operator[](unsigned int pos);
        const Matrix::Vector3& operator[](unsigned int pos) const;
    };
}

#endif
