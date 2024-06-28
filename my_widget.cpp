#include "my_widget.h"

my_widget::my_widget(QWidget *parent)
    : QWidget{parent}
{
    im_created = false;
}

void my_widget::paintEvent(QPaintEvent *) //переопредление события
{
    if (im_created)
    {
        QPainter painter(this);
        painter.drawImage(0,0,im);
    }
}

void my_widget::create_im(int SizeX, int SizeY) //создание im по ХУ
{
    if (!im_created)
    {
        im = QImage(SizeX, SizeY, QImage::Format_ARGB32_Premultiplied);
        im_created = true;
    }
}
