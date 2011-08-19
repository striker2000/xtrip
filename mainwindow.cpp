#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    m_map = new MapWidget(this);
    setCentralWidget(m_map);
    m_map->setFocus();
}
