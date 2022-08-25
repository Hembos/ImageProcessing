#include "tools.h"

#include <QDebug>

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

    switch (type)
    {
    case ToolType::CHAN_VESE:
        smartRefinement.execChanVese(params, editedImage, originalImage, filteredImage);
        break;
    case ToolType::SMART_BRUSH:
        params[0] = QString::number(params[0].toInt() / zoomValue + offset.x());
        params[1] = QString::number(params[1].toInt() / zoomValue + offset.y());
        smartRefinement.execSmartBrush(params, editedImage, originalImage, filteredImage);
        break;
    case ToolType::PEN:
    {
        int x = params[0].toInt() / zoomValue + offset.x();
        int y = params[1].toInt() / zoomValue + offset.y();
        smartRefinement.addPointInMask(x, y);
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
    smartRefinement.reset();
}
