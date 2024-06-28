#ifndef DIALOG_PEN_H
#define DIALOG_PEN_H

#include <QDialog>
#include "my_pen.h"
#include <QCloseEvent>
namespace Ui {
class Dialog_pen;
}

class Dialog_pen : public QDialog, public my_pen
{
    Q_OBJECT

public:
    explicit Dialog_pen(QWidget *parent = nullptr);
    ~Dialog_pen();


    void setValue(my_pen n_pen);
    my_pen m_pen;

private slots:

    void on_Slider_a_valueChanged(int value);

    void on_Slider_b_valueChanged(int value);

    void on_Slider_g_valueChanged(int value);

    void on_Slider_r_valueChanged(int value);

    void on_comboBox_penStyle_currentIndexChanged(int index);

    void on_pushButton_ok_clicked();

    void closeEvent(QCloseEvent * event);


    void on_horizontalSlider_width_valueChanged(int value);

private:
    Ui::Dialog_pen *ui;

};

#endif // DIALOG_PEN_H
