#ifndef POINTMARKER_H
#define POINTMARKER_H

#include <QCoreApplication>
#include <QPainter>
#include <QWidget>

class PointMarker : public QWidget
{
    Q_OBJECT

public:
    explicit PointMarker(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QPixmap m_marker;
};

#endif // POINTMARKER_H
