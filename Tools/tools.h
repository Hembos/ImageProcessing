#ifndef TOOLS_H
#define TOOLS_H

#include "zoom.h"
#include "hand.h"
#include "pen.h"
#include "smartrefinement.h"

#include <QImage>

enum class ToolType
{
    ZOOM,
    HAND,
    PEN,
    SMART_BRUSH,
    CHAN_VESE,
    NONE
};

class Tools
{
private:
    Zoom zoom;
    Hand hand;
    Pen pen;
    SmartRefinement smartRefinement;

    QImage editedImage;
public:
    Tools();

    ToolType getToolType(const QString& tool);

    QImage execTool(const QString& tool, QStringList& params, const QImage &filteredImage, const QImage& originalImage, const QSize& size);

    QImage generateNewImage(const QSize& windowSize, const QSize& originalSize);

    void reset(const int& zoomValue);

    void setEditedImage(const QImage &newEditedImage);
    const QImage &getEditedImage() const;
};

#endif // TOOLS_H
