#ifndef MY_COLOR_H
#define MY_COLOR_H


class my_color
{
public:
    my_color();

    void SetColor(int a,int r,int g,int b);
    void GetColor(int &a,int &r,int &g,int &b);

protected:
    int red;
    int green;
    int blue;
    int alpha;


};



#endif // MY_COLOR_H
