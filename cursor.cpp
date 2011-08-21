#include "cursor.h"

Cursor::Cursor(QWidget *parent) :
    QWidget(parent),
    m_coord(0, 0)
{
    hide();
    m_image.load(QCoreApplication::applicationDirPath() + "/img/cursor.png");
}

void Cursor::paintEvent(QPaintEvent *event)
{
    QPainter p;
    p.begin(this);
    p.drawPixmap(0, 0, m_image);
    p.end();
}
