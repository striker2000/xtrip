#include "mapwidget.h"

MapWidget::MapWidget(QWidget *parent) :
    QWidget(parent),
    m_center(55.755831, 37.617673),
    m_zoom(10),
    m_online(true),
    m_lockWheel(false)
{
    m_tiles = new TilesMap(m_center, m_zoom, m_online, this);
    connect(m_tiles, SIGNAL(updated(QRect)), SLOT(updateMap(QRect)));
    connect(m_tiles, SIGNAL(tilesLoading(int)), SIGNAL(tilesLoading(int)));
    connect(m_tiles, SIGNAL(zoomChanged(int)), SIGNAL(zoomChanged(int)));

    m_overlays = new Overlays(m_center, m_zoom, this);

    m_pointDialog = new PointDialog(this);
    connect(m_pointDialog, SIGNAL(accepted()), SLOT(pointDialogAccepted()));

    QShortcut *sc;
    sc = new QShortcut(QKeySequence("Left"), this);
    connect(sc, SIGNAL(activated()), SLOT(panLeft()));
    sc = new QShortcut(QKeySequence("Right"), this);
    connect(sc, SIGNAL(activated()), SLOT(panRight()));
    sc = new QShortcut(QKeySequence("Down"), this);
    connect(sc, SIGNAL(activated()), SLOT(panDown()));
    sc = new QShortcut(QKeySequence("Up"), this);
    connect(sc, SIGNAL(activated()), SLOT(panUp()));
    sc = new QShortcut(QKeySequence("PgUp"), this);
    connect(sc, SIGNAL(activated()), SLOT(zoomIn()));
    sc = new QShortcut(QKeySequence("PgDown"), this);
    connect(sc, SIGNAL(activated()), SLOT(zoomOut()));
    sc = new QShortcut(QKeySequence("Escape"), this);
    connect(sc, SIGNAL(activated()), SLOT(hideAll()));
    sc = new QShortcut(QKeySequence("E"), this);
    connect(sc, SIGNAL(activated()), SLOT(openEditPointDialog()));
    sc = new QShortcut(QKeySequence("Delete"), this);
    connect(sc, SIGNAL(activated()), SLOT(openDeletePointDialog()));
    sc = new QShortcut(QKeySequence("Alt+I"), this);
    connect(sc, SIGNAL(activated()), SLOT(switchOnline()));
    sc = new QShortcut(QKeySequence("Alt+O, Alt+O"), this);
    connect(sc, SIGNAL(activated()), SLOT(openInOSM()));
    sc = new QShortcut(QKeySequence("Alt+O, Alt+G"), this);
    connect(sc, SIGNAL(activated()), SLOT(openInGoogleMaps()));
    sc = new QShortcut(QKeySequence("Alt+O, Alt+Y"), this);
    connect(sc, SIGNAL(activated()), SLOT(openInYandexMaps()));
}

void MapWidget::resizeEvent(QResizeEvent *event)
{
    m_tiles->resize(size());
    m_overlays->resize(size());
}

void MapWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_pressPos = m_movePos = event->pos();
    }
    else if (event->button() == Qt::RightButton) {
        QPoint delta = event->pos() - QPoint(width() / 2, height() / 2);
        LatLon coord = pointToLatLon(latLonToPoint(m_center) + delta);

        QLineEdit *edit;
        edit = m_pointDialog->findChild<QLineEdit *>("latitudeEdit");
        edit->setText(QString().setNum(coord.lat(), 'f', 6));
        edit = m_pointDialog->findChild<QLineEdit *>("longitudeEdit");
        edit->setText(QString().setNum(coord.lon(), 'f', 6));
        edit = m_pointDialog->findChild<QLineEdit *>("nameEdit");
        edit->setText("");
        edit->setFocus();

        m_pointDialog->setWindowTitle("Add Point");
        m_pointDialog->open();
    }
}

void MapWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint delta = event->pos() - m_movePos;
    m_movePos = event->pos();

    QPointF ct = latLonToPoint(m_center) - QPointF(delta);
    m_center = pointToLatLon(ct);

    m_tiles->setCenter(m_center);
    m_overlays->setCenter(m_center);

    update();
}

void MapWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->pos() == m_pressPos) {
        m_overlays->deselect();
    }
    update();
    m_tiles->loadTiles();
}

void MapWidget::wheelEvent(QWheelEvent *event)
{
    if (m_lockWheel) {
        return;
    }

    int newZoom = m_tiles->setZoom(m_zoom + event->delta() / 120);
    if (newZoom != m_zoom) {
        QPoint delta = event->pos() - QPoint(width() / 2, height() / 2);
        LatLon pos = pointToLatLon(latLonToPoint(m_center) + delta);
        m_zoom = newZoom;
        m_center = pointToLatLon(latLonToPoint(pos) - delta);
        m_tiles->setCenter(m_center);
        m_tiles->loadTiles();
        m_overlays->setZoom(m_zoom);
        m_overlays->setCenter(m_center);
        update();
    }

    m_lockWheel = true;
    QTimer::singleShot(100, this, SLOT(unlockWheel()));
}

