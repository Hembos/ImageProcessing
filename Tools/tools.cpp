#include "tools.h"

#include <QDebug>

void Tools::setEditedImage(const QImage &newEditedImage)
{
    editedImage = newEditedImage;
}

Tools::Tools()
{

}

ToolType Tools::getToolType(const QString &tool)
{
    if (tool == "Zoom")
        return ToolType::ZOOM;
    if (tool == "Hand")
        return ToolType::HAND;
    if (tool == "Pen")
        return ToolType::PEN;
    if (tool == "SmartBrush")
        return ToolType::SMART_BRUSH;
    if (tool == "ChanVese")
        return ToolType::CHAN_VESE;

    return ToolType::NONE;
}

QImage Tools::execTool(const QString &tool, QStringList &params, const QImage &filteredImage, const QImage &originalImage, const QSize& size)
{
    ToolType type = getToolType(tool);

    switch (type)
    {
    case ToolType::CHAN_VESE:
        params.append(QString::number(zoom.getZoomValue()));
        params.append(QString::number(hand.getOffsetX()));
        params.append(QString::number(hand.getOffsetY()));
        smartRefinement.execChanVese(params, editedImage, originalImage, filteredImage);
        break;
    case ToolType::SMART_BRUSH:
        params[0] = QString::number(params[0].toInt() / zoom.getZoomValue() + hand.getOffsetX());
        params[1] = QString::number(params[1].toInt() / zoom.getZoomValue() + hand.getOffsetY());
        smartRefinement.execSmartBrush(params, editedImage, originalImage, filteredImage);
        break;
    case ToolType::PEN:
    {
        int x = params[0].toInt() / zoom.getZoomValue() + hand.getOffsetX();
        int y = params[1].toInt() / zoom.getZoomValue() + hand.getOffsetY();
        smartRefinement.addPointInMask(x, y);
        params[0] = QString::number(x);
        params[1] = QString::number(y);
        pen.exec(params, editedImage, originalImage);
        break;
    }
    case ToolType::HAND:
        params.append(QString::number(zoom.getZoomValue()));
        hand.exec(params, originalImage.size(), size);
        break;
    case ToolType::ZOOM:
        params.append(QString::number(hand.getOffsetX()));
        params.append(QString::number(hand.getOffsetY()));
        zoom.exec(params);
        break;
    case ToolType::NONE:
        break;
    }

    QImage image = generateNewImage(size, originalImage.size());

    return image;
}

QImage Tools::generateNewImage(const QSize& windowSize, const QSize& originalSize)
{
    int zoomValue = zoom.getZoomValue();

    hand.checkOffset(zoomValue, windowSize, originalSize);

    int offsetX = hand.getOffsetX();
    int offsetY = hand.getOffsetY();

    QRect rect(offsetX, offsetY, windowSize.width() / zoomValue, windowSize.height() / zoomValue);

    QImage newImage = editedImage.copy(rect);

    return newImage.scaled(newImage.width() * zoomValue, newImage.height() * zoomValue);
}

void Tools::reset(const int& zoomValue)
{
    zoom.reset(zoomValue);
    hand.reset();
    smartRefinement.reset();
}
