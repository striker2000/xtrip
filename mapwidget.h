#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <math.h>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include <QWidget>

#include "tilesmap.h"

class MapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = 0);
    int zoom();

signals:
    void zoomChanged(int zoom);
    void tilesLoading(int count);

private slots:
    void updateMap(const QRect &r);
    void unlockWheel();

protected:
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    TilesMap *m_tiles;
    LatLon m_center;
    int m_zoom;
    QPoint m_pressPos;
    QPoint m_movePos;
    bool m_lockWheel;

    QPointF latLonToPoint(LatLon latlon);
    LatLon pointToLatLon(QPointF point);
};

#endif // MAPWIDGET_H
