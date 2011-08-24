#include "overlays.h"

Overlays::Overlays(LatLon center, int zoom, QWidget *parent) :
    QWidget(parent),
    m_center(center),
    m_zoom(zoom),
    m_selectedPoint(NULL)
{
    m_cursor = new Cursor(this);
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

void Overlays::showCursor(const LatLon &coord)
{
    m_cursor->setCoord(coord);
    QPoint center(width() / 2, height() / 2);
    QPoint delta = (latLonToPoint(coord) - latLonToPoint(m_center)).toPoint();
    m_cursor->move(center + delta + QPoint(-7, -7));
    m_cursor->show();
    if (m_selectedPoint) {
        m_selectedPoint->deselect();
        m_selectedPoint = NULL;
    }
}

void Overlays::hideCursor()
{
    m_cursor->hide();
}

void Overlays::addPoint(const LatLon &coord, const QString &label)
{
    Point *p = new Point(coord, label, this);
    m_points.append(p);
    connect(p, SIGNAL(selectionChanged(Point *, bool)), SLOT(pointSelectionChanged(Point *, bool)));
    QPoint center(width() / 2, height() / 2);
    QPoint delta = (latLonToPoint(coord) - latLonToPoint(m_center)).toPoint();
    p->move(center + delta + QPoint(-5, -7));
    p->show();
    savePoints();
}

void Overlays::updateSelectedPoint(const LatLon &coord, const QString &name)
{
    if (m_selectedPoint) {
        m_selectedPoint->setCoord(coord);
        m_selectedPoint->setLabel(name);
        QPoint center(width() / 2, height() / 2);
        QPoint delta = (latLonToPoint(coord) - latLonToPoint(m_center)).toPoint();
        m_selectedPoint->move(center + delta + QPoint(-5, -7));
        m_selectedPoint->deselect();
        m_selectedPoint = NULL;
        savePoints();
    }
}

void Overlays::deleteSelectedPoint()
{
    if (m_selectedPoint) {
        m_points.removeAll(m_selectedPoint);
        delete m_selectedPoint;
        m_selectedPoint = NULL;
        savePoints();
    }
}

void Overlays::pointSelectionChanged(Point *point, bool selected)
{
    if (selected) {
        if (m_selectedPoint) {
            m_selectedPoint->deselect();
        }
        m_selectedPoint = point;
        m_cursor->hide();
    }
    else {
        m_selectedPoint = NULL;
    }
}

void Overlays::resizeEvent(QResizeEvent *event)
{
    movePoints();
}

void Overlays::movePoints()
{
    QPoint center(width() / 2, height() / 2);
    QPoint delta = (latLonToPoint(m_cursor->coord()) - latLonToPoint(m_center)).toPoint();
    m_cursor->move(center + delta + QPoint(-7, -7));
    foreach (Point *p, m_points) {
        QPoint delta = (latLonToPoint(p->coord()) - latLonToPoint(m_center)).toPoint();
        p->move(center + delta + QPoint(-5, -7));
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
        connect(p, SIGNAL(selectionChanged(Point *, bool)), SLOT(pointSelectionChanged(Point *, bool)));
        p->show();
    }
    f.close();
}

void Overlays::savePoints() {
    QFile f(QCoreApplication::applicationDirPath() + "/points.tsv");
    f.open(QIODevice::WriteOnly);
    QTextStream stream(&f);
    foreach (Point *p, m_points) {
        stream << p->coord().lat() << "\t" << p->coord().lon() << "\t" << p->label() << "\n";
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
