#include "pixel.h"

pixel::pixel()//конструктор
{

}

void pixel::draw(QImage &im) //рисуем пиксель
{
    QPainter painter(&im);


    QPen pen;
    pen.setWidth(5);

    QColor color;
    color.setRgb(red,green,blue,alpha);
    pen.setColor(color);
    painter.setPen(pen);

    painter.drawPoint(x, y);
}
