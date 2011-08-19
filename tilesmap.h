#ifndef TILESMAP_H
#define TILESMAP_H

#include <math.h>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmapCache>
#include <QWidget>

#include "latlon.h"
#include "osmtilessource.h"
#include "tilesloaderthread.h"
#include "tilessource.h"

#define TILE_SIZE 256

class TilesMap : public QWidget
{
    Q_OBJECT

public:
    explicit TilesMap(LatLon center, int zoom, QWidget *parent = 0);
    void setCenter(LatLon center);
    void setZoom(int zoom);
    void setSource(QString source);
    void loadTiles();

signals:
    void updated(const QRect &rect);

public slots:
    void updateTile(QPoint tp);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    int m_width;
    int m_height;
    LatLon m_center;
    int m_zoom;
    QPoint m_tileCenter;
    QPoint m_offset;
    QRect m_tilesRect;
    QPixmap m_emptyTile;
    QHash<QString, TilesSource *> m_sources;
    TilesSource *m_source;
    TilesLoaderThread m_loaderThread;
    QPixmap m_buffer;

    void calcTilesRect();
    QRect tileRect(const QPoint &tp);
    QPointF tileForCoordinate(LatLon coord);
    LatLon coordinateForTile(const QPointF &tile);
};

#endif // TILESMAP_H
