#include "chanvese.h"

#include "Utils/matrix.h"

ChanVese::ChanVese()
{

}

void ChanVese::exec(std::unordered_set<MaskPoint, MaskPoint::HashFunction> &resMask, const QImage &originalImage)
{
    sizeX = originalImage.width();
    sizeY = originalImage.height();

    Matrix<bool> mask(sizeY, sizeX);
    for (const auto& point : resMask)
    {
        mask.setValue(point.y, point.x, true);
    }

    Matrix<double> intensities(sizeY, sizeX, [originalImage](int i, int j){
        return originalImage.pixelColor(j, i).red() / 255.0;
    });

    Matrix<double> phiMatrix(sizeY, sizeX, [mask](int i, int j){
        return mask.getValue(i, j) ? 1 : -1.;
    });

    run(mask, intensities, resMask, phiMatrix);

    resMask.clear();

    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
        {
            if (phiMatrix.getValue(i, j) >= 1e-8)
                resMask.insert(MaskPoint(j, i));
        }
    }
}

void ChanVese::execWithAnchors(std::unordered_set<MaskPoint, MaskPoint::HashFunction> &resMask, const QImage &originalImage, const int& radius)
{
    if (anchorPoints.empty())
        return;

    int width = originalImage.width();
    int height = originalImage.height();

    MaskPoint leftUpCorner(width, height);
    MaskPoint rightDownCorner(0, 0);

    for (const auto& anchor : anchorPoints)
    {
        leftUpCorner.x = std::min(leftUpCorner.x, std::max(anchor.x - radius, 0));
        leftUpCorner.y = std::min(leftUpCorner.y, std::max(anchor.y - radius, 0));

        rightDownCorner.x = std::max(rightDownCorner.x, std::min(anchor.x + radius, width));
        rightDownCorner.y = std::max(rightDownCorner.y, std::min(anchor.y + radius, height));
    }

    sizeX = rightDownCorner.x - leftUpCorner.x + 1;
    sizeY = rightDownCorner.y - leftUpCorner.y + 1;

    std::unordered_set<MaskPoint, MaskPoint::HashFunction> tmp;
    Matrix<bool> mask(sizeY, sizeX);
    for (const auto& point : resMask)
    {
        if (point.x >= leftUpCorner.x && point.x <= rightDownCorner.x && point.y >= leftUpCorner.y && point.y <= rightDownCorner.y)
        {
            tmp.insert({point.y - leftUpCorner.y, point.x - leftUpCorner.x});
            mask.setValue(point.y - leftUpCorner.y, point.x - leftUpCorner.x, true);
        }
    }
    resMask = tmp;

    Matrix<double> intensities(sizeY, sizeX, [originalImage, leftUpCorner](int i, int j){
        return originalImage.pixelColor(j + leftUpCorner.x, i + leftUpCorner.y).red() / 255.0;
    });

    Matrix<double> phiMatrix(sizeY, sizeX, [mask](int i, int j){
        return mask.getValue(i, j) ? 1 : -1.;
    });

    run(mask, intensities, resMask, phiMatrix);

    resMask.clear();

    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
        {
            if (phiMatrix.getValue(i, j) >= 1e-8)
                resMask.insert(MaskPoint(j + leftUpCorner.x, i + leftUpCorner.y));
        }
    }
}

void ChanVese::setSensitivity(const double &newSensitivity)
{
    sensitivity_ = newSensitivity;
}

void ChanVese::addAnchorPoint(MaskPoint point)
{
    anchorPoints.insert(point);
}

void ChanVese::removeAnchorPoint(MaskPoint point)
{
    if (anchorPoints.find(point) != anchorPoints.end())
        anchorPoints.erase(point);
}

void ChanVese::reset()
{
    anchorPoints.clear();
}

double ChanVese::calculateAverageIntensity(std::unordered_set<MaskPoint, MaskPoint::HashFunction> &resMask, const Matrix<double>& intensities)
{    
    double sum = 0;

    for (const auto& point : resMask)
    {
        sum += intensities.getValue(point.y, point.x);
    }

    return sum / resMask.size();
}

void ChanVese::calculateGamma(double c, const Matrix<double> &intesities, double* gamma)
{
    NormalizedIntensityHistogram<256> histogram;

    histogram.fromImage(intesities, sizeX, sizeY);
    double border1, border2;
    if (histogram.otsu2(border1, border2)) {
        if (c < border1)
        {
            gamma[0] = c;
            gamma[1] = border1 - c;
        }
        else
            if (c < border2)
            {
                gamma[0] = c - border1;
                gamma[1] = border2 - c;
            }
            else
            {
                gamma[0] = c - border2;
                gamma[1] = 1 - c;
            }
    }
    else
    {
        gamma[0] = gamma[1] = 0.1;
    }

    if (gamma[0] == 0)
        gamma[0] = 0.1;
    if (gamma[1] == 0)
        gamma[1] = 0.1;

    const double MIN_GAMMA_MULTIPLIER = 0.5;
    const double MAX_GAMMA_MULTIPLIER = 5;


    gamma[0] *= MIN_GAMMA_MULTIPLIER * sensitivity_ + MAX_GAMMA_MULTIPLIER * (1 - sensitivity_);
    gamma[1] *= MIN_GAMMA_MULTIPLIER * sensitivity_ + MAX_GAMMA_MULTIPLIER * (1 - sensitivity_);
//    gamma[0] *= 2 * (1 - sensitivity_);
//    gamma[1] *= 2 * (1 - sensitivity_);
}

