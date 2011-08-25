#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_map = new MapWidget(this);
    setCentralWidget(m_map);
    m_map->setFocus();

    m_zoomLabel = new QLabel(this);
    statusBar()->addWidget(m_zoomLabel);
    connect(m_map, SIGNAL(zoomChanged(int)), SLOT(setZoomLabel(int)));
    setZoomLabel(m_map->zoom());

    m_onlineLabel = new QLabel(this);
    statusBar()->addWidget(m_onlineLabel);
    connect(m_map, SIGNAL(onlineSwitched(bool)), SLOT(setOnlineLabel(bool)));
    setOnlineLabel(m_map->online());

    m_loadingLabel = new QLabel();
    statusBar()->addWidget(m_loadingLabel);
    connect(m_map, SIGNAL(tilesLoading(int)), SLOT(setLoadingLabel(int)));
}

void MainWindow::setZoomLabel(int zoom)
{
    QString s = "Zoom: %1";
    s = s.arg(zoom);
    m_zoomLabel->setText(s);
}

void MainWindow::setOnlineLabel(bool online)
{
    m_onlineLabel->setText(online ? "Online" : "Offline");
}

void MainWindow::setLoadingLabel(int count)
{
    if (count > 0) {
        QString s = "Loading: %1";
        s = s.arg(count);
        m_loadingLabel->setText(s);
    }
    else {
        m_loadingLabel->clear();
    }
}
