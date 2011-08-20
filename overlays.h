#ifndef OVERLAYS_H
#define OVERLAYS_H

#include <math.h>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QWidget>

#include "point.h"

class Overlays : public QWidget
{
    Q_OBJECT

public:
    explicit Overlays(LatLon center, int zoom, QWidget *parent = 0);
    void setCenter(LatLon center);
    void setZoom(int zoom);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    LatLon m_center;
    int m_zoom;
    QList<Point *> m_points;

    void movePoints();
    void loadPoints();
    QPointF latLonToPoint(LatLon latlon);
    LatLon pointToLatLon(QPointF point);
};

#endif // OVERLAYS_H
