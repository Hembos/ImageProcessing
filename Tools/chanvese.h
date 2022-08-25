#ifndef CHANVESE_H
#define CHANVESE_H

#include <QImage>
#include "Utils/matrix.h"
#include "Utils/maskpoint.h"
#include <cmath>

class ChanVese
{
private:
    const int ITERATIONS_NUM = 100;
    const double ETA = 1e-8;
    const double MU = 0.1;
//    const double LAM1 = 1;
//    const double LAM2 = 0.5;
    const double TOLERANCE = 1e-3;
    const double DT = 1;
    const double EPS = 1;
    const double DF2SS = 0.5;
    const double R2MU = 0.5;
    const double NU = 0.1;

    double sensitivity_ = 0.5;

    int sizeX;
    int sizeY;

public:
    ChanVese();

    void exec(std::unordered_set<MaskPoint, MaskPoint::HashFunction>& resMask, const QImage& originalImage);

    void setSensitivity(const double& newSensitivity);

private:
    double calculateAverageIntensity(std::unordered_set<MaskPoint, MaskPoint::HashFunction>& resMask, const Matrix<double>& intensities);

    void calculateGamma(double c, const Matrix<double>& intesities, double* gamma);

    double updatePhi(Matrix<double>& phiMatrix, const Matrix<double>& rMatrix);

    double calculateA(const Matrix<double>& phiMatrix, int x, int y);
    double calculateB(const Matrix<double>& phiMatrix, int x, int y);

    double f1(int x, int y, const double& c, const Matrix<double>& intensities);
    double f2(int x, int y, const double& c, const Matrix<double>& intensities, const double* gamma);
};

template<size_t bucketsNumber>
class NormalizedIntensityHistogram {
public:
    NormalizedIntensityHistogram() {
        reset();
    }

    void reset() {
        observationsNumber = 0;
        std::fill(&buckets[0], &buckets[bucketsNumber - 1], 0);
    }

    void add(double val) {
        size_t idx = index(val);
        buckets[idx] += 1.0;
        observationsNumber++;
    }

    double get(double val) const {
        return buckets[index(val)];
    }

    void normalize(double denominator) {
        for (size_t i = 0; i < bucketsNumber; i++) {
            buckets[i] /= denominator;
        }
    }

    void normalize() {
        if (observationsNumber != 0) {
            normalize(static_cast<double>(observationsNumber));
            observationsNumber = 0;
        }
        else {
            normalize(sum());
        }
    }

    double sum() {
        double res = 0.0;
        for (size_t i = 0; i < bucketsNumber; i++) {
            res += buckets[i];
        }
        return res;
    }

    bool otsu2(double &border1, double &border2)
    {
        int k1 = -1, k2 = -1;

        double sigma_b_max = -1;

        double mu_t = 0;
        for (int i = 0; i < bucketsNumber; i++)
            mu_t += i * buckets[i];

        double omega_0 = 0;
        double mu_0_tmp = 0;
        for (int i = 0; i < bucketsNumber - 1; i++)
        {
            omega_0 += buckets[i];
            mu_0_tmp += i * buckets[i];

            double mu_0 = mu_0_tmp;
            if (omega_0 != 0)
                mu_0 /= omega_0;

            double omega_1 = 0;
            double mu_1_tmp = 0;
            for (int j = i + 1; j < bucketsNumber; j++)
            {
                omega_1 += buckets[j];
                mu_1_tmp += j * buckets[j];
                double mu_1 = mu_1_tmp;
                if (omega_1 != 0)
                    mu_1 /= omega_1;

                double omega_2 = 0;
                double mu_2 = 0;
                for (int k = j + 1; k < bucketsNumber; k++)
                {
                    omega_2 += buckets[k];
                    mu_2 += k * buckets[k];
                } // end of for k

                if (omega_0 == 0 || omega_1 == 0 || omega_2 == 0)
                    continue;

                mu_2 /= omega_2;

                double sigma_b = omega_0 * (pow(mu_0 - mu_t, 2.))
                                  + omega_1 * (pow(mu_1 - mu_t, 2.))
                                  + omega_2 * (pow(mu_2 - mu_t, 2.));

                if (sigma_b > sigma_b_max)
                {
                    k1 = i;
                    k2 = j;
                    sigma_b_max = sigma_b;
                }
            } // end of for j
        } // end of for i

        if (k1 == -1 || k2 == -1)
            return false;

        border1 = (double)k1 / bucketsNumber;
        border2 = (double)k2 / bucketsNumber;

        return true;
    }

    void fromImage(const Matrix<double> &image, int sizeX, int sizeY)
    {
        for (int i = 0; i < sizeY; i++)
            for (int j = 0; j < sizeX; j++)
                add(image.getValue(i, j));

        normalize();
    }

    std::array<double, bucketsNumber> buckets = { 0 };
    size_t observationsNumber = 0;
private:
    size_t index(double val) const {
        const size_t ind = static_cast<size_t>(std::ceil(val * (bucketsNumber - 1)));
        if (ind >= bucketsNumber)
            return bucketsNumber - 1;
        if (ind < 0)
            return 0;
        return ind;
    }
};

#endif // CHANVESE_H
