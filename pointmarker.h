#ifndef POINTMARKER_H
#define POINTMARKER_H

#include <QCoreApplication>
#include <QMouseEvent>
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
    void dragStarted();
    void dragMove(QPoint delta);
    void dragFinished();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QPixmap m_marker;
    QPixmap m_markerSelected;
    bool m_selected;
    QPoint m_pressPos;
    bool m_dragging;
};

#endif // POINTMARKER_H
