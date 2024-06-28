#ifndef ARC_H
#define ARC_H

#include"tochka.h"
#include"my_pen.h"
#include <QImage>
#include <QPainter>
#include <ellipse.h>

class arc
{
public:
    arc();
    void draw(QImage &im);

    bool SetStartA(int new_startA);
    int GetStartA(void);

    bool SetSpanA(int new_spanA);
    int GetSpanA(void);

    bool SetAngles(int new_startA,int new_spanA);
    void GetAngles(int &cur_x, int &cur_y);

    tochka start;
    ellipse rad_el;

    my_pen mpen;

protected:
    int startA;
    int spanA;
};

#endif // ARC_H
