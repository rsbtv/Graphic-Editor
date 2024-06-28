#include "dialog_points.h"
#include "ui_dialog_points.h"

Dialog_points::Dialog_points(QWidget *parent) ://конструктор
    QDialog(parent),
    ui(new Ui::Dialog_points)
{
    ui->setupUi(this);
    this->setWindowTitle("Координаты точек");

    is_ok=false;
    no_auto_change=true;
}

Dialog_points::~Dialog_points()//деструктор
{
    delete ui;
}

void Dialog_points::on_spinBox_valueChanged(int arg1)//изменили значение спинбокса
{
    ui->tableWidget->setRowCount(arg1);
}

bool Dialog_points::setValue(int kol, int *mas)//установка значение
{
    is_ok=false;

    if (mas!=NULL)
    {

        ui->spinBox->setValue(kol);
        for (int i=0;i<kol;i++)
        {
            if (ui->tableWidget->item(i,0)==NULL)
            {
                QTableWidgetItem * ti = new QTableWidgetItem;
                ui->tableWidget->setItem(i,0,ti);
            }
            ui->tableWidget->item(i,0)->setText(QString::number(mas[i]));

            if (ui->tableWidget->item(i,1)==NULL)
            {
                QTableWidgetItem * ti = new QTableWidgetItem;
                ui->tableWidget->setItem(i,1,ti);
            }
            ui->tableWidget->item(i,1)->setText(QString::number(mas[i+kol]));

        }
    }

    return(true);
}
bool Dialog_points::getValue(int &kol, int *&mas)//возвращение значений
{
    if(!is_ok)
        return(false);
    kol = ui->spinBox->value();

    mas = new int [kol * 2];

    for (int i = 0; i < kol; i++)
    {
        mas[i] =  ui->tableWidget->item(i,0)->text().toInt();
        mas[i + kol] = ui->tableWidget->item(i, 1)->text().toInt();
    }

    return(true);

}

void Dialog_points::on_pushButton_cancel_clicked()//кнопка отмена
{
    is_ok=false;
    close();
}

void Dialog_points::on_pushButton_set_clicked()//кнопка ок
{
    no_auto_change = false;

    int size = ui->tableWidget->rowCount();
    bool flag; // признак успешного преоб-я ячейки
    bool gl_flag = true; // признак успешного формирования массива
    bool flag_error = false; // признак ошибочности формирования ячейки



    // форм-е массива
    for (int i = 0; i < size; i++)
    {
        if (ui->tableWidget->item(i, 0) == nullptr) // проверка существования ячейки
        {
            if (!flag_error)
            {
                ui->tableWidget->selectRow(i);
                ui->tableWidget->scrollToItem(ui->tableWidget->item(i, 0));
            }
            flag_error = true;

            gl_flag = false; // если ячейка без значений
            QTableWidgetItem * ti; // создание указателя
            ti = new QTableWidgetItem; // выделение памяти

            ui->tableWidget->setItem(i, 0, ti); // поместили ячейку в таблицу
            ui->tableWidget->item(i, 0)->setBackground(Qt::red);
        }
        else // если существует
        {


             ui->tableWidget->item(i, 0)->text().toInt(&flag);

            if (flag)
            {
                ui->tableWidget->item(i, 0)->setBackground(Qt::white);
            }
            else
            {
                if (!flag_error)
                {
                    ui->tableWidget->selectRow(i);
                    ui->tableWidget->scrollToItem(ui->tableWidget->item(i, 0));
                }
                flag_error = true;

                gl_flag = false; // если ячейка без значений
                ui->tableWidget->item(i, 0)->setBackground(Qt::red);
            }
        }


        if (ui->tableWidget->item(i, 1) == nullptr) // проверка существования ячейки
        {
            if (!flag_error)
            {
                ui->tableWidget->selectRow(i);
                ui->tableWidget->scrollToItem(ui->tableWidget->item(i, 1));
            }
            flag_error = true;

            gl_flag = false; // если ячейка без значений
            QTableWidgetItem * ti; // создание указателя
            ti = new QTableWidgetItem; // выделение памяти

            ui->tableWidget->setItem(i, 1, ti); // поместили ячейку в таблицу
            ui->tableWidget->item(i, 1)->setBackground(Qt::red);
        }
        else // если существует
        {


            ui->tableWidget->item(i, 1)->text().toInt(&flag);

            if (flag)
            {
                ui->tableWidget->item(i, 1)->setBackground(Qt::white);
            }
            else
            {
                if (!flag_error)
                {
                    ui->tableWidget->selectRow(i);
                    ui->tableWidget->scrollToItem(ui->tableWidget->item(i, 1));
                }
                flag_error = true;

                gl_flag = false; // если ячейка без значений
                ui->tableWidget->item(i, 1)->setBackground(Qt::red);
            }
        }
    }

    if (gl_flag) // если массив сформирован
    {
        is_ok=true;
        close();
    }
    else // если что-то не так
    {
        is_ok=false;
        QMessageBox::information(this,"Ошибка","Таблица содержит некорректные значения", QMessageBox::Ok);
    }
    no_auto_change = true;
}


void Dialog_points::on_pushButton_AddRow_clicked()//добавить строку
{
    ui->spinBox->setValue(ui->spinBox->value()+1);
}


void Dialog_points::on_pushButton_DelRow_clicked()//удалить строку
{
    ui->spinBox->setValue(ui->spinBox->value()-1);
}


void Dialog_points::on_tableWidget_cellChanged(int row, int column)//изменили ячейку
{
    if (no_auto_change)
    {
        bool flag;

         ui->tableWidget->item(row, column)->text().toInt(&flag);

        if (flag) // если значение в клетке конвертировалось в Int
            ui->tableWidget->item(row, column)->setBackground(Qt::white);
        else // если значение в клетке не конвертировалось в Int
            ui->tableWidget->item(row, column)->setBackground(Qt::red);

    }
}


void Dialog_points::on_pushButton_rand_clicked() //нажата кнопка со случайными значениями
{
    int row = ui->tableWidget->rowCount();
    int val;


    for (int i=0; i < row; i++)
    {
        if (ui->tableWidget->item(i,0) == nullptr) //проверка существования ячейки
        {//не существует
            //создаем ячейку
            QTableWidgetItem * ti; //создали указатель
            ti = new QTableWidgetItem; //выделили память
            ui->tableWidget->setItem(i, 0, ti); //поместили ячейку в таблицу
        }
        if (ui->tableWidget->item(i,1) == nullptr) //проверка существования ячейки
        {//не существует
            //создаем ячейку
            QTableWidgetItem * ti; //создали указатель
            ti = new QTableWidgetItem; //выделили память
            ui->tableWidget->setItem(i, 1, ti); //поместили ячейку в таблицу
        }
        val = rand()%700;
        ui->tableWidget->item(i, 0)->setBackground(Qt::white);
        ui->tableWidget->item(i, 0)->setText(QString::number(val));
        val = rand()%500;
        ui->tableWidget->item(i, 1)->setBackground(Qt::white);
        ui->tableWidget->item(i, 1)->setText(QString::number(val));
    }
}

