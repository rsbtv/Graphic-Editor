#ifndef POLYGON_H
#define POLYGON_H

#include"tochka.h"
#include"my_pen.h"
#include"my_brush.h"
#include <QImage>
#include <QPainter>


class polygon
{
public:
    polygon();

    void draw(QImage &im, int k);
    void SetArray(int new_array);

    tochka *array;

    my_pen mpen;
    my_brush mbrush;

};

#endif // POLYGON_H
