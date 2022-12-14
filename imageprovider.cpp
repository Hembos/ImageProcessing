#include "imageprovider.h"

ImageProvider::ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{
}

ImageProvider::~ImageProvider()
{

}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QStringList tokens;
    if (id.contains('?'))
        tokens = id.split('?');

    if (tokens.size() == 0)
    {
        QImage image(id);

        image = image.convertToFormat(QImage::Format_ARGB32);

        imageProcessing.setOriginalImage(image, id);
    }

    if (tokens.size() != 0 && tokens[0] == "filteredImage")
        return imageProcessing.getFilteredImage();

    return imageProcessing.getShowingImage();
}
