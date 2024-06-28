#include "arc.h"

arc::arc()//конструктор
{
    startA=0;
    spanA=0;
}

bool arc::SetStartA(int new_startA)//установка начального угла
{
    if ((new_startA >=-360) && (new_startA<=360))
    {
        startA = new_startA;
        return true;
    }
    else
    {
        return false;
    }
}

int arc::GetStartA()//возращение начального угла
{
    return startA;
}

bool arc::SetSpanA(int new_spanA)//установка длины  угла дуги
{
    if ((new_spanA >=-360)&&(new_spanA<=360) && (new_spanA != 0))
    {
        spanA = new_spanA;
        return true;
    }
    else
    {
        return false;
    }
}

int arc::GetSpanA()//возвращение длины угла дуги
{
    return spanA;
}

bool arc::SetAngles(int new_startA, int new_spanA)//установка начального угла и длины угла
{

    if ((new_startA >= -360) && (new_spanA >= -360) && (new_startA <= 360) && (new_spanA <= 360))
    {
        startA = new_startA;
        spanA = new_spanA;
        return true;

    }
    else
    {
        return false;
    }
}
void arc::GetAngles(int &cur_x, int &cur_y)//возвращение начального угла и длины угла
{
    cur_x=startA;
    cur_y=spanA;
}


void arc::draw(QImage &im)//рисуем дугу
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

    int x,y,rad1,rad2,spanA,startA;
    rad1=rad_el.GetR1();
    rad2=rad_el.GetR2();
    x=start.GetX();
    y=start.GetY();
    startA = GetStartA();
    spanA = GetSpanA();


    painter.drawArc(x-rad1,y-rad2,rad1*2,rad2*2,startA*16,spanA*16);
}
