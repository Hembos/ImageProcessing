#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QImage>
#include <QString>
#include <QObject>

#include "Tools/tools.h"
#include "Filters/filters.h"
#include "Tools/zoom.h"
#include "Tools/hand.h"

class ImageProcessing : public QObject
{
    Q_OBJECT
private:
    QImage originalImage;

    const int windowSize = 800;

    Zoom zoom;
    Hand hand;

    Tools tools;
    Filters filters;

    QString fileName;
public:
    ImageProcessing(QObject* parent = nullptr);
    virtual ~ImageProcessing();

    void setOriginalImage(const QImage& image, const QString& fileName);

    QImage getShowingImage();

    QImage getFilteredImage();

public slots:
    void process(QString tool, QStringList params);

    void applyFilter(QString filter);

    void saveImage();

    void zooming(QStringList params);

    void shift(QStringList params);

private:
    QImage cutRectFromImage(const QImage& image);
};

#endif // IMAGEPROCESSING_H
