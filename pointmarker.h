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
    void deselect() { m_selected = false; repaint(); }

signals:
    void selectionChanged(bool selected);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event) {}

private:
    QPixmap m_marker;
    QPixmap m_markerSelected;
    bool m_selected;
};

#endif // POINTMARKER_H
