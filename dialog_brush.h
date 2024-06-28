#ifndef DIALOG_BRUSH_H
#define DIALOG_BRUSH_H

#include <QDialog>
#include "my_brush.h"
#include <QCloseEvent>
namespace Ui {
class Dialog_brush;
}

class Dialog_brush : public QDialog, public my_brush
{
    Q_OBJECT

public:
    explicit Dialog_brush(QWidget *parent = nullptr);
    ~Dialog_brush();
    void setValue(my_brush br);
    my_brush m_brush;

private slots:

    void on_Slider_a_valueChanged(int value);

    void on_Slider_b_valueChanged(int value);

    void on_Slider_g_valueChanged(int value);

    void on_Slider_r_valueChanged(int value);

    void on_comboBox_brushStyle_currentIndexChanged(int index);

    void on_pushButton_set_clicked();

    void closeEvent(QCloseEvent * event);

private:
    Ui::Dialog_brush *ui;
};

#endif // DIALOG_BRUSH_H
