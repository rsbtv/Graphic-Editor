#include "rectangle.h"

rectangle::rectangle()//конструктор
{

}

void rectangle::draw(QImage &im)//рисуем прямоугольник
{
    QPainter painter(&im);
    QPen pen;



    QColor color;
    int r,g,b,a;
    mpen.GetColor(a,r,g,b);
    color.setRgb(r,g,b,a);
    pen.setColor(color);

    pen.setWidth(mpen.GetWidth());//толщина

    pen.setStyle(Qt::PenStyle(mpen.GetStyle()));//стиль линии

    painter.setPen(pen);

    QBrush brush;
    mbrush.GetColor(a,r,g,b);
    color.setRgb(r,g,b,a);
    brush.setColor(color);

    brush.setStyle(Qt::BrushStyle(mbrush.GetStyle()));//стиль заливки
    painter.setBrush(brush);

    int x,y,w,h;
    x=top_left.GetX();
    y=top_left.GetY();
    w=botton_right.GetX() - x;
    h=botton_right.GetY() - y;

    painter.drawRect(x,y,w,h);
}
