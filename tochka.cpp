#include "tochka.h"


tochka::tochka()//конструктор
{
    x=0;
    y=0;

}
//сеттеры (нужна проверка ;-) )
bool tochka::SetX(int new_x) //устанавливаем х
{
    if (new_x >= 0)
    {
        x = new_x;
        return true;

    }
    else
    {
        return false;
    }
}
bool tochka::SetY(int new_y)//устанавливаем у
{
    if (new_y >= 0)
    {
        y = new_y;
        return true;
    }
    else
    {
        return false;
    }
}
bool tochka::SetXY(int new_x, int new_y)//устанавливаем х и у
{

    if ((new_x >= 0) && (new_y >= 0))
    {
        x = new_x;
        y = new_y;
        return true;

    }
    else
    {
        return false;
    }
}

//геттеры
int tochka::GetX()//получаем х
{
    return(x);
}
int tochka::GetY()//получаем у
{
    return(y);
}

void tochka::GetXY(int &cur_x, int &cur_y)//получаем х и у
{
    cur_x=x;
    cur_y=y;
}
