#ifndef TOOLS_H
#define TOOLS_H

#include "pen.h"
#include "smartrefinement.h"

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
    SmartRefinement smartRefinement;

    QImage editedImage;
public:
    Tools();

    ToolType getToolType(const QString& tool);

    void execTool(const QString& tool, QStringList& params, const QImage &filteredImage, const QImage& originalImage, const int& zoomValue, const QPoint& offset);

    void reset();

    void setEditedImage(const QImage &newEditedImage);
    const QImage &getEditedImage() const;
};

#endif // TOOLS_H
