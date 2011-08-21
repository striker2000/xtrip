#ifndef CURSOR_H
#define CURSOR_H

#include <QCoreApplication>
#include <QPainter>
#include <QWidget>

#include "latlon.h"

class Cursor : public QWidget
{
    Q_OBJECT

public:
    explicit Cursor(QWidget *parent = 0);
    LatLon coord() { return m_coord; }
    void setCoord(const LatLon &coord) { m_coord = coord; }

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap m_image;
    LatLon m_coord;
};

#endif // CURSOR_H
