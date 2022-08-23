#ifndef HAND_H
#define HAND_H

#include <QStringList>
#include <QSize>

class Hand
{
private:
    int offsetX = 0;
    int offsetY = 0;

    float offsetSpeed = 0.5;
public:
    Hand();

    int getOffsetX();

    int getOffsetY();

    void exec(const QStringList& params, const QSize& originalSize, const QSize &windowSize);

    void checkOffset(int zoomValue, const QSize &windowSize, const QSize& originalSize);

    void reset();

private:
    void changeOffsetX(int dirX, int width, int originalWidth);

    void changeOffsetY(int dirY, int height, int originalHeight);
};

#endif // HAND_H
