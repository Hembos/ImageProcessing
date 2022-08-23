#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#include "Utils/matrix.h"

class MedianFilter
{
public:
    MedianFilter();

    Matrix<int> exec(const Matrix<int>& intensities, const int& sizeX, const int& sizeY);
};

#endif // MEDIANFILTER_H
