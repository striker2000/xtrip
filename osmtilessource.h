#ifndef OSMTILESSOURCE_H
#define OSMTILESSOURCE_H

#include "tilessource.h"

class OSMTilesSource : public TilesSource
{
public:
    OSMTilesSource() {}
    int id() { return 1; }
    QString dirname() { return "osmmapMapnik"; }
    QString format() { return "png"; }
    QString tileUrl(int x, int y, int zoom)
    {
        return QString("http://tile.openstreetmap.org/%1/%2/%3.png")
                .arg(zoom).arg(x).arg(y);
    }
};

#endif // OSMTILESSOURCE_H
