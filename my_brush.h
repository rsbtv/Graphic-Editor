#ifndef MY_BRUSH_H
#define MY_BRUSH_H

#include <my_color.h>


class my_brush : public my_color
{
public:
    my_brush();

    void SetStyle(int n_s);

    int GetStyle(void);

protected:
    int style;
};

#endif // MY_BRUSH_H
