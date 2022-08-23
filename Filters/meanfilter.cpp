#include "meanfilter.h"

#include <cmath>

MeanFilter::MeanFilter()
{

}

Matrix<int> MeanFilter::exec(const Matrix<int>& intensities, const int& sizeX, const int& sizeY)
{
    Matrix<int> resImage(sizeY, sizeX);

    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
        {
            double curInt = intensities.getValue(i, j);
            double sum = 0;
            for (int n = -1; n <= 1; n++)
                for (int m = -1; m <= 1; m++)
                    sum += intensities.getValue(i + n, j + m, curInt);

            int newIntensity = ceil(sum / 9);

            resImage.setValue(i, j, newIntensity);
        }
    }

    return resImage;
}
