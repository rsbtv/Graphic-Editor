#ifndef PIXEL_H
#define PIXEL_H

#include <tochka.h>
#include <my_color.h>
#include <QImage>
#include <QPainter>

class pixel: public tochka, public my_color
{
public:
    pixel();

     void draw(QImage &im);

};

#endif // PIXEL_H
