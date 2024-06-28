#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include "dialog_pen.h"
#include<QPushButton>
#include "dialog_points.h"
#include <QFile>
#include <QMessageBox>
#include <pixel.h>
#include "dialog_pixel.h"
#include "dialog_brush.h"
#include <line.h>
#include <rectangle.h>
#include <arc.h>
#include <circle.h>
#include <ellipse.h>
#include <polyline.h>
#include <polygon.h>
#include <QFileDialog>
#include <QTableWidgetItem>


#define KOL_COL 8
#define FILE_NAME "save.dat"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT


    struct T_OBJ_DATA
    {
        my_pen mpen;
        int kol;
        int *mas;
        int cbox;
        my_brush mbrush;

    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    T_OBJ_DATA *obj_data;

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_save_clicked();

    void on_pushButton_load_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_draw_clicked();

    void on_tableWidget_cellChanged(int row, int column);


private:
    Ui::MainWindow *ui;
    bool no_auto_change; // признак не авто ввода

protected slots:
    void combo_click(int arg1);
    void color_click(void);
    void color_br_click(void);
    void color_pix_click(void);
    void point_click(void);

};
#endif // MAINWINDOW_H
