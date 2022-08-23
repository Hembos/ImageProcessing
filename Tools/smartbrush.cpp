#include "smartbrush.h"

#include <QPainter>
#include <cmath>
#include <limits>

SmartBrush::SmartBrush()
{

}

void SmartBrush::exec(const QStringList &params, std::unordered_set<MaskPoint, MaskPoint::HashFunction>& resMask, const QImage &originalImage)
{
    int leftUpCornerX = params[0].toInt() - radius;
    int leftUpCornerY = params[1].toInt() - radius;

    sizeX = 2 * radius + 1;
    sizeY = sizeX;

    Matrix<double> intensities(sizeY, sizeX, [originalImage, leftUpCornerX, leftUpCornerY](int i, int j) {
        return QColor(originalImage.pixel(leftUpCornerX + j, leftUpCornerY + i)).red();
    });

    Matrix<double> densityMatrix = calcDensityField(intensities);

    Matrix<bool> mask(sizeY, sizeX);
    makeInitialMask(mask);

    //Пока без понижения разрешения и упрощенная версия (без построения квадратов)
    runLevelSetAlgorithm(densityMatrix, mask);
    removeLeaks(mask); // Сделать также удаление компоненет связности которые не соеденены с главной точкой

    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
        {
            if (mask.getValue(i, j))
            {
                resMask.insert(MaskPoint(leftUpCornerX + j, leftUpCornerY + i));
            }
        }
    }
}

Matrix<double> SmartBrush::calcDensityField(const Matrix<double>& intensities)
{
    double eps = 1 - (0.8 + SENSITIVITY / 5);

    double threshold = intensities.getValue(sizeY / 2, sizeX / 2);

    //Пока попробуем без нормализации
    double min = std::numeric_limits<double>::max();
    double max = std::numeric_limits<double>::min();

    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            double value = intensities.getValue(y, x);
            if (value > max)
                max = value;
            if (value < min)
                min = value;
        }
    }

    if (max == min) {
        min = max - 1.0;
    }

    threshold = (threshold - min) / (max - min);

    Matrix<double> densityMatrix(sizeY, sizeX, [intensities, eps, threshold, min, max](int i, int j) {
        double value = intensities.getValue(i, j);
        value = (value - min) / (max - min);
        return eps - fabs(value - threshold);
    });

    return densityMatrix;
}

void SmartBrush::runLevelSetAlgorithm(const Matrix<double> &densityField, Matrix<bool> &mask)
{
    Matrix<double> phiMatrix(sizeY, sizeX, [mask](int i, int j){
        if (mask.getValue(i, j))
            return 1.0;
        return -1.0;
    });

    Matrix<double> curvatureMatrix(sizeY, sizeX);
    Matrix<double> fGradPhiMatrix(sizeY, sizeX);

    int iter = 0;
    for (iter = 0; iter < ITERATIONS_NUM; iter++)
    {
        //Нет проверки на alpha < 1
        updateCurvature(phiMatrix, curvatureMatrix);

        const double maxFGradPhi = calculateFGradPhi(phiMatrix, densityField, curvatureMatrix, fGradPhiMatrix);

        double appendix = updatePhi(maxFGradPhi, fGradPhiMatrix, phiMatrix);

        if (iter >= 50 && appendix < 1)
        {
            break;
        }
    }

    std::cout << iter << std::endl;

    mask.forEach([phiMatrix](int i, int j){
        return phiMatrix.getValue(i, j) >= 0.0;
    });
}

void SmartBrush::makeInitialMask(Matrix<bool> &mask)
{
    for (int i = -INITIAL_RADIUS; i <= INITIAL_RADIUS; i++)
    {
        for (int j = -INITIAL_RADIUS; j <= INITIAL_RADIUS; j++)
        {
            if (i * i + j * j <= INITIAL_RADIUS * INITIAL_RADIUS)
                mask.setValue(radius + i, radius +j, true);
        }
    }
}

void SmartBrush::removeLeaks(Matrix<bool> &mask)
{
    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
        {
            if (mask.getValue(i, j))
            {
                int x = j - radius;
                int y = i - radius;
                if (x * x + y * y > radius * radius)
                    mask.setValue(i, j, false);
            }
        }
    }
}

