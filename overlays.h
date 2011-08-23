#ifndef OVERLAYS_H
#define OVERLAYS_H

#include <math.h>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QWidget>

#include "cursor.h"
#include "point.h"

class Overlays : public QWidget
{
    Q_OBJECT

public:
    explicit Overlays(LatLon center, int zoom, QWidget *parent = 0);
    void setCenter(LatLon center);
    void setZoom(int zoom);
    LatLon cursor() { return m_cursor->coord(); }
    void showCursor(const LatLon &coord);
    void hideCursor();
    bool isCursorVisible() { return m_cursor->isVisible(); }
    void addPoint(const LatLon &coord, const QString &name);
    Point *selectedPoint() { return m_selectedPoint; }
    void updateSelectedPoint(const LatLon &coord, const QString &name);
    void deleteSelectedPoint();

private slots:
    void pointSelectionChanged(Point *point, bool selected);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    LatLon m_center;
    int m_zoom;
    Cursor *m_cursor;
    QList<Point *> m_points;
    Point *m_selectedPoint;

    void movePoints();
    void loadPoints();
    void savePoints();
    QPointF latLonToPoint(LatLon latlon);
    LatLon pointToLatLon(QPointF point);
};

#endif // OVERLAYS_H