double ChanVese::updatePhi(Matrix<double> &phiMatrix, const Matrix<double> &rMatrix)
{
    double tolerance = 0;

    Matrix<double> aMatrix(sizeY, sizeX);
    Matrix<double> bMatrix(sizeY, sizeX);

    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
        {
            double curPhi = phiMatrix.getValue(i, j);

            aMatrix.setValue(i, j, calculateA(phiMatrix, j, i));
            bMatrix.setValue(i, j, calculateB(phiMatrix, j, i));

            double dtDelta = DT * EPS / (M_PI * (EPS * EPS + curPhi * curPhi));

            double newPhi = (curPhi + dtDelta *
                             (
                                -NU +
                               (1 - R2MU) * (aMatrix.getValue(i, j, -1) * phiMatrix.getValue(i + 1, j, -1) +
                                 aMatrix.getValue(i - 1, j, -1) * phiMatrix.getValue(i - 1, j, -1) +
                                 bMatrix.getValue(i, j, -1) * phiMatrix.getValue(i, j + 1, -1) +
                                 bMatrix.getValue(i, j - 1, -1) * phiMatrix.getValue(i, j - 1, -1)) -
                                 R2MU * rMatrix.getValue(i, j)
                              )
                             ) / (1 + dtDelta * (1 - R2MU) *
                                  (aMatrix.getValue(i, j, -1) + aMatrix.getValue(i - 1, j, -1) +
                                  bMatrix.getValue(i, j, -1) + bMatrix.getValue(i, j - 1, -1))
                                  );

            tolerance += fabs(curPhi - newPhi);

            phiMatrix.setValue(i, j, newPhi);
        }
    }

    return tolerance;
}

double ChanVese::calculateA(const Matrix<double> &phiMatrix, int x, int y)
{
    return MU / sqrt(
                ETA * ETA +
                pow(phiMatrix.getValue(y + 1, x, -1) - phiMatrix.getValue(y, x, -1), 2.) +
                pow((phiMatrix.getValue(y, x + 1, -1) - phiMatrix.getValue(y, x - 1, -1)) / 2., 2.)
                );
}

double ChanVese::calculateB(const Matrix<double> &phiMatrix, int x, int y)
{
    return MU / sqrt(
                ETA * ETA +
                pow((phiMatrix.getValue(y + 1, x, -1) - phiMatrix.getValue(y - 1, x, -1)) / 2., 2.) +
                pow(phiMatrix.getValue(y, x, -1) - phiMatrix.getValue(y + 1, x, -1), 2.)
                );
}

double ChanVese::f1(int x, int y, const double &c, const Matrix<double> &intensities)
{
    return pow(intensities.getValue(y, x) - c, 2.);
}

double ChanVese::f2(int x, int y, const double &c, const Matrix<double> &intensities, const double* gamma)
{
    const double z = intensities.getValue(y, x);
    if (z < c - gamma[0] || z > c + gamma[1])
        return 0;
    else if (z < c)
        return 1 + (z - c) / gamma[0];
    return 1 - (z - c) / gamma[1];
}

void ChanVese::run(Matrix<bool>& mask, const Matrix<double>& intensities, std::unordered_set<MaskPoint, MaskPoint::HashFunction> &resMask, Matrix<double>& phiMatrix)
{
    double c = calculateAverageIntensity(resMask, intensities);
    std::cout << "average " << c << std::endl;

    double gamma[2] = {0, 0};

    calculateGamma(c, intensities, gamma);

    double rMax = 0;

    Matrix<double> rMatrix(sizeY, sizeX, [intensities, c, this, gamma, &rMax](int i, int j){
        double rVal = DF2SS * (f1(j, i, c, intensities) - f2(j, i, c, intensities, gamma));

        rMax = std::max(fabs(rVal), rMax);

        return rVal;
    });

    rMatrix.forEach([rMatrix, rMax](int i, int j){
       return rMatrix.getValue(i, j) / rMax;
    });

    for (int iter = 0; iter < ITERATIONS_NUM; iter++)
    {
        double tol = updatePhi(phiMatrix, rMatrix);

//        if (tol <= TOLERANCE)
//            break;

        std::cout << "iter = " << iter << " tol = " << tol << std::endl;
    }
}
