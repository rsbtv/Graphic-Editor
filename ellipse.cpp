#include "ellipse.h"

ellipse::ellipse()//конструктор
{
  rad1=0;
  rad2=0;
}

bool ellipse::SetR1(int new_r1)
{

    if (new_r1>0)
    {
        rad1 = new_r1;
        return true;
    }
    else
        return false;
}

bool ellipse::SetR2(int new_r2)
{
    if (new_r2>0)
    {
        rad2 = new_r2;
        return true;
    }
    else
        return false;
}

bool ellipse::SetR(int new_r1, int new_r2)
{

    if ((new_r1 > 0) && (new_r2 >0))
    {
        rad1 = new_r1;
        rad2 = new_r2;
        return true;

    }
    else
    {
        return false;
    }
}
void ellipse::GetR(int &cur_x, int &cur_y)
{
    cur_x=rad1;
    cur_y=rad2;
}

int ellipse::GetR1()
{
    return(rad1);
}

int ellipse::GetR2()
{
    return(rad2);
}

void ellipse::draw(QImage &im)
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

    pen.setStyle(Qt::PenStyle(mpen.GetStyle()));//стиль линиии

    painter.setPen(pen);

    QBrush brush;
    mbrush.GetColor(a,r,g,b);
    color.setRgb(r,g,b,a);
    brush.setColor(color);

    brush.setStyle(Qt::BrushStyle(mbrush.GetStyle()));//стиль заливки
    painter.setBrush(brush);

    int x,y,rad1,rad2;
    rad1=GetR1();
    rad2=GetR2();
    x=start.GetX();
    y=start.GetY();


    painter.drawEllipse(x-rad1,y-rad2,rad1*2,rad2*2);
}
