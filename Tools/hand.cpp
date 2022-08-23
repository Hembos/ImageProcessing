#include "hand.h"

#include <QDebug>

Hand::Hand()
{

}

int Hand::getOffsetX()
{
    return offsetX;
}

int Hand::getOffsetY()
{
    return offsetY;
}

void Hand::exec(const QStringList &params, const QSize &originalSize, const QSize &windowSize)
{
    int zoomValue = params[2].toInt();

    if (params[0] == "offsetX")
        changeOffsetX(params[1].toInt() / zoomValue * offsetSpeed, windowSize.width() / zoomValue, originalSize.width());
    else if (params[0] == "offsetY")
        changeOffsetY(params[1].toInt() / zoomValue * offsetSpeed, windowSize.height() / zoomValue, originalSize.height());
}

void Hand::checkOffset(int zoomValue, const QSize &windowSize, const QSize &originalSize)
{
    if (offsetX + windowSize.width() / zoomValue > originalSize.width())
        offsetX += originalSize.width() - windowSize.width() / zoomValue - offsetX;

    if (offsetY + windowSize.height() / zoomValue > originalSize.height())
        offsetY += originalSize.height() - windowSize.height() / zoomValue - offsetY;

    offsetX = std::max(0, offsetX);
    offsetY = std::max(0, offsetY);
}

void Hand::reset()
{
    offsetX = 0;
    offsetY = 0;
}

void Hand::changeOffsetX(int dirX, int width, int originalWidth)
{
    if (dirX > 0)
    {
        int delta = originalWidth - (width + offsetX);
        offsetX += qMin(dirX, delta);
    }
    else
    {
        int delta = offsetX + dirX;
        if (delta < 0)
            offsetX = 0;
        else
            offsetX += dirX;
    }
}

void Hand::changeOffsetY(int dirY, int height, int originalHeight)
{
    if (dirY > 0)
    {
        int delta = originalHeight - (height + offsetY);
        offsetY += qMin(dirY, delta);
    }
    else
    {
        int delta = offsetY + dirY;
        if (delta < 0)
            offsetY = 0;
        else
            offsetY += dirY;
    }
}
