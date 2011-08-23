#include "point.h"

Point::Point(const LatLon &coord, const QString &label, QWidget *parent) :
    QWidget(parent),
    m_coord(coord)
{
    m_marker = new PointMarker(this);
    m_marker->move(0, 2);
    connect(m_marker, SIGNAL(selectionChanged(bool)), SLOT(selectionChanged(bool)));
    m_label = new QLabel(label, this);
    m_label->move(15, 0);
    m_label->setFrameShape(QFrame::Box);
    m_label->setAutoFillBackground(true);
    m_label->adjustSize();
}

void Point::setLabel(const QString &label)
{
     m_label->setText(label);
     m_label->adjustSize();
     adjustSize();
}

void Point::selectionChanged(bool selected)
{
    emit selectionChanged(this, selected);
}
