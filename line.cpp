#include "line.h"

line::line()//конструктор
{

}

void line::draw(QImage &im)//рисуем линию
{
    QPainter painter(&im);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen;

    QColor color;

    color.setRgb(red,green,blue,alpha);
    pen.setColor(color);


    pen.setWidth(widht);//толщина
    pen.setStyle(Qt::PenStyle(my_style));//стиль линии

    painter.setPen(pen);

    painter.drawLine(st.GetX(),st.GetY(),end.GetX(),end.GetY());
}
