#include "filters.h"

Filters::FilterType Filters::getFilterType(QString filter)
{
    FilterType type = FilterType::NONE;

    if (filter == "Border")
        type = FilterType::BORDER_HIGHLIGHT;
    if (filter == "Median")
        type = FilterType::MEDIAN;
    if (filter == "Gauss")
        type = FilterType::GAUSS;
    if (filter == "Mean")
        type = FilterType::MEAN;

    return type;
}

Filters::Filters()
{

}

void Filters::exec(QString filter)
{
    FilterType type = getFilterType(filter);

    int sizeX = filteredImage.width();
    int sizeY = filteredImage.height();

    Matrix<int> intensities(sizeY, sizeX, [this](int i, int j) {
        return QColor(filteredImage.pixel(j, i)).red();
    });

    Matrix<int> newIntensities(sizeY, sizeX);

    switch (type) {
    case Filters::FilterType::MEAN:
        newIntensities = meanFilter.exec(intensities, sizeX, sizeY);
        break;
    case Filters::FilterType::GAUSS:
        newIntensities = gaussFilter.exec(intensities, sizeX, sizeY);
        break;
    case Filters::FilterType::MEDIAN:
        newIntensities = medianFilter.exec(intensities, sizeX, sizeY);
        break;
    case Filters::FilterType::BORDER_HIGHLIGHT:
        newIntensities = borderHighlight.exec(intensities, sizeX, sizeY);
        break;
    case Filters::FilterType::NONE:
        break;
    }

    for (int i = 0; i < sizeY; i++)
    {
        for (int j = 0; j < sizeX; j++)
        {
            int newIntensity = newIntensities.getValue(i, j);
            filteredImage.setPixel(j, i, QColor(newIntensity, newIntensity, newIntensity).rgb());
        }
    }
}

const QImage &Filters::getFilteredImage() const
{
    return filteredImage;
}

void Filters::setFilteredImage(const QImage &newFilteredImage)
{
    filteredImage = newFilteredImage;
}
