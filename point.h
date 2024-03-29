#ifndef POINT_H
#define POINT_H

#include <QLabel>
#include <QWidget>

#include "latlon.h"
#include "pointmarker.h"

class Point : public QWidget
{
    Q_OBJECT

public:
    explicit Point(const LatLon &coord, const QString &label, QWidget *parent = 0);
    LatLon coord() { return m_coord; }
    QString label() { return m_label->text(); }
    void setCoord(const LatLon &coord) { m_coord = coord; }
    void setLabel(const QString &label);
    void deselect() { m_marker->deselect(); }

signals:
    void selectionChanged(Point *point, bool selected);
    void moved(Point *point, QPoint delta);

private slots:
    void selectionChanged(bool selected);
    void dragStarted();
    void dragMove(QPoint delta);
    void dragFinished();

private:
    PointMarker *m_marker;
    QLabel *m_label;
    LatLon m_coord;
    QPoint m_dragStartPos;
};

#endif // POINT_H
