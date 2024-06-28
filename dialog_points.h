#ifndef DIALOG_POINTS_H
#define DIALOG_POINTS_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class Dialog_points;
}

class Dialog_points : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_points(QWidget *parent = nullptr);
    ~Dialog_points();
    bool setValue(int kol, int *mas);
    bool getValue(int &kol,int *&mas);

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_cancel_clicked();

    void on_pushButton_set_clicked();

    void on_pushButton_AddRow_clicked();

    void on_pushButton_DelRow_clicked();

    void on_tableWidget_cellChanged(int row, int column);

    void on_pushButton_rand_clicked();

private:
    Ui::Dialog_points *ui;
    bool is_ok;
    bool no_auto_change; // признак не авто ввода
};

#endif // DIALOG_POINTS_H
