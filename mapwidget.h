#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <math.h>
#include <QDesktopServices>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMessageBox>
#include <QMouseEvent>
#include <QShortcut>
#include <QTimer>
#include <QWidget>

#include "overlays.h"
#include "pointdialog.h"
#include "tilesmap.h"

class MapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = 0);
    int zoom() { return m_zoom; }
    bool online() { return m_online; }

signals:
    void zoomChanged(int zoom);
    void tilesLoading(int count);
    void onlineSwitched(bool online);

private slots:
    void updateMap(const QRect &r) { update(r); }
    void unlockWheel() { m_lockWheel = false; }
    void panLeft() { pan(QPoint(-100, 0)); }
    void panRight() { pan(QPoint(100, 0)); }
    void panDown() { pan(QPoint(0, 100)); }
    void panUp() { pan(QPoint(0, -100)); }
    void zoomIn();
    void zoomOut();
    void hideAll();
    void openEditPointDialog();
    void openDeletePointDialog();
    void pointDialogAccepted();
    void switchOnline();
    void openInOSM();
    void openInGoogleMaps();
    void openInYandexMaps();

protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    TilesMap *m_tiles;
    Overlays *m_overlays;
    LatLon m_center;
    int m_zoom;
    bool m_online;
    QPoint m_pressPos;
    QPoint m_movePos;
    bool m_lockWheel;
    PointDialog *m_pointDialog;

    void pan(const QPoint &delta);
    QPointF latLonToPoint(LatLon latlon);
    LatLon pointToLatLon(QPointF point);
};

#endif // MAPWIDGET_H
