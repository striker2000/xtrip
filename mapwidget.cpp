#include "mapwidget.h"

MapWidget::MapWidget(QWidget *parent) :
    QWidget(parent),
    m_center(55.755831, 37.617673),
    m_zoom(10)
{
    m_tiles = new TilesMap(m_center, m_zoom, this);
    connect(m_tiles, SIGNAL(updated(QRect)), SLOT(updateMap(QRect)));
    connect(m_tiles, SIGNAL(tilesLoading(int)), SIGNAL(tilesLoading(int)));
    connect(m_tiles, SIGNAL(zoomChanged(int)), SIGNAL(zoomChanged(int)));
    m_lockWheel = false;
}

int MapWidget::zoom()
{
    return m_zoom;
}

void MapWidget::updateMap(const QRect &r)
{
    update(r);
}

void MapWidget::resizeEvent(QResizeEvent *event)
{
    m_tiles->resize(width(), height());
}

void MapWidget::keyPressEvent(QKeyEvent *event)
{
    int newZoom;
    switch (event->key()) {
        case Qt::Key_Left:
            pan(QPoint(-100, 0));
            break;
        case Qt::Key_Right:
            pan(QPoint(100, 0));
            break;
        case Qt::Key_Down:
            pan(QPoint(0, 100));
            break;
        case Qt::Key_Up:
            pan(QPoint(0, -100));
            break;
        case Qt::Key_PageUp:
            newZoom = m_tiles->setZoom(m_zoom + 1);
            if (newZoom != m_zoom) {
                m_zoom = newZoom;
                m_tiles->loadTiles();
                update();
            }
            break;
        case Qt::Key_PageDown:
            newZoom = m_tiles->setZoom(m_zoom - 1);
            if (newZoom != m_zoom) {
                m_zoom = newZoom;
                m_tiles->loadTiles();
                update();
            }
            break;
        case Qt::Key_O:
            m_tiles->setSource("osm");
            update();
            break;
    }
}

void MapWidget::mousePressEvent(QMouseEvent *event)
{
    m_pressPos = m_movePos = event->pos();
}

void MapWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint delta = event->pos() - m_movePos;
    m_movePos = event->pos();

    QPointF ct = latLonToPoint(m_center) - QPointF(delta);
    m_center = pointToLatLon(ct);

    m_tiles->setCenter(m_center);

    update();
}

void MapWidget::mouseReleaseEvent(QMouseEvent *event)
{
    update();
    m_tiles->loadTiles();
}

void MapWidget::wheelEvent(QWheelEvent *event)
{
    if (m_lockWheel) {
        return;
    }

    int newZoom = m_tiles->setZoom(m_zoom + event->delta() / 120);
    if (newZoom != m_zoom) {
        QPoint delta = event->pos() - QPoint(width() / 2, height() / 2);
        LatLon pos = pointToLatLon(latLonToPoint(m_center) + delta);
        m_zoom = newZoom;
        m_center = pointToLatLon(latLonToPoint(pos) - delta);
        m_tiles->setCenter(m_center);
        m_tiles->loadTiles();
        update();
    }

    m_lockWheel = true;
    QTimer::singleShot(100, this, SLOT(unlockWheel()));
}

void MapWidget::unlockWheel()
{
    m_lockWheel = false;
}

void MapWidget::pan(const QPoint &delta)
{
    m_center = pointToLatLon(latLonToPoint(m_center) + delta);
    m_tiles->setCenter(m_center);
    m_tiles->loadTiles();
    update();
}

QPointF MapWidget::latLonToPoint(LatLon latlon)
{
    qreal x = (latlon.lon() + 180.0) / 360.0;
    qreal y = (M_PI - log(tan(M_PI / 4 + latlon.lat() * (M_PI / 180.0) / 2))) / (2 * M_PI);
    qreal z = static_cast<qreal>(1 << m_zoom);
    return QPointF(x * z * 256, y * z * 256);
}

LatLon MapWidget::pointToLatLon(QPointF point)
{
    qreal z = static_cast<qreal>(1 << m_zoom);
    qreal lat = (2 * atan(exp((0.5 - point.y() / z / 256) * 2 * M_PI)) - M_PI / 2) * 180 / M_PI;
    qreal lon = point.x() / z / 256 * 360.0 - 180.0;
    return LatLon(lat, lon);
}
