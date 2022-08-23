#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include <QImage>
#include <QString>
#include <QObject>

#include "Tools/tools.h"
#include "Filters/filters.h"

class ImageProcessing : public QObject
{
    Q_OBJECT
private:
    QImage originalImage;
    QImage windowImage;

    const int windowSize = 800;

    Tools tools;
    Filters filters;
public:
    ImageProcessing(QObject* parent = nullptr);
    virtual ~ImageProcessing();

    void setOriginalImage(const QImage& image);

    QImage getEditedImage();

public slots:
    void process(QString tool, QStringList params);

    void applyFilter(QString filter);
};

#endif // IMAGEPROCESSING_H
