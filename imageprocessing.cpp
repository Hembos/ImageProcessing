#include "imageprocessing.h"
#include <QDebug>
#include <QDir>

ImageProcessing::ImageProcessing(QObject *parent) : QObject(parent)
{

}

ImageProcessing::~ImageProcessing()
{

}

void ImageProcessing::setOriginalImage(const QImage& image, const QString& fileName)
{
    QStringList tokens = fileName.split('/');
    this->fileName = tokens[tokens.size() - 1];

    tools.setEditedImage(image);
    filters.setFilteredImage(image);

    originalImage = image;

    int zoomValue = windowSize / std::min(image.width(), image.height());

    tools.reset();
    zoom.reset(zoomValue > 0 ? zoomValue : 1);
    hand.reset();
    filters.reset();
}

QImage ImageProcessing::getShowingImage()
{
    return cutRectFromImage(tools.getEditedImage());
}

QImage ImageProcessing::getFilteredImage()
{
    return cutRectFromImage(filters.getFilteredImage());
}

void ImageProcessing::process(QString tool, QStringList params)
{
    if (!originalImage.isNull())
        tools.execTool(tool, params, filters.getFilteredImage(), originalImage, zoom.getZoomValue(), QPoint(hand.getOffsetX(), hand.getOffsetY()));
}

void ImageProcessing::applyFilter(QString filter)
{
    if (!originalImage.isNull())
        filters.exec(filter);
}

void ImageProcessing::saveImage(const QString& tag)
{
    QString filePath = "./images/" + tag + "/";
    if (!QDir(filePath).exists())
        QDir().mkdir(filePath);

    QImage image = tools.getEditedImage();
    image.save(filePath + fileName);
}

void ImageProcessing::zooming(QStringList params)
{
    params.append(QString::number(hand.getOffsetX()));
    params.append(QString::number(hand.getOffsetY()));
    zoom.exec(params);
}

void ImageProcessing::shift(QStringList params)
{
    params.append(QString::number(zoom.getZoomValue()));
    hand.exec(params, originalImage.size(), QSize(windowSize, windowSize));
}

QImage ImageProcessing::cutRectFromImage(const QImage &image)
{
    int zoomValue = zoom.getZoomValue();

    hand.checkOffset(zoomValue, QSize(windowSize, windowSize), originalImage.size());

    int offsetX = hand.getOffsetX();
    int offsetY = hand.getOffsetY();

    QRect rect(offsetX, offsetY, windowSize / zoomValue, windowSize / zoomValue);

    QImage newImage = image.copy(rect);

    return newImage.scaled(newImage.width() * zoomValue, newImage.height() * zoomValue);
}
