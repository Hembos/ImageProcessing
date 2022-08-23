#include "zoom.h"

#include <QDebug>

Zoom::Zoom()
{

}

int Zoom::getZoomValue()
{
    return zoomValue;
}

void Zoom::exec(const QStringList &params)
{
    if (params[0] == "ZoomIn")
        zoomIn();
    else if (params[0] == "ZoomOut")
        zoomOut();
}

void Zoom::reset(const int& zoomValue)
{
    this->zoomValue = zoomValue;
    zoomMin = zoomValue;
}

void Zoom::zoomIn()
{
    if (zoomValue < zoomMax)
        zoomValue += zoomSpeed;
}

void Zoom::zoomOut()
{
    if (zoomValue > zoomMin)
        zoomValue -= zoomSpeed;
}
