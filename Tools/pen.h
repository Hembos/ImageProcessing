#ifndef PEN_H
#define PEN_H

#include <QImage>

class Pen
{
public:
    Pen();

    void exec(const QStringList& params, QImage& image, const QImage& originalImage);
};

#endif // PEN_H
