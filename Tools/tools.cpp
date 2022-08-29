#include "tools.h"

#include <QDebug>

#include <QPainter>
#include <queue>

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
    if (tool == "ChanVeseAnchors")
        return ToolType::CHAN_VESE_ANCHORS_MODE;
    if (tool == "RemoveRegion")
        return ToolType::REMOVE_REGION;

    return ToolType::NONE;
}

void Tools::execTool(const QString &tool, QStringList &params, const QImage &filteredImage, const QImage &originalImage, const int& zoomValue, const QPoint& offset)
{
    ToolType type = getToolType(tool);
    QColor color;

    int x = params[0].toInt() / zoomValue + offset.x();
    int y = params[1].toInt() / zoomValue + offset.y();

    switch (type)
    {
    case ToolType::REMOVE_REGION:
        removeRegion(originalImage, x, y);
        break;
    case ToolType::CHAN_VESE_ANCHORS_MODE:
        chanVese.setSensitivity(params[4].toFloat());
        chanVese.execWithAnchors(mask, filteredImage, smartBrush.getRadius());

        color.setRgba(qRgba(params[0].toInt(), params[1].toInt(), params[2].toInt(), params[3].toInt()));

        drawMask(color, originalImage);
        break;
    case ToolType::CHAN_VESE:
        chanVese.setSensitivity(params[4].toFloat());
        chanVese.exec(mask, filteredImage);

        color.setRgba(qRgba(params[0].toInt(), params[1].toInt(), params[2].toInt(), params[3].toInt()));

        drawMask(color, originalImage);
        break;
    case ToolType::SMART_BRUSH:
        chanVese.addAnchorPoint(MaskPoint(x, y));

        params[0] = QString::number(x);
        params[1] = QString::number(y);
        smartBrush.exec(params, mask, filteredImage);

        color.setRgba(qRgba(params[2].toInt(), params[3].toInt(), params[4].toInt(), params[5].toInt()));

        drawMask(color, originalImage);
        break;
    case ToolType::PEN:
    {
        if (mask.find(MaskPoint(x, y)) == mask.end())
        {
            mask.insert(MaskPoint(x, y));
            params[0] = QString::number(x);
            params[1] = QString::number(y);
            pen.exec(params, editedImage, originalImage);
        }
        else
        {
            mask.erase(MaskPoint(x, y));
            editedImage.setPixel(x, y, originalImage.pixel(x, y));
        }
        break;
    }
    case ToolType::NONE:
        break;
    default:
        break;
    }
}

void Tools::reset()
{
    mask.clear();
    chanVese.reset();
}

void Tools::drawMask(const QColor& color, const QImage &originalImage)
{
    editedImage = originalImage;
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

void Tools::removeRegion(const QImage &originalImage, int x, int y)
{
    std::queue<MaskPoint> queue;

    const std::vector<MaskPoint> neighbors = { { 1, 0 },{ 0, 1 },{ -1, 0 },{ 0, -1 } };

    queue.push({x, y});
    mask.erase({x, y});

    while (!queue.empty())
    {
        MaskPoint curPoint = queue.front();
        queue.pop();
        editedImage.setPixel(curPoint.x, curPoint.y, originalImage.pixel(curPoint.x, curPoint.y));
        chanVese.removeAnchorPoint(curPoint);
        for (const auto& neighbor : neighbors)
        {
            MaskPoint tmp = {neighbor.x + curPoint.x, neighbor.y + curPoint.y};
            if (mask.find(tmp) != mask.end())
            {
                mask.erase(tmp);
                queue.push(tmp);

            }
        }
    }
}
