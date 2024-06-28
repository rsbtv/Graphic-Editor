#include "polygon.h"

polygon::polygon()//конструктор
{

}

void polygon::SetArray(int new_array) // устанавливаем массив
{

    array = new tochka[new_array];
}


void polygon::draw(QImage &im, int k) // рисуем
{
    QPainter painter(&im);

    painter.setRenderHint(QPainter::Antialiasing);

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

    QPoint *arr;
    arr = new QPoint[k];

    for (int i = 0; i < k; i++)

    {
        arr[i].setX(array[i].GetX());
        arr[i].setY(array[i].GetY());
    }

    painter.drawPolygon(arr, k);
}
