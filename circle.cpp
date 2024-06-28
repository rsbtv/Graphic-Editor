#include "circle.h"

circle::circle()//конструктор
{
    rad = 0;
}

bool circle::SetR(int new_r)//установка радиуса
{
    if (new_r>0)
    {
        rad = new_r;
        return true;
    }
    else
    {
        return false;
    }

}

int circle::GetR()//возвращение радиуса
{
    return(rad);
}

void circle::draw(QImage &im)//рисуем круг
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

    int x,y,rad;
    rad=GetR();
    x=start.GetX();
    y=start.GetY();


    painter.drawEllipse(x-rad,y-rad,rad*2,rad*2);
}
