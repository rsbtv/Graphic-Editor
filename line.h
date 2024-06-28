#ifndef LINE_H
#define LINE_H

#include"tochka.h"
#include"my_pen.h"
#include <QImage>
#include <QPainter>



class line: public my_pen
{
public:
    line();
    void draw(QImage &im);


    tochka st;//старт
    tochka end;//конец
};

#endif // LINE_H
