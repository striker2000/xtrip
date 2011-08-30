#ifndef LATLON_H
#define LATLON_H

#include <QRegExp>
#include <QString>
#include <QtGlobal>

class LatLon
{
public:
    LatLon(qreal latitude, qreal longitude);
    LatLon(const QString &str);
    void setValue(qreal latitude, qreal longitude);
    qreal lat();
    qreal lon();

private:
    qreal m_latitude;
    qreal m_longitude;
};

#endif // LATLON_H
