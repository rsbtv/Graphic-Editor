#include "my_brush.h"

my_brush::my_brush() //конструктор
{
    style = 1;
}

void my_brush::SetStyle(int n_s) // устанавливаем стиль
{
    style = n_s;
}


int my_brush::GetStyle(void) // вызываем стиль
{
    return(style);
}
