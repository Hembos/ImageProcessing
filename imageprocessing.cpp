#include "imageprocessing.h"
#include <QDebug>

ImageProcessing::ImageProcessing(QObject *parent) : QObject(parent)
{

}

ImageProcessing::~ImageProcessing()
{

}

void ImageProcessing::setOriginalImage(const QImage& image)
{
    tools.setEditedImage(image);
    filters.setFilteredImage(image);

    originalImage = image;

    int zoomValue = windowSize / std::min(image.width(), image.height());

    tools.reset(zoomValue - 1 > 0 ? zoomValue - 1 : 1);

    windowImage = tools.generateNewImage(QSize(windowSize, windowSize), image.size());
}

QImage ImageProcessing::getEditedImage()
{
    return windowImage;
}

void ImageProcessing::process(QString tool, QStringList params)
{
    QImage newImage = tools.execTool(tool, params, filters.getFilteredImage(), originalImage, QSize(windowSize, windowSize));

    if (!newImage.isNull())
        windowImage = newImage;
}

void ImageProcessing::applyFilter(QString filter)
{
    filters.exec(filter);
}
