#include "borderhighlight.h"
#include <QDebug>

#include <cmath>
#include <iostream>

BorderHighlight::BorderHighlight()
    :Gx(3, 3, {-1, 0, 1, -1, 0, 1, -1, 0, 1}), Gy(3, 3, {-1, -1, -1, 0, 0, 0, 1, 1, 1})
{

}

int BorderHighlight::prewittX(const Matrix<int>& intensities, int x, int y)
{
    int curInt = intensities.getValue(y, x);
    return Gx.getValue(0, 0) * intensities.getValue(y - 1, x - 1, curInt) + Gx.getValue(0, 2) * intensities.getValue(y - 1, x + 1, curInt) +
            Gx.getValue(1, 0) * intensities.getValue(y, x - 1, curInt) + Gx.getValue(1, 2) * intensities.getValue(y, x + 1, curInt) +
            Gx.getValue(2, 0) * intensities.getValue(y + 1, x - 1, curInt) + Gx.getValue(2, 2) * intensities.getValue(y + 1, x + 1, curInt);
}

int BorderHighlight::prewittY(const Matrix<int>& intensities, int x, int y)
{
    int curInt = intensities.getValue(y, x);
    return Gx.getValue(0, 0) * intensities.getValue(y - 1, x - 1, curInt) + Gx.getValue(2, 0) * intensities.getValue(y + 1, x - 1, curInt) +
            Gx.getValue(0, 1) * intensities.getValue(y - 1, x, curInt) + Gx.getValue(2, 1) * intensities.getValue(y + 1, x, curInt) +
            Gx.getValue(0, 2) * intensities.getValue(y - 1, x + 1, curInt) + Gx.getValue(2, 2) * intensities.getValue(y + 1, x + 1, curInt);
}

Matrix<int> BorderHighlight::exec(const Matrix<int>& intensities, const int& sizeX, const int& sizeY)
{
    Matrix<int> resImage(sizeY, sizeX);

    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
        {
            int gpx = prewittX(intensities, j, i);
            int gpy = prewittY(intensities, j, i);

            resImage.setValue(i, j, sqrt(powf(gpx, 2.0) + powf(gpy, 2.0)));
        }
    }

    return resImage;
}

