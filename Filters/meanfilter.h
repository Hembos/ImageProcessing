#ifndef MEANFILTER_H
#define MEANFILTER_H

#include "Utils/matrix.h"

class MeanFilter
{
public:
    MeanFilter();

    Matrix<int> exec(const Matrix<int>& intensities, const int& sizeX, const int& sizeY);
};

#endif // MEANFILTER_H
