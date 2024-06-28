#include "my_color.h"

my_color::my_color() //конструктор
{


    red = 0;
    green = 0;
    blue = 0;
    alpha = 0;
}

void my_color::SetColor(int a,int r,int g,int b) //установка цвета
{

  alpha=a;
  red=r;
  green=g;
  blue=b;

}

void my_color::GetColor(int &a, int &r, int &g, int &b) //получение цвета
{
    a=alpha;
    r=red;
    g=green;
    b=blue;

}
