#include "pointmarker.h"

PointMarker::PointMarker(QWidget *parent) :
    QWidget(parent)
{
    m_marker.load(QCoreApplication::applicationDirPath() + "/img/marker.png");
}

void PointMarker::paintEvent(QPaintEvent *event)
{
    QPainter p;
    p.begin(this);
    p.drawPixmap(0, 0, m_marker);
    p.end();
}
