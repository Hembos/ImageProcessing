#ifndef GAUSSFILTER_H
#define GAUSSFILTER_H

#include "Utils/matrix.h"

class GaussFIlter
{
private:
    const double SIGMA = 0.8;
    double radius = 1;

public:
    GaussFIlter();

    Matrix<int> exec(const Matrix<int>& intensities, const int& sizeX, const int& sizeY);
};

#endif // GAUSSFILTER_H
