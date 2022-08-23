#ifndef SMARTREFINEMENT_H
#define SMARTREFINEMENT_H

#include "Tools/smartbrush.h"
#include "Tools/chanvese.h"
#include <unordered_set>

class SmartRefinement
{
private:
    SmartBrush smartBrush;

    ChanVese chanVese;

    std::unordered_set<MaskPoint, MaskPoint::HashFunction> mask;
public:
    SmartRefinement();

    void execSmartBrush (const QStringList& params, QImage& image, const QImage& originalImage, const QImage &filteredImage);

    void execChanVese(const QStringList& params, QImage& image, const QImage& originalImage, const QImage &filteredImage);

    void drawMask(const QColor& color, QImage& image, const QImage &originalImage);

    void reset();

    void addPointInMask(const int& x, const int& y);
};

#endif // SMARTREFINEMENT_H
