#include "tilesmap.h"

TilesMap::TilesMap(LatLon center, int zoom, QWidget *parent) :
    QWidget(parent),
    m_center(center),
    m_zoom(zoom)
{
    m_emptyTile = QPixmap(TILE_SIZE, TILE_SIZE);
    m_emptyTile.fill(Qt::lightGray);
    m_sources["osm"] = new OSMTilesSource();
    m_source = m_sources["osm"];
    connect(&m_loaderThread, SIGNAL(tileLoaded(QPoint)), SLOT(updateTile(QPoint)));
    connect(&m_loaderThread, SIGNAL(tilesLoading(int)), SIGNAL(tilesLoading(int)));
}

void TilesMap::setCenter(LatLon center)
{
    m_center = center;
    calcTilesRect();
}

int TilesMap::setZoom(int zoom)
{
    if (zoom < 0) {
        zoom = 0;
    }
    else if (zoom > m_source->maxZoom()) {
        zoom = m_source->maxZoom();
    }
    if (zoom != m_zoom) {
        m_zoom = zoom;
        emit zoomChanged(m_zoom);
        calcTilesRect();
    }
    return m_zoom;
}

void TilesMap::setSource(QString source)
{
    if (m_sources.contains(source)) {
        m_source = m_sources[source];
    }
    else {
        m_source = m_sources["osm"];
    }
    loadTiles();
}

void TilesMap::paintEvent(QPaintEvent *event)
{
    QPainter p;
    p.begin(this);

    QPixmap tile;
    QString key;
    for (int x = m_tilesRect.left(); x <= m_tilesRect.right(); x++) {
        for (int y = m_tilesRect.top(); y <= m_tilesRect.bottom(); y++) {
            QPoint tp(x, y);
            QRect box = tileRect(tp);
            if (event->rect().intersects(box)) {
                key = QString("tile.%1.%2.%3.%4")
                        .arg(m_source->id()).arg(m_zoom).arg(tp.x()).arg(tp.y());
                if (QPixmapCache::find(key, tile)) {
                    p.drawPixmap(box, tile);
                }
                else {
                    bool drawn = false;
                    for (int k = 2, z = m_zoom - 1; k <= 8; k *= 2, z--) {
                        key = QString("tile.%1.%2.%3.%4")
                                .arg(m_source->id()).arg(z).arg(x / k).arg(y / k);
                        if (QPixmapCache::find(key, tile)) {
                            int s = TILE_SIZE / k;
                            p.drawPixmap(box, tile, QRect((x % k) * s, (y % k) * s, s, s));
                            drawn = true;
                            break;
                        }
                    }
                    if (!drawn) {
                        p.drawPixmap(box, m_emptyTile);
                    }
                }
            }
        }
    }

    p.end();
}

void TilesMap::resizeEvent(QResizeEvent *event)
{
    m_width = width();
    m_height = height();
    calcTilesRect();
    loadTiles();
}

void TilesMap::calcTilesRect()
{
    QPointF ct = tileForCoordinate(m_center);
    qreal tx = ct.x();
    qreal ty = ct.y();

    m_tileCenter = QPoint(floor(tx), floor(ty));

    int xp = m_width / 2 - (tx - floor(tx)) * TILE_SIZE;
    int yp = m_height / 2 - (ty - floor(ty)) * TILE_SIZE;

    int xa = (xp + TILE_SIZE - 1) / TILE_SIZE;
    int ya = (yp + TILE_SIZE - 1) / TILE_SIZE;
    int xs = static_cast<int>(tx) - xa;
    int ys = static_cast<int>(ty) - ya;

    m_offset = QPoint(xp - xa * TILE_SIZE, yp - ya * TILE_SIZE);

    int xe = static_cast<int>(tx) + (m_width - xp - 1) / TILE_SIZE;
    int ye = static_cast<int>(ty) + (m_height - yp - 1) / TILE_SIZE;

    m_tilesRect = QRect(xs, ys, xe - xs + 1, ye - ys + 1);
}

void TilesMap::loadTiles()
{
    QRect rect = m_tilesRect.intersected(QRect(0, 0, 1 << m_zoom, 1 << m_zoom));
    m_loaderThread.loadTiles(
                m_source,
                rect,
                m_zoom
                );
}

void TilesMap::updateTile(QPoint tp)
{
    emit updated(tileRect(tp));
}

QRect TilesMap::tileRect(const QPoint &tp)
{
    QPoint t = tp - m_tilesRect.topLeft();
    int x = t.x() * TILE_SIZE + m_offset.x();
    int y = t.y() * TILE_SIZE + m_offset.y();
    return QRect(x, y, TILE_SIZE, TILE_SIZE);
}

QPointF TilesMap::tileForCoordinate(LatLon coord)
{
    qreal zn = static_cast<qreal>(1 << m_zoom);
    qreal tx = (coord.lon() + 180.0) / 360.0;
    qreal ty = (1.0 - log(tan(coord.lat() * M_PI / 180.0)
                          + 1.0 / cos(coord.lat() * M_PI / 180.0)) / M_PI) / 2.0;
    return QPointF(tx * zn, ty * zn);
}

LatLon TilesMap::coordinateForTile(const QPointF &tile)
{
    qreal zn = static_cast<qreal>(1 << m_zoom);
    qreal n = M_PI - 2 * M_PI * tile.y() / zn;
    qreal lat = 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
    qreal lon = tile.x() / zn * 360.0 - 180.0;
    return LatLon(lat, lon);
}
