#ifndef TOCHKA_H
#define TOCHKA_H


class tochka
{
public:
    tochka();
    bool SetX(int new_x);
    bool SetY(int new_y);
    bool SetXY(int new_x,int new_y);
    int GetX(void);
    int GetY(void);
    void GetXY(int &cur_x, int &cur_y);


protected:
    int x;
    int y;
};

#endif // TOCHKA_H

