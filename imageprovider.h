#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>
#include "imageprocessing.h"

class ImageProvider : public QQuickImageProvider
{
private:
    ImageProcessing imageProcessing;
public:
    ImageProvider();

    virtual ~ImageProvider();

    QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize) override;

    ImageProcessing& getImageProcessing() {return imageProcessing;}
};

#endif // IMAGEPROVIDER_H
