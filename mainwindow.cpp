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
}

void MainWindow::setZoomLabel(int zoom)
{
    QString s = "Zoom: %1";
    s = s.arg(zoom);
    m_zoomLabel->setText(s);
}
