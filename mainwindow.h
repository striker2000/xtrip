#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QStatusBar>

#include "mapwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private slots:
    void setZoomLabel(int zoom);
    void setLoadingLabel(int count);

private:
    MapWidget *m_map;
    QLabel *m_zoomLabel;
    QLabel *m_loadingLabel;
};

#endif // MAINWINDOW_H
