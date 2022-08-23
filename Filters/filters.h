#ifndef FILTERS_H
#define FILTERS_H

#include "borderhighlight.h"
#include "medianfilter.h"
#include "gaussfilter.h"
#include "meanfilter.h"
#include <QImage>

class Filters
{
private:
    BorderHighlight borderHighlight;
    MedianFilter medianFilter;
    GaussFIlter gaussFilter;
    MeanFilter meanFilter;

    QImage filteredImage;

    enum class FilterType
    {
        BORDER_HIGHLIGHT,
        MEDIAN,
        GAUSS,
        MEAN,
        NONE
    };

    FilterType getFilterType(QString filter);

public:
    Filters();

    void exec(QString filter);
    const QImage &getFilteredImage() const;
    void setFilteredImage(const QImage &newFilteredImage);
};

#endif
