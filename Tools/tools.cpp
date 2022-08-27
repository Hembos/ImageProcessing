#include "tools.h"

#include <QDebug>

#include <QPainter>

void Tools::setEditedImage(const QImage &newEditedImage)
{
    editedImage = newEditedImage;
}

const QImage &Tools::getEditedImage() const
{
    return editedImage;
}

Tools::Tools()
{

}

ToolType Tools::getToolType(const QString &tool)
{
    if (tool == "Pen")
        return ToolType::PEN;
    if (tool == "SmartBrush")
        return ToolType::SMART_BRUSH;
    if (tool == "ChanVese")
        return ToolType::CHAN_VESE;

    return ToolType::NONE;
}

void Tools::execTool(const QString &tool, QStringList &params, const QImage &filteredImage, const QImage &originalImage, const int& zoomValue, const QPoint& offset)
{
    ToolType type = getToolType(tool);
    QColor color;

    switch (type)
    {
    case ToolType::CHAN_VESE:
        chanVese.setSensitivity(params[4].toFloat());
        chanVese.exec(mask, filteredImage);

        color.setRgba(qRgba(params[0].toInt(), params[1].toInt(), params[2].toInt(), params[3].toInt()));

        drawMask(color, originalImage);
        break;
    case ToolType::SMART_BRUSH:
        params[0] = QString::number(params[0].toInt() / zoomValue + offset.x());
        params[1] = QString::number(params[1].toInt() / zoomValue + offset.y());
        smartBrush.exec(params, mask, filteredImage);

        color.setRgba(qRgba(params[2].toInt(), params[3].toInt(), params[4].toInt(), params[5].toInt()));

        drawMask(color, originalImage);
        break;
    case ToolType::PEN:
    {
        int x = params[0].toInt() / zoomValue + offset.x();
        int y = params[1].toInt() / zoomValue + offset.y();
        mask.insert(MaskPoint(x, y));
        params[0] = QString::number(x);
        params[1] = QString::number(y);
        pen.exec(params, editedImage, originalImage);
        break;
    }
    case ToolType::NONE:
        break;
    }
}

void Tools::reset()
{
    mask.clear();
}

void Tools::drawMask(const QColor& color, const QImage &originalImage)
{
    QPainter painter(&editedImage);
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
