#include "point.h"

Point::Point(const LatLon &coord, const QString &label, QWidget *parent) :
    QWidget(parent),
    m_coord(coord)
{
    m_marker = new PointMarker(this);
    m_marker->move(0, 2);
    connect(m_marker, SIGNAL(selectionChanged(bool)), SLOT(selectionChanged(bool)));
    connect(m_marker, SIGNAL(dragStarted()), SLOT(dragStarted()));
    connect(m_marker, SIGNAL(dragMove(QPoint)), SLOT(dragMove(QPoint)));
    connect(m_marker, SIGNAL(dragFinished()), SLOT(dragFinished()));
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

void Point::dragStarted()
{
    m_dragStartPos = pos();
}

void Point::dragMove(QPoint delta)
{
    move(pos() + delta);
}

void Point::dragFinished()
{
    emit moved(this, pos() - m_dragStartPos);
}
