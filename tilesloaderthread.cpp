#include "tilesloaderthread.h"

TilesLoaderThread::TilesLoaderThread(QObject *parent) :
    QThread(parent)
{
    QSettings settings(QCoreApplication::applicationDirPath() + "/xtrip.ini", QSettings::IniFormat);
    m_tilesPath = settings.value("TilesPath").toString();
    m_tileLifetime = settings.value("TileLifetime").toInt();
    m_abort = false;
    start();
}

TilesLoaderThread::~TilesLoaderThread()
{
    m_mutex.lock();
    m_abort = true;
    m_mutex.unlock();
    quit();

    wait();
}

void TilesLoaderThread::loadTiles(TilesSource *source, const QRect &bounds, int zoom)
{
    m_mutex.lock();
    m_source = source;
    m_bounds = bounds;
    m_zoom = zoom;
    m_tilesQueue.clear();
    m_restart = true;
    m_mutex.unlock();
    quit();
}

void TilesLoaderThread::run()
{
    QSettings settings(QCoreApplication::applicationDirPath() + "/xtrip.ini", QSettings::IniFormat);
    QPixmapCache::setCacheLimit(settings.value("CacheLimit").toInt());

    m_manager = new QNetworkAccessManager();
    connect(m_manager,
            SIGNAL(finished(QNetworkReply *)),
            SLOT(handleNetworkData(QNetworkReply *)),
            Qt::DirectConnection);

    while (!m_abort) {
        m_mutex.lock();
        TilesSource *source = m_source;
        QRect bounds = m_bounds;
        int zoom = m_zoom;
        m_restart = false;
        m_mutex.unlock();

        QMap<int, QPoint> tiles;
        QPoint center = bounds.center();
        QString key;
        for (int x = bounds.left(); x <= bounds.right(); x++) {
            for (int y = bounds.top(); y <= bounds.bottom(); y++) {
                QPoint tp(x, y);
                key = QString("tile.%1.%2.%3.%4")
                        .arg(source->id()).arg(zoom).arg(tp.x()).arg(tp.y());
                if (!QPixmapCache::find(key)) {
                    QPoint p = tp - center;
                    tiles.insertMulti(p.x() * p.x() + p.y() * p.y(), tp);
                }
            }
        }

        if (m_restart) {
            continue;
        }

        QDateTime expiration = QDateTime::currentDateTime().addDays(-m_tileLifetime);
        foreach (QPoint tp, tiles) {
            QFile file(tilePath(source->dirname(), zoom, tp, source->format()));
            if (file.open(QIODevice::ReadOnly)) {
                QByteArray buf = file.readAll();
                file.close();
                QPixmap tile;
                if (tile.loadFromData(buf)) {
                    key = QString("tile.%1.%2.%3.%4")
                            .arg(source->id()).arg(zoom).arg(tp.x()).arg(tp.y());
                    if (QPixmapCache::insert(key, tile)) {
                        emit tileLoaded(tp);
                    }
                    QFileInfo info(file);
                    if (info.lastModified() >= expiration) {
                        continue;
                    }
                }
            }

            QUrl url = QUrl(source->tileUrl(tp.x(), tp.y(), zoom));
            QNetworkRequest request;
            request.setUrl(url);
            QHash<QString, QVariant> data;
            data["tp"] = tp;
            data["key"] = QString("tile.%1.%2.%3.%4")
                    .arg(source->id()).arg(zoom).arg(tp.x()).arg(tp.y());
            data["dirname"] = dirPath(source->dirname(), zoom, tp);
            data["filename"] = tilePath(source->dirname(), zoom, tp, source->format());
            request.setAttribute(QNetworkRequest::User, QVariant(data));
            m_mutex.lock();
            m_tilesQueue.enqueue(request);
            m_mutex.unlock();

            if (m_restart) {
                break;
            }
        }

        if (m_restart) {
            continue;
        }

        emit tilesLoading(m_tilesQueue.count());
        loadNextTile();

        exec();
    }
}

void TilesLoaderThread::handleNetworkData(QNetworkReply *reply)
{
    QPixmap tile;
    QHash<QString, QVariant> data = reply->request().attribute(QNetworkRequest::User).toHash();
    QPoint tp = data["tp"].toPoint();
    QString key = data["key"].toString();
    QString dirname = data["dirname"].toString();
    QString filename = data["filename"].toString();

    if (!reply->error()) {
        QDir dir;
        dir.mkpath(dirname);
        QFile file(filename);
        file.open(QFile::WriteOnly);
        QByteArray buf = reply->readAll();
        file.write(buf);
        file.close();

        if (tile.loadFromData(buf)) {
            QPixmapCache::insert(key, tile);
            emit tileLoaded(tp);
        }
    }
    else {
        qDebug() << reply->errorString();
    }

    reply->deleteLater();

    emit tilesLoading(m_tilesQueue.count());
    loadNextTile();
}

void TilesLoaderThread::loadNextTile()
{
    m_mutex.lock();
    if (m_tilesQueue.isEmpty()) {
        m_mutex.unlock();
        return;
    }
    QNetworkRequest request = m_tilesQueue.dequeue();
    m_mutex.unlock();
    m_manager->get(request);
}

QString TilesLoaderThread::dirPath(const QString &source, int zoom, const QPoint &tp)
{
    return (m_tilesPath + "\\" + source + "\\z%1\\%2\\x%3\\%4")
            .arg(zoom + 1)
            .arg(tp.x() / 1024)
            .arg(tp.x())
            .arg(tp.y() / 1024);
}

QString TilesLoaderThread::tilePath(const QString &source, int zoom, const QPoint &tp, const QString &format)
{
    return (dirPath(source, zoom, tp) + "\\y%1." + format)
            .arg(tp.y());
}
