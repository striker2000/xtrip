#ifndef TILESSOURCE_H
#define TILESSOURCE_H

class TilesSource
{
public:
    virtual int id() = 0;
    virtual QString dirname() = 0;
    virtual QString format() = 0;
    virtual QString tileUrl(int x, int y, int zoom) = 0;

protected:
    QString m_url;
};

#endif // TILESSOURCE_H
