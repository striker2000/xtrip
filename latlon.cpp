#include "latlon.h"

LatLon::LatLon(qreal latitude, qreal longitude)
{
    m_latitude = latitude;
    m_longitude = longitude;
}

LatLon::LatLon(const QString &str)
{
    QRegExp re;

    re.setPattern("([NS]?)\\D*(\\d+)\\D+(\\d+)\\D+(\\d+[.,]\\d+)\\D*([EW]?)\\D*(\\d+)\\D+(\\d+)\\D+(\\d+[.,]\\d+)");
    if (re.indexIn(str) >= 0) {
        m_latitude = re.cap(2).toInt() + re.cap(3).toDouble() / 60 + re.cap(4).toDouble() / 3600;
        if (re.cap(1) == "S") m_latitude = -m_latitude;
        m_longitude = re.cap(6).toInt() + re.cap(7).toDouble() / 60 + re.cap(8).toDouble() / 3600;
        if (re.cap(5) == "W") m_longitude = -m_longitude;
        return;
    }

    re.setPattern("([NS]?)\\D*(\\d+)\\D+(\\d+[.,]\\d+)\\D*([EW]?)\\D*(\\d+)\\D+(\\d+[.,]\\d+)");
    if (re.indexIn(str) >= 0) {
        m_latitude = re.cap(2).toInt() + re.cap(3).toDouble() / 60;
        if (re.cap(1) == "S") m_latitude = -m_latitude;
        m_longitude = re.cap(5).toInt() + re.cap(6).toDouble() / 60;
        if (re.cap(4) == "W") m_longitude = -m_longitude;
        return;
    }

    re.setPattern("([NS]?)\\D*(\\d+[.,]\\d+)\\D*([EW]?)\\D*(\\d+[.,]\\d+)");
    if (re.indexIn(str) >= 0) {
        m_latitude = re.cap(2).toDouble();
        if (re.cap(1) == "S") m_latitude = -m_latitude;
        m_longitude = re.cap(4).toDouble();
        if (re.cap(3) == "W") m_longitude = -m_longitude;
        return;
    }

    m_latitude = m_longitude = 0;
}

void LatLon::setValue(qreal latitude, qreal longitude)
{
    m_latitude = latitude;
    m_longitude = longitude;
}

qreal LatLon::lat()
{
    return m_latitude;
}

qreal LatLon::lon()
{
    return m_longitude;
}
