#include "pen.h"

#include <QDebug>
#include <QPainter>

Pen::Pen()
{

}

void Pen::exec(const QStringList &params, QImage &image, const QImage& originalImage)
{
    int x = params[0].toInt();
    int y = params[1].toInt();

    QPainter painter(&image);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    QColor oldColor = originalImage.pixelColor(x, y);
    QColor color(params[2].toInt(), params[3].toInt(), params[4].toInt(), params[5].toInt());
    painter.setPen(QPen(oldColor));
    painter.drawPoint(x, y);
    painter.setPen(QPen(color));
    painter.drawPoint(x, y);
    painter.end();
}
