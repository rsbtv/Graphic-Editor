#ifndef POLYLINE_H
#define POLYLINE_H

#include"tochka.h"
#include"my_pen.h"
#include <QImage>
#include <QPainter>


class polyline: public my_pen
{
public:
    polyline();

    void draw(QImage &im, int k);
    void SetArray(int new_array);

    tochka *array;

};

#endif // POLYLINE_H
