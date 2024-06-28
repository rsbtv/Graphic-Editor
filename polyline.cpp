#include "polyline.h"

polyline::polyline()//конструктор
{

}

void polyline::SetArray(int new_array) // устанавливаем массив
{

    array = new tochka[new_array];
}


void polyline::draw(QImage &im, int k) // рисуем
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

    QPoint *arr;
    arr = new QPoint[k];

    for (int i = 0; i < k; i++)

    {
        arr[i].setX(array[i].GetX());
        arr[i].setY(array[i].GetY());
    }

    painter.drawPolyline(arr, k);
}
