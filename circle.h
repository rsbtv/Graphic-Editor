#ifndef CIRCLE_H
#define CIRCLE_H

#include"tochka.h"
#include"my_pen.h"
#include"my_brush.h"
#include <QImage>
#include <QPainter>

class circle
{
public:
    circle();
    void draw(QImage &im);

    bool SetR(int new_r);
    int GetR(void);

    tochka start;

    my_pen mpen;
    my_brush mbrush;

protected:
    int rad;
};

#endif // CIRCLE_H
