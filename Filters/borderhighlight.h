#ifndef BORDERHIGHLIGHT_H
#define BORDERHIGHLIGHT_H

#include <QImage>
#include <Utils/matrix.h>

class BorderHighlight
{
private:
    Matrix<double> Gx;
    Matrix<double> Gy;
public:
    BorderHighlight();

    Matrix<int> exec(const Matrix<int>& intesities, const int& sizeX, const int& sizeY);

private:
    int prewittX(const Matrix<int>& intensities, int x, int y);
    int prewittY(const Matrix<int>& intensities, int x, int y);
};

#endif // BORDERHIGHLIGHT_H
