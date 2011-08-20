#ifndef POINT_H
#define POINT_H

#include <QLabel>
#include <QWidget>

#include "latlon.h"
#include "pointmarker.h"

class Point : public QWidget
{
    Q_OBJECT

public:
    explicit Point(const LatLon &coord, const QString &label, QWidget *parent = 0);
    LatLon coord() { return m_coord; }

private:
    PointMarker *m_marker;
    QLabel *m_label;
    LatLon m_coord;
};

#endif // POINT_H
