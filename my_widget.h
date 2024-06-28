#ifndef MY_WIDGET_H
#define MY_WIDGET_H

#include <QWidget>
#include <QPainter>

class my_widget : public QWidget
{
    Q_OBJECT
public:
    explicit my_widget(QWidget *parent = nullptr);
    QImage im;
    void create_im(int SizeX, int SizeY);

signals:

protected:
    void paintEvent(QPaintEvent *);

private:
    bool im_created;

};

#endif // MY_WIDGET_H
