#include "point.h"

Point::Point(const LatLon &coord, const QString &label, QWidget *parent) :
    QWidget(parent),
    m_coord(coord)
{
    m_marker = new PointMarker(this);
    m_marker->move(0, 2);
    m_label = new QLabel(label, this);
    m_label->move(15, 0);
    m_label->setFrameShape(QFrame::Box);
    m_label->setAutoFillBackground(true);
}
