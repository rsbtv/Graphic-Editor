#ifndef RECTANGLE_H
#define RECTANGLE_H

#include"tochka.h"
#include"my_pen.h"
#include"my_brush.h"
#include <QImage>
#include <QPainter>

class rectangle /*: public m_pen, public m_brush*/
{
public:
    rectangle();
    void draw(QImage &im);


    tochka top_left;//старт
    tochka botton_right;//конец

  //  color gran;
  // color vnutr;
    my_pen mpen;
    my_brush mbrush;

};

#endif // RECTANGLE_H
