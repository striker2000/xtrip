#include "overlays.h"

Overlays::Overlays(LatLon center, int zoom, QWidget *parent) :
    QWidget(parent),
    m_center(center),
    m_zoom(zoom)
{
    loadPoints();
}

void Overlays::setCenter(LatLon center)
{
    m_center = center;
    movePoints();
}

void Overlays::setZoom(int zoom)
{
    m_zoom = zoom;
    movePoints();
}

void Overlays::resizeEvent(QResizeEvent *event)
{
    movePoints();
}

void Overlays::movePoints()
{
    QPoint center(width() / 2, height() / 2);
    foreach (Point *p, m_points) {
        QPoint delta = (latLonToPoint(p->coord()) - latLonToPoint(m_center)).toPoint();
        p->move(center + delta + QPoint(-5, -5));
    }
}

void Overlays::loadPoints()
{
    QFile f(QCoreApplication::applicationDirPath() + "/points.tsv");
    f.open(QIODevice::ReadOnly);
    QTextStream stream(&f);
    m_points.clear();
    QString s;
    while (!(s = stream.readLine()).isNull()) {
        QStringList l = s.split("\t");
        Point *p = new Point(LatLon(l.at(0).toDouble(), l.at(1).toDouble()), l.at(2), this);
        m_points.append(p);
        p->show();
    }
    f.close();
}

QPointF Overlays::latLonToPoint(LatLon latlon)
{
    qreal x = (latlon.lon() + 180.0) / 360.0;
    qreal y = (M_PI - log(tan(M_PI / 4 + latlon.lat() * (M_PI / 180.0) / 2))) / (2 * M_PI);
    qreal z = static_cast<qreal>(1 << m_zoom);
    return QPointF(x * z * 256, y * z * 256);
}

LatLon Overlays::pointToLatLon(QPointF point)
{
    qreal z = static_cast<qreal>(1 << m_zoom);
    qreal lat = (2 * atan(exp((0.5 - point.y() / z / 256) * 2 * M_PI)) - M_PI / 2) * 180 / M_PI;
    qreal lon = point.x() / z / 256 * 360.0 - 180.0;
    return LatLon(lat, lon);
}
