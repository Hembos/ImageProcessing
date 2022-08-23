#include "medianfilter.h"

MedianFilter::MedianFilter()
{

}

Matrix<int> MedianFilter::exec(const Matrix<int>& intensities, const int& sizeX, const int& sizeY)
{
    Matrix<int> window(1, 9);

    Matrix<int> resImage(sizeY, sizeX);

    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
        {
            double curInt = intensities.getValue(i, j);

            window.setValue(0, 0, intensities.getValue(i - 1, j - 1, curInt));
            window.setValue(0, 1, intensities.getValue(i - 1, j, curInt));
            window.setValue(0, 2, intensities.getValue(i - 1, j + 1, curInt));
            window.setValue(0, 3, intensities.getValue(i, j - 1, curInt));
            window.setValue(0, 4, intensities.getValue(i, j, curInt));
            window.setValue(0, 5, intensities.getValue(i, j + 1, curInt));
            window.setValue(0, 6, intensities.getValue(i + 1, j - 1, curInt));
            window.setValue(0, 7, intensities.getValue(i + 1, j, curInt));
            window.setValue(0, 8, intensities.getValue(i + 1, j + 1, curInt));

            window.sort();

            int newIntensity = window.getValue(0, 4);

            resImage.setValue(i, j, newIntensity);
        }
    }

    return resImage;
}
