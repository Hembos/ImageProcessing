#include "smartrefinement.h"

#include <QPainter>

SmartRefinement::SmartRefinement()
{

}

void SmartRefinement::execSmartBrush(const QStringList &params, QImage& image, const QImage &originalImage, const QImage &filteredImage)
{
    smartBrush.exec(params, mask, filteredImage);

    QColor color(params[2].toInt(), params[3].toInt(), params[4].toInt(), params[5].toInt());

    drawMask(color, image, originalImage);
}

void SmartRefinement::execChanVese(const QStringList &params, QImage &image, const QImage &originalImage, const QImage &filteredImage)
{
    chanVese.setSensitivity(params[4].toFloat());
    chanVese.exec(mask, filteredImage);

    QColor color(params[0].toInt(), params[1].toInt(), params[2].toInt(), params[3].toInt());

    drawMask(color, image, originalImage);
}

void SmartRefinement::drawMask(const QColor& color, QImage &image, const QImage &originalImage)
{
    QPainter painter(&image);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);


    for (const auto& point : mask)
    {
        QColor oldColor = originalImage.pixelColor(point.x, point.y);
        painter.setPen(QPen(oldColor));
        painter.drawPoint(point.x, point.y);
        painter.setPen(QPen(color));
        painter.drawPoint(point.x, point.y);
    }

    painter.end();
}

void SmartRefinement::reset()
{
    mask.clear();
}

void SmartRefinement::addPointInMask(const int &x, const int &y)
{
    mask.insert(MaskPoint(x, y));
}
