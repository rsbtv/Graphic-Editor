#ifndef MY_PEN_H
#define MY_PEN_H

#include <my_color.h>

class my_pen : public my_color
{
public:
    my_pen();

    void SetWidth(int n_w);
    int GetWidth(void);
    void SetStyle(int n_s);
    int GetStyle(void);

protected:
    int widht;
    int my_style;
};

#endif // MY_PEN_H
