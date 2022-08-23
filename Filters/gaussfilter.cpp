#include "gaussfilter.h"

#include <cmath>

GaussFIlter::GaussFIlter()
{

}

Matrix<int> GaussFIlter::exec(const Matrix<int> &intensities, const int &sizeX, const int &sizeY)
{
    Matrix<int> resImage(sizeY, sizeX);

    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
        {
            int newIntensity = 0;

            int curIntensity = intensities.getValue(i, j);

            for (int n = -radius; n <= radius; n++)
            {
                for (int m = -radius; m <= radius; m++)
                {
                    newIntensity += intensities.getValue(i + n, j + m, curIntensity) * 1 / (2 * M_PI * SIGMA * SIGMA) * exp(-(n * n + m * m) / (2 * SIGMA * SIGMA));
                }
            }

            resImage.setValue(i, j, newIntensity);
        }
    }

    return resImage;
}
