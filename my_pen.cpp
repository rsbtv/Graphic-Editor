#include "my_pen.h"

my_pen::my_pen() //конструктор
{
    widht = 1;
    my_style = 0;

}

void my_pen::SetWidth(int n_w)//установка ширины
{
    widht=n_w;
}

int my_pen ::GetWidth(void)//получение ширины
{
    return widht;
}

void my_pen::SetStyle(int n_s)//установка стиля
{
     my_style=n_s;
}

int my_pen ::GetStyle(void)//получение стиля
{
    return my_style;
}
