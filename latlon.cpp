#include "latlon.h"

LatLon::LatLon(qreal latitude, qreal longitude)
{
    m_latitude = latitude;
    m_longitude = longitude;
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
