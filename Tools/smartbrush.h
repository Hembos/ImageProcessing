#ifndef SMARTBRUSH_H
#define SMARTBRUSH_H

#include <QImage>
#include "Utils/matrix.h"
#include "Utils/maskpoint.h"

class SmartBrush
{
private:
    const double SENSITIVITY = 0.3;
    const double ALPHA = 0.9;
    const int ITERATIONS_NUM = 100;
    const double DX = 0.05;
    const double CURVATURE_EPSILON = 1e-10;
    const int INITIAL_RADIUS = 3;

    int radius = 7;

    int sizeX;
    int sizeY;
public:
    SmartBrush();

    void exec(const QStringList& params, std::unordered_set<MaskPoint, MaskPoint::HashFunction>& mask, const QImage& originalImage);

private:
    Matrix<double> calcDensityField(const Matrix<double>& intensities);

    void runLevelSetAlgorithm(const Matrix<double>& densityField, Matrix<bool>& mask);

    void makeInitialMask(Matrix<bool>& mask);

    void removeLeaks(Matrix<bool>& mask);

    void updateCurvature(const Matrix<double>& phiMatrix, Matrix<double>& curvatureMatrix);

    double calculateFGradPhi(const Matrix<double>& phiMatrix, const Matrix<double>& densityMatrix, const Matrix<double> &cuvatureMatrix, Matrix<double>& fGradPhiMatrix);

    double updatePhi(const double maxFGradPhi, const Matrix<double>& fGradPhiMatrix, Matrix<double>& phiMatrix);
};

#endif // SMARTBRUSH_H