void SmartBrush::updateCurvature(const Matrix<double> &phiMatrix, Matrix<double> &curvatureMatrix)
{
    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
        {
            const double curPhi = phiMatrix.getValue(i, j);

            const double rightPhi = phiMatrix.getValue(i, j + 1),
                        leftPhi = phiMatrix.getValue(i, j - 1),
                        upPhi = phiMatrix.getValue(i - 1, j),
                        downPhi = phiMatrix.getValue(i + 1, j);

            const double dx = (rightPhi - leftPhi) / 2.;
            const double dy = (upPhi - downPhi) / 2.;
            const double dxplus = rightPhi - curPhi;
            const double dyplus = upPhi - curPhi;
            const double dxminus = curPhi - leftPhi;
            const double dyminus = curPhi - downPhi;

            const double dxplusy = (phiMatrix.getValue(i - 1, j + 1) - phiMatrix.getValue(i - 1, j - 1)) / 2.0;
            const double dxminusy = (phiMatrix.getValue(i + 1, j + 1) - phiMatrix.getValue(i + 1, j - 1)) / 2.0;
            const double dyplusx = (phiMatrix.getValue(i - 1, j + 1) - phiMatrix.getValue(i + 1, j + 1)) / 2.0;
            const double dyminusx = (phiMatrix.getValue(i - 1, j - 1) - phiMatrix.getValue(i + 1, j - 1)) / 2.0;

            //Без curEps
            const double nplusx = dxplus / sqrt(CURVATURE_EPSILON + pow(dxplus, 2.) + pow((dyplusx + dy) / 2.0, 2.));
            const double nplusy = dyplus / sqrt(CURVATURE_EPSILON + pow(dyplus, 2.) + pow((dxplusy + dx) / 2.0, 2.));
            const double nminusx = dxminus / sqrt(CURVATURE_EPSILON + pow(dxminus, 2.) + pow((dyminusx + dy) / 2.0, 2.));
            const double nminusy = dyminus / sqrt(CURVATURE_EPSILON + pow(dyminus, 2.) + pow((dxminusy + dx) / 2.0, 2.));

            curvatureMatrix.setValue(i, j, ((nplusx - nminusx) + (nplusy - nminusy)) / 2.0);
        }
    }
}

double SmartBrush::calculateFGradPhi(const Matrix<double> &phiMatrix, const Matrix<double> &densityMatrix, const Matrix<double> &curvatureMatrix, Matrix<double> &fGradPhiMatrix)
{
    double maxFgradphi = std::numeric_limits<double>::min();

    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
        {
            const double phiValue = phiMatrix.getValue(i, j);
            const double F = densityMatrix.getValue(i, j) * ALPHA + curvatureMatrix.getValue(i, j) * (1 - ALPHA);

            const double dxplus = phiMatrix.getValue(i, j + 1) - phiValue;
            const double dyplus = phiMatrix.getValue(i - 1, j) - phiValue;
            const double dxminus = phiValue - phiMatrix.getValue(i, j - 1);
            const double dyminus = phiValue - phiMatrix.getValue(i + 1, j);

            const double gradphimax_x_sqr = (pow(std::max(dxplus, 0.), 2.) + pow(std::max(-dxminus, 0.), 2.));
            const double gradphimin_x_sqr = (pow(std::min(dxplus, 0.), 2.) + pow(std::min(-dxminus, 0.), 2.));
            const double gradphimax_y_sqr = (pow(std::max(dyplus, 0.), 2.) + pow(std::max(-dyminus, 0.), 2.));
            const double gradphimin_y_sqr = (pow(std::min(dyplus, 0.), 2.) + pow(std::min(-dyminus, 0.), 2.));

            double gradPhi;
            if (F > 0)
            {
                gradPhi = sqrt(gradphimax_x_sqr + gradphimax_y_sqr);
            }
            else
            {
                gradPhi = sqrt(gradphimin_x_sqr + gradphimin_y_sqr);
            }

            const double FGradphi = fabs(F * gradPhi);
            if (FGradphi > maxFgradphi) {
                maxFgradphi = FGradphi;
            }

            fGradPhiMatrix.setValue(i, j, F * gradPhi);
        }
    }

    return maxFgradphi;
}

double SmartBrush::updatePhi(const double maxFGradPhi, const Matrix<double> &fGradPhiMatrix, Matrix<double> &phiMatrix)
{
    double dt = DX / maxFGradPhi;
    double sumAppendix = 0;

    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
        {
            const double phiValue = phiMatrix.getValue(i, j);
            const double appendix = dt * fGradPhiMatrix.getValue(i, j);

            sumAppendix += fabs(appendix);
            const double newValue = phiValue + appendix;

            phiMatrix.setValue(i, j, newValue);
        }
    }

    return sumAppendix;
}
