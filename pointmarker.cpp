#include "pointmarker.h"

PointMarker::PointMarker(QWidget *parent) :
    QWidget(parent),
    m_selected(false)
{
    m_marker.load(QCoreApplication::applicationDirPath() + "/img/marker.png");
    m_markerSelected.load(QCoreApplication::applicationDirPath() + "/img/marker_selected.png");
    setCursor(Qt::PointingHandCursor);
    setMaximumSize(m_marker.size());
    setMinimumSize(m_marker.size());
}

void PointMarker::paintEvent(QPaintEvent *event)
{
    QPainter p;
    p.begin(this);
    p.drawPixmap(0, 0, m_selected ? m_markerSelected : m_marker);
    p.end();
}

void PointMarker::mousePressEvent(QMouseEvent *event)
{
    m_selected = !m_selected;
    repaint();
    emit selectionChanged(m_selected);
}
