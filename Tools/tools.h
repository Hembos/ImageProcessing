#ifndef TOOLS_H
#define TOOLS_H

#include "pen.h"

#include "Tools/smartbrush.h"
#include "Tools/chanvese.h"
#include <unordered_set>

#include <QImage>

enum class ToolType
{
    PEN,
    SMART_BRUSH,
    CHAN_VESE,
    NONE
};

class Tools
{
private:
    Pen pen;

    SmartBrush smartBrush;

    ChanVese chanVese;

    QImage editedImage;

    std::unordered_set<MaskPoint, MaskPoint::HashFunction> mask;

    void drawMask(const QColor& color, const QImage &originalImage);
public:
    Tools();

    ToolType getToolType(const QString& tool);

    void execTool(const QString& tool, QStringList& params, const QImage &filteredImage, const QImage& originalImage, const int& zoomValue, const QPoint& offset);

    void reset();

    void setEditedImage(const QImage &newEditedImage);
    const QImage &getEditedImage() const;
};

#endif // TOOLS_H
