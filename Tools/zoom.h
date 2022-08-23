#ifndef ZOOM_H
#define ZOOM_H

#include <QStringList>
#include <QSize>

class Zoom
{
private:
    int zoomValue = 1;
    int zoomSpeed = 1;

    int zoomMin = 1;
    int zoomMax = 30;
public:
    Zoom();

    int getZoomValue();

    void exec(const QStringList& params);

    void reset(const int& zoomValue);

private:
    void zoomIn();

    void zoomOut();
};

#endif // ZOOM_H
