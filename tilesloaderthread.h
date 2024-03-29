#ifndef TILESLOADERTHREAD_H
#define TILESLOADERTHREAD_H

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMutex>
#include <QNetworkReply>
#include <QPixmapCache>
#include <QQueue>
#include <QRect>
#include <QSettings>
#include <QThread>

#include "tilessource.h"

class TilesLoaderThread : public QThread
{
    Q_OBJECT

public:
    explicit TilesLoaderThread(QObject *parent = 0);
    ~TilesLoaderThread();
    void loadTiles(TilesSource *source, const QRect &bounds, int zoom, bool online);

signals:
    void tileLoaded(QPoint tp);
    void tilesLoading(int count);

public slots:
    void handleNetworkData(QNetworkReply *reply);

protected:
    void run();

private:
    bool m_restart;
    bool m_abort;
    QMutex m_mutex;
    QString m_tilesPath;
    TilesSource *m_source;
    QRect m_bounds;
    int m_zoom;
    bool m_online;
    QNetworkAccessManager *m_manager;
    QUrl m_url;
    QList<QNetworkReply *> m_replies;
    QQueue<QNetworkRequest> m_tilesQueue;
    int m_tileLifetime;

    void loadNextTile();
    QString dirPath(const QString &source, int zoom, const QPoint &tp);
    QString tilePath(const QString &source, int zoom, const QPoint &tp, const QString &format);
};

#endif // TILESLOADERTHREAD_H
