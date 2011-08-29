#include "pointmarker.h"
#include <QDebug>

PointMarker::PointMarker(QWidget *parent) :
    QWidget(parent),
    m_selected(false),
    m_dragging(false)
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
    if (event->button() == Qt::LeftButton) {
        m_selected = !m_selected;
        repaint();
        emit selectionChanged(m_selected);
    }
    else if (event->button() == Qt::RightButton) {
        m_pressPos = event->pos();
        m_dragging = true;
        emit dragStarted();
    }
}

void PointMarker::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging) {
        emit dragMove(event->pos() - m_pressPos);
    }
}

void PointMarker::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_dragging) {
        m_dragging = false;
        emit dragFinished();
    }
}