void MapWidget::zoomIn()
{
    int newZoom = m_tiles->setZoom(m_zoom + 1);
    if (newZoom != m_zoom) {
        m_zoom = newZoom;
        m_tiles->loadTiles();
        m_overlays->setZoom(m_zoom);
        update();
    }
}

void MapWidget::zoomOut()
{
    int newZoom = m_tiles->setZoom(m_zoom - 1);
    if (newZoom != m_zoom) {
        m_zoom = newZoom;
        m_tiles->loadTiles();
        m_overlays->setZoom(m_zoom);
        update();
    }
}

void MapWidget::hideAll()
{
    m_overlays->deselect();
}

void MapWidget::openEditPointDialog()
{
    if (Point *p = m_overlays->selectedPoint()) {
        QLineEdit *edit;
        edit = m_pointDialog->findChild<QLineEdit *>("latitudeEdit");
        edit->setText(QString().setNum(p->coord().lat(), 'f', 6));
        edit = m_pointDialog->findChild<QLineEdit *>("longitudeEdit");
        edit->setText(QString().setNum(p->coord().lon(), 'f', 6));
        edit = m_pointDialog->findChild<QLineEdit *>("nameEdit");
        edit->setText(p->label());
        edit->selectAll();
        edit->setFocus();

        m_pointDialog->setWindowTitle("Edit Point");
        m_pointDialog->open();
    }
}

void MapWidget::openDeletePointDialog()
{
    if (Point *p = m_overlays->selectedPoint()) {
        if (QMessageBox::Ok == QMessageBox::warning(
                    this,
                    "Delete Point",
                    QString("Delete point \"%1\"?").arg(p->label()),
                    QMessageBox::Ok | QMessageBox::Cancel)) {
            m_overlays->deleteSelectedPoint();
        }
    }
}

void MapWidget::pointDialogAccepted()
{
    QLineEdit *edit;
    edit = m_pointDialog->findChild<QLineEdit *>("latitudeEdit");
    qreal lat = edit->text().toDouble();
    edit = m_pointDialog->findChild<QLineEdit *>("longitudeEdit");
    qreal lon = edit->text().toDouble();
    edit = m_pointDialog->findChild<QLineEdit *>("nameEdit");
    QString label = edit->text();
    LatLon coord(lat, lon);

    if (m_overlays->selectedPoint()) {
        m_overlays->updateSelectedPoint(coord, label);
    }
    else {
        m_overlays->addPoint(coord, label);
    }
}

void MapWidget::switchOnline()
{
    m_online = !m_online;
    m_tiles->setOnline(m_online);
    emit onlineSwitched(m_online);
}

void MapWidget::openInOSM()
{
    QDesktopServices::openUrl(QUrl(
        QString("http://www.openstreetmap.org/?lat=%1&lon=%2&zoom=%3&layers=M")
            .arg(m_center.lat())
            .arg(m_center.lon())
            .arg(m_zoom)));
}

void MapWidget::openInGoogleMaps()
{
    QDesktopServices::openUrl(QUrl(
        QString("http://maps.google.com/?ll=%1,%2&z=%3&t=m")
            .arg(m_center.lat())
            .arg(m_center.lon())
            .arg(m_zoom)));
}

void MapWidget::openInYandexMaps()
{
    QDesktopServices::openUrl(QUrl(
        QString("http://maps.yandex.ru/?ll=%2,%1&z=%3&l=map")
            .arg(m_center.lat())
            .arg(m_center.lon())
            .arg(m_zoom)));
}

void MapWidget::pan(const QPoint &delta)
{
    m_center = pointToLatLon(latLonToPoint(m_center) + delta);
    m_tiles->setCenter(m_center);
    m_tiles->loadTiles();
    m_overlays->setCenter(m_center);
    update();
}

QPointF MapWidget::latLonToPoint(LatLon latlon)
{
    qreal x = (latlon.lon() + 180.0) / 360.0;
    qreal y = (M_PI - log(tan(M_PI / 4 + latlon.lat() * (M_PI / 180.0) / 2))) / (2 * M_PI);
    qreal z = static_cast<qreal>(1 << m_zoom);
    return QPointF(x * z * 256, y * z * 256);
}

LatLon MapWidget::pointToLatLon(QPointF point)
{
    qreal z = static_cast<qreal>(1 << m_zoom);
    qreal lat = (2 * atan(exp((0.5 - point.y() / z / 256) * 2 * M_PI)) - M_PI / 2) * 180 / M_PI;
    qreal lon = point.x() / z / 256 * 360.0 - 180.0;
    return LatLon(lat, lon);
}
