#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent)//конструктор
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->widget->create_im(ui->widget->width(),ui->widget->height());

    ui->tableWidget->setColumnCount(KOL_COL);
    ui->tableWidget->setRowCount(0);

    obj_data = nullptr;

    for (int i = 0; i < KOL_COL; i++)
    {
        ui->tableWidget->setColumnWidth(i,180);
    }

    this->setWindowTitle("Графический редактор");
}

MainWindow::~MainWindow()//деструктор
{
    delete ui;
}


void MainWindow::on_spinBox_valueChanged(int arg1) //изменили значение спинбокса
{
    no_auto_change = false;
    int need_rows=arg1*2;//на графический эллемент по 2 строки

    int cur_rows=ui->tableWidget->rowCount();

    int old_val=cur_rows/2;
    if (cur_rows<0)
        cur_rows=0;

    if (need_rows>cur_rows)//добавляем строки
    {
        ui->tableWidget->setRowCount(need_rows);
        int i = cur_rows;
        QTableWidgetItem * twi;
        QComboBox *cb;
        while (i < need_rows)
        {
            // создаем ячейки с названиями параметров
            twi = new QTableWidgetItem;
            twi->setText("Объект");
            twi->setFlags(twi->flags()&0xfffffffd);  //ячейки нельзя редактировать
            ui->tableWidget->setItem(i,0,twi);

            twi=new QTableWidgetItem;
            twi->setText("X");
            twi->setFlags(twi->flags()&0xfffffffd); //ячейки нельзя редактировать
            ui->tableWidget->setItem(i,1,twi);

            twi=new QTableWidgetItem;
            twi->setText("Y");
            twi->setFlags(twi->flags()&0xfffffffd); //ячейки нельзя редактировать
            ui->tableWidget->setItem(i,2,twi);

            twi=new QTableWidgetItem;
            twi->setText("Цвет точки");
            twi->setFlags(twi->flags()&0xfffffffd); //ячейки нельзя редактировать
            ui->tableWidget->setItem(i,3,twi);



            for(int j = 4; j < KOL_COL; j++)
            {
                twi=new QTableWidgetItem;
                twi->setText("---");
                twi->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->setItem(i,j,twi);
            }


            i++;

            cb=new QComboBox;
            cb->addItem("Пиксель");
            cb->addItem("Линия");
            cb->addItem("Прямоугольник");
            cb->addItem("Круг");
            cb->addItem("Эллипс");
            cb->addItem("Дуга");
            cb->addItem("Ломаная");
            cb->addItem("Многоугольник");

            cb->setProperty("row",i);
            //делаем коннект слотов внутри таблицы
            connect(cb,SIGNAL(activated(int)),this,SLOT(combo_click(int)));
            connect(cb,SIGNAL(currentIndexChanged(int)),this,SLOT(combo_click(int)));

            ui->tableWidget->setCellWidget(i,0,cb);

            //потому что по умолчанию выбирается режим точка
            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->setItem(i,1,twi);

            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->setItem(i,2,twi);

            QPushButton *pb = new QPushButton(this);
            pb->setText("Задать");
            pb->setProperty("row",i);
            connect(pb,SIGNAL(clicked()),this,SLOT(color_pix_click()));

            ui->tableWidget->setCellWidget(i,3,pb);

            for(int j = 4; j < KOL_COL; j++)
            {
                twi=new QTableWidgetItem;
                twi->setText("---");
                twi->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->setItem(i,j,twi);
            }


            i++;
        }
    }

    else
    {
        ui->tableWidget->setRowCount(need_rows);
    }

    if (arg1<=0) //если 0 объектов очищаем
    {
        delete [] obj_data;
        obj_data = nullptr;
    }
    else
    {
        if ((old_val == 0) || (obj_data == nullptr))
            obj_data = new T_OBJ_DATA[arg1];

        {
            if (old_val<arg1)
            {
                //добавляем;
                T_OBJ_DATA * n_obj_data=obj_data; //сохранили старое значение
                obj_data=new T_OBJ_DATA[arg1]; //выделили память под новые данные
                memcpy(obj_data,n_obj_data, old_val * sizeof(T_OBJ_DATA));
                //выше cкопировали старые данные

                delete [] n_obj_data;

                //инициализация значений

                for (int i=old_val;i<arg1;i++)
                {
                    obj_data[i].kol=0;
                    obj_data[i].mas=nullptr;
                    obj_data[i].mpen.SetColor(255,0,0,0);
                    obj_data[i].mpen.SetStyle(1);
                    obj_data[i].mpen.SetWidth(5);
                    obj_data[i].mbrush.SetColor(255,0,0,0);
                    obj_data[i].mbrush.SetStyle(1);
                    obj_data[i].cbox=0;


                    //.....
                }

            }
            else
            {
                //удалям
                T_OBJ_DATA * n_obj_data=obj_data; //сохранили старое значение
                obj_data=new T_OBJ_DATA[arg1]; //выделили память под новые данные
                memcpy(obj_data,n_obj_data, arg1 * sizeof(T_OBJ_DATA));
                //выше cкопировали старые данные

                for (int i=arg1;i<old_val;i++)
                {
                    //освобождение памяти внутри ненужных структур
                    if (n_obj_data[i].kol>0)
                        delete [] n_obj_data[i].mas;
                }
                delete [] n_obj_data;
            }
        }

    }
    no_auto_change = true;
}

void MainWindow::combo_click(int arg1) //изменили комбобокс с фигурами
{
    int row;
    no_auto_change = false;

    QComboBox *cb = qobject_cast<QComboBox*>(sender()); //относим наш комбобокс к классу комбобоксов
    QPushButton *pb;
    if (cb!=nullptr)
    {

        row=cb->property("row").toInt(); //задаем свойство row
        QTableWidgetItem * twi;

        //инициализация значений
        int indx=(row-1)/2;
        if (obj_data[indx].kol>0)
            delete [] obj_data[indx].mas;
        obj_data[indx].kol=0;
        obj_data[indx].mas=nullptr;
        obj_data[indx].mpen.SetColor(255,0,0,0);
        obj_data[indx].mpen.SetStyle(1);
        obj_data[indx].mpen.SetWidth(5);
        obj_data[indx].mbrush.SetColor(255,0,0,0);
        obj_data[indx].mbrush.SetStyle(1);
        obj_data[indx].cbox=arg1;
        //...


        switch (arg1)
        {
        case 0://точка
        {
            int rowm=row-1;

            //строка подписей

            twi=new QTableWidgetItem;
            twi->setText("X");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,1);
            ui->tableWidget->setItem(rowm,1,twi);

            twi=new QTableWidgetItem;
            twi->setText("Y");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,2);
            ui->tableWidget->setItem(rowm,2,twi);

            twi=new QTableWidgetItem;
            twi->setText("Цвет точки");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,3 );
            ui->tableWidget->setItem(rowm,3,twi);

            for(int j=4; j<KOL_COL;j++)
            {
                twi=new QTableWidgetItem;
                twi->setText("---");
                twi->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->removeCellWidget(rowm,j);
                ui->tableWidget->setItem(rowm,j,twi);
            }

            //строка значений

            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->removeCellWidget(row,1);
            ui->tableWidget->setItem(row,1,twi);

            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->removeCellWidget(row,2);
            ui->tableWidget->setItem(row,2,twi);


            pb = new QPushButton(this);
            pb->setText("Задать");
            pb->setProperty("row",row);
            connect(pb,SIGNAL(clicked()),this,SLOT(color_pix_click()));


            ui->tableWidget->removeCellWidget(row,3);
            ui->tableWidget->setCellWidget(row,3,pb);

            for(int j=4; j<KOL_COL;j++)
            {
                twi=new QTableWidgetItem;
                twi->setText("---");
                twi->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->removeCellWidget(row,j);
                ui->tableWidget->setItem(row,j,twi);
            }
            break;
        }

        case 1: //линия
        {
            int rowm=row-1;

            //строка подписей
            twi = new QTableWidgetItem;
            twi->setText("X1");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,1);
            ui->tableWidget->setItem(rowm,1,twi);

            twi = new QTableWidgetItem;
            twi->setText("Y1");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,2);
            ui->tableWidget->setItem(rowm,2,twi);

            twi = new QTableWidgetItem;
            twi->setText("X2");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,3);
            ui->tableWidget->setItem(rowm,3,twi);

            twi = new QTableWidgetItem;
            twi->setText("Y2");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,4);
            ui->tableWidget->setItem(rowm,4,twi);

            twi=new QTableWidgetItem;
            twi->setText("Параметры линии");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,5);
            ui->tableWidget->setItem(rowm,5,twi);





            for (int j=6; j<KOL_COL;j++)
            {
                twi = new QTableWidgetItem;
                twi->setText("---");
                twi->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->removeCellWidget(rowm,j);
                ui->tableWidget->setItem(rowm,j,twi);
            }


            //строка значений
            twi = new QTableWidgetItem;
            twi->setText("0"); //или убираем
            ui->tableWidget->removeCellWidget(row,1);
            ui->tableWidget->setItem(row,1,twi);

            twi = new QTableWidgetItem;
            twi->setText("0"); //или убираем
            ui->tableWidget->removeCellWidget(row,2);
            ui->tableWidget->setItem(row,2,twi);

            twi = new QTableWidgetItem;
            twi->setText("0"); //или убираем
            ui->tableWidget->removeCellWidget(row,3);
            ui->tableWidget->setItem(row,3,twi);

            twi = new QTableWidgetItem;
            twi->setText("0"); //или убираем
            ui->tableWidget->removeCellWidget(row,4);
            ui->tableWidget->setItem(row,4,twi);

            pb = new QPushButton(this);
            pb->setText("Задать");
            pb->setProperty("row",row);
            connect(pb,SIGNAL(clicked()),this,SLOT(color_click()));


            ui->tableWidget->removeCellWidget(row,5);
            ui->tableWidget->setCellWidget(row,5,pb);

            for (int j=6; j<KOL_COL;j++)
            {
                twi = new QTableWidgetItem;
                twi->setText("---");
                twi->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->removeCellWidget(row,j);
                ui->tableWidget->setItem(row,j,twi);
            }

            break;
        }

        case 2://прямоугольник
        {
            int rowm=row-1;

            //строка подписей

            twi=new QTableWidgetItem;
            twi->setText("X1");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,1);
            ui->tableWidget->setItem(rowm,1,twi);

            twi=new QTableWidgetItem;
            twi->setText("Y1");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,2);
            ui->tableWidget->setItem(rowm,2,twi);

            twi=new QTableWidgetItem;
            twi->setText("X2");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,3);
            ui->tableWidget->setItem(rowm,3,twi);

            twi=new QTableWidgetItem;
            twi->setText("Y2");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,4);
            ui->tableWidget->setItem(rowm,4,twi);

            twi = new QTableWidgetItem;
            twi->setText("Параметры границы");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,5);
            ui->tableWidget->setItem(rowm,5,twi);

            twi = new QTableWidgetItem;
            twi->setText("Параметры заливки");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,6);
            ui->tableWidget->setItem(rowm,6,twi);

            for(int j=7; j<KOL_COL;j++)
            {
                twi=new QTableWidgetItem;
                twi->setText("---");
                twi->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->removeCellWidget(rowm,j);
                ui->tableWidget->setItem(rowm,j,twi);
            }

            //строка значений


            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->removeCellWidget(row,1);
            ui->tableWidget->setItem(row,1,twi);

            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->removeCellWidget(row,2);
            ui->tableWidget->setItem(row,2,twi);


            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->removeCellWidget(row,3);
            ui->tableWidget->setItem(row,3,twi);

            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->removeCellWidget(row,4);
            ui->tableWidget->setItem(row,4,twi);

            QPushButton * pb = new QPushButton(this);
            pb->setText("Задать");
            pb->setProperty("row",row);
            connect(pb,SIGNAL(clicked()),this,SLOT(color_click()));


            ui->tableWidget->removeCellWidget(row,5);
            ui->tableWidget->setCellWidget(row,5,pb);

            pb = new QPushButton(this);
            pb->setText("Задать");
            pb->setProperty("row",row);
            connect(pb,SIGNAL(clicked()),this,SLOT(color_br_click()));



            ui->tableWidget->removeCellWidget(row,6);
            ui->tableWidget->setCellWidget(row,6,pb);


            for(int j=7; j<KOL_COL;j++)
            {
                twi=new QTableWidgetItem;
                twi->setText("---");
                twi->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->removeCellWidget(row,j);
                ui->tableWidget->setItem(row,j,twi);
            }

            break;
        }

        case 3://Круг
        {
            int rowm=row-1;

            //строка подписей

            twi=new QTableWidgetItem;
            twi->setText("X");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,1);
            ui->tableWidget->setItem(rowm,1,twi);

            twi=new QTableWidgetItem;
            twi->setText("Y");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,2);
            ui->tableWidget->setItem(rowm,2,twi);

            twi=new QTableWidgetItem;
            twi->setText("R");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,3);
            ui->tableWidget->setItem(rowm,3,twi);

            twi = new QTableWidgetItem;
            twi->setText("Параметры границы");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,4);
            ui->tableWidget->setItem(rowm,4,twi);

            twi = new QTableWidgetItem;
            twi->setText("Параметры заливки");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,5);
            ui->tableWidget->setItem(rowm,5,twi);

            for(int j=6; j<KOL_COL;j++)
            {
                twi=new QTableWidgetItem;
                twi->setText("---");
                twi->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->removeCellWidget(rowm,j);
                ui->tableWidget->setItem(rowm,j,twi);
            }

            //строка значений

            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->removeCellWidget(row,1);
            ui->tableWidget->setItem(row,1,twi);

            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->removeCellWidget(row,2);
            ui->tableWidget->setItem(row,2,twi);


            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->removeCellWidget(row,3);
            ui->tableWidget->setItem(row,3,twi);


            QPushButton * pb = new QPushButton(this);
            pb->setText("Задать");
            pb->setProperty("row",row);
            connect(pb,SIGNAL(clicked()),this,SLOT(color_click()));



            ui->tableWidget->removeCellWidget(row,4);
            ui->tableWidget->setCellWidget(row,4,pb);

            pb = new QPushButton(this);
            pb->setText("Задать");
            pb->setProperty("row",row);
            connect(pb,SIGNAL(clicked()),this,SLOT(color_br_click()));



            ui->tableWidget->removeCellWidget(row,5);
            ui->tableWidget->setCellWidget(row,5,pb);


            for(int j=6; j<KOL_COL;j++)
            {
                twi=new QTableWidgetItem;
                twi->setText("---");
                twi->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->removeCellWidget(row,j);
                ui->tableWidget->setItem(row,j,twi);
            }

            break;
        }

        case 4://эллипс
        {
            int rowm=row-1;

            //строка подписей

            twi=new QTableWidgetItem;
            twi->setText("X");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,1);
            ui->tableWidget->setItem(rowm,1,twi);

            twi=new QTableWidgetItem;
            twi->setText("Y");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,2);
            ui->tableWidget->setItem(rowm,2,twi);

            twi=new QTableWidgetItem;
            twi->setText("R1");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,3);
            ui->tableWidget->setItem(rowm,3,twi);

            twi=new QTableWidgetItem;
            twi->setText("R2");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,4);
            ui->tableWidget->setItem(rowm,4,twi);

            twi = new QTableWidgetItem;
            twi->setText("Параметры границы");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,5);
            ui->tableWidget->setItem(rowm,5,twi);

            twi = new QTableWidgetItem;
            twi->setText("Параметры заливки");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,6);
            ui->tableWidget->setItem(rowm,6,twi);

            for(int j=7; j<KOL_COL;j++)
            {
                twi=new QTableWidgetItem;
                twi->setText("---");
                twi->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->removeCellWidget(rowm,j);
                ui->tableWidget->setItem(rowm,j,twi);
            }

            //строка значений


            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->removeCellWidget(row,1);
            ui->tableWidget->setItem(row,1,twi);

            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->removeCellWidget(row,2);
            ui->tableWidget->setItem(row,2,twi);


            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->removeCellWidget(row,3);
            ui->tableWidget->setItem(row,3,twi);

            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->removeCellWidget(row,4);
            ui->tableWidget->setItem(row,4,twi);

            QPushButton * pb = new QPushButton(this);
            pb->setText("Задать");
            pb->setProperty("row",row);
            connect(pb,SIGNAL(clicked()),this,SLOT(color_click()));



            ui->tableWidget->removeCellWidget(row,5);
            ui->tableWidget->setCellWidget(row,5,pb);

            pb = new QPushButton(this);
            pb->setText("Задать");
            pb->setProperty("row",row);
            connect(pb,SIGNAL(clicked()),this,SLOT(color_br_click()));



            ui->tableWidget->removeCellWidget(row,6);
            ui->tableWidget->setCellWidget(row,6,pb);


            for(int j=7; j<KOL_COL;j++)
            {
                twi=new QTableWidgetItem;
                twi->setText("---");
                twi->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->removeCellWidget(row,j);
                ui->tableWidget->setItem(row,j,twi);
            }

            break;
        }

        case 5://дуга
        {
            int rowm=row-1;

            //строка подписей

            twi=new QTableWidgetItem;
            twi->setText("X");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,1);
            ui->tableWidget->setItem(rowm,1,twi);

            twi=new QTableWidgetItem;
            twi->setText("Y");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,2);
            ui->tableWidget->setItem(rowm,2,twi);

            twi=new QTableWidgetItem;
            twi->setText("R1");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,3);
            ui->tableWidget->setItem(rowm,3,twi);

            twi=new QTableWidgetItem;
            twi->setText("R2");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,4);
            ui->tableWidget->setItem(rowm,4,twi);

            twi=new QTableWidgetItem;
            twi->setText("Начальный угол,°");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,5);
            ui->tableWidget->setItem(rowm,5,twi);

            twi=new QTableWidgetItem;
            twi->setText("Угловая длина,°");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,6);
            ui->tableWidget->setItem(rowm,6,twi);


            twi = new QTableWidgetItem;
            twi->setText("Параметры линии");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,7);
            ui->tableWidget->setItem(rowm,7,twi);


            for(int j=8; j<KOL_COL;j++)
            {
                twi=new QTableWidgetItem;
                twi->setText("---");
                twi->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->removeCellWidget(rowm,j);
                ui->tableWidget->setItem(rowm,j,twi);
            }

            //строка значений


            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->removeCellWidget(row,1);
            ui->tableWidget->setItem(row,1,twi);

            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->removeCellWidget(row,2);
            ui->tableWidget->setItem(row,2,twi);


            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->removeCellWidget(row,3);
            ui->tableWidget->setItem(row,3,twi);

            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->removeCellWidget(row,4);
            ui->tableWidget->setItem(row,4,twi);

            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->removeCellWidget(row,5);
            ui->tableWidget->setItem(row,5,twi);

            twi=new QTableWidgetItem;
            twi->setText("0");//или убираем
            ui->tableWidget->removeCellWidget(row,6);
            ui->tableWidget->setItem(row,6,twi);

            QPushButton * pb = new QPushButton(this);
            pb->setText("Задать");
            pb->setProperty("row",row);
            connect(pb,SIGNAL(clicked()),this,SLOT(color_click()));



            ui->tableWidget->removeCellWidget(row,7);
            ui->tableWidget->setCellWidget(row,7,pb);

            for(int j=8; j<KOL_COL;j++)
            {
                twi=new QTableWidgetItem;
                twi->setText("---");
                twi->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->removeCellWidget(row,j);
                ui->tableWidget->setItem(row,j,twi);
            }

            break;
        }

        case 6: //ломаная
        {
            int rowm=row-1;

            //строка подписей

            twi=new QTableWidgetItem;
            twi->setText("Параметры линии");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,1);
            ui->tableWidget->setItem(rowm,1,twi);

            twi=new QTableWidgetItem;
            twi->setText("Массив точек");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,2);
            ui->tableWidget->setItem(rowm,2,twi);



            for (int j=3; j<KOL_COL;j++)
            {
                twi = new QTableWidgetItem;
                twi->setText("---");
                twi->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->removeCellWidget(rowm,j);
                ui->tableWidget->setItem(rowm,j,twi);
            }


            //строка значений


            QPushButton *pb = new QPushButton(this);
            pb->setText("Задать");
            pb->setProperty("row",row);
            connect(pb,SIGNAL(clicked()),this,SLOT(color_click()));



            ui->tableWidget->removeCellWidget(row,1);
            ui->tableWidget->setCellWidget(row,1,pb);


            pb = new QPushButton(this);
            pb->setText("Точки");
            pb->setProperty("row",row);
            connect(pb,SIGNAL(clicked()),this,SLOT(point_click()));



            ui->tableWidget->removeCellWidget(row,2);
            ui->tableWidget->setCellWidget(row,2,pb);

            for (int j=3; j<KOL_COL;j++)
            {
                twi = new QTableWidgetItem;
                twi->setText("---");
                twi->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->removeCellWidget(row,j);
                ui->tableWidget->setItem(row,j,twi);
            }

            break;
        }

        case 7: //многоугольник
        {
            int rowm=row-1;

            //строка подписей

            twi=new QTableWidgetItem;
            twi->setText("Параметры границы");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,1);
            ui->tableWidget->setItem(rowm,1,twi);

            twi=new QTableWidgetItem;
            twi->setText("Параметры заливки");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,2);
            ui->tableWidget->setItem(rowm,2,twi);



            twi=new QTableWidgetItem;
            twi->setText("Массив точек");
            twi->setFlags(twi->flags()&0xfffffffd);
            ui->tableWidget->removeCellWidget(rowm,3);
            ui->tableWidget->setItem(rowm,3,twi);



            for (int j=4; j<KOL_COL;j++)
            {
                twi = new QTableWidgetItem;
                twi->setText("---");
                twi->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->removeCellWidget(rowm,j);
                ui->tableWidget->setItem(rowm,j,twi);
            }


            //строка значений


            QPushButton *pb = new QPushButton(this);
            pb->setText("Задать");
            pb->setProperty("row",row);
            connect(pb,SIGNAL(clicked()),this,SLOT(color_click()));



            ui->tableWidget->removeCellWidget(row,1);
            ui->tableWidget->setCellWidget(row,1,pb);

            pb = new QPushButton(this);
            pb->setText("Задать");
            pb->setProperty("row",row);
            connect(pb,SIGNAL(clicked()),this,SLOT(color_br_click()));



            ui->tableWidget->removeCellWidget(row,2);
            ui->tableWidget->setCellWidget(row,2,pb);


            pb = new QPushButton(this);
            pb->setText("Точки");
            pb->setProperty("row",row);
            connect(pb,SIGNAL(clicked()),this,SLOT(point_click()));



            ui->tableWidget->removeCellWidget(row,3);
            ui->tableWidget->setCellWidget(row,3,pb);

            for (int j=4; j<KOL_COL;j++)
            {
                twi = new QTableWidgetItem;
                twi->setText("---");
                twi->setFlags(Qt::ItemIsEditable);
                ui->tableWidget->removeCellWidget(row,j);
                ui->tableWidget->setItem(row,j,twi);
            }

            break;
        }


        }

    }
    no_auto_change = true;

}

void MainWindow::color_click()//кнопка выбора параметров линии(границы)
{
    QPushButton *pb=qobject_cast<QPushButton*>(sender());
    int row=pb->property("row").toInt();

    int indx=(row-1)/2;
    Dialog_pen dial;

    my_pen pen;
    //получить предыдущие значения стиля

    int a,r,g,b;
    dial.setValue(obj_data[indx].mpen);



    dial.exec();
    //сохранить значения из dial

    //dial->GetColor(___);


    dial.GetColor(a,r,g,b);


    obj_data[indx].mpen.SetColor(a,r,g,b);
    obj_data[indx].mpen.SetStyle(dial.m_pen.GetStyle());
    obj_data[indx].mpen.SetWidth(dial.m_pen.GetWidth());


}

void MainWindow::color_br_click()//кнопка выбора параметров заливки
{
    QPushButton *pb=qobject_cast<QPushButton*>(sender());
    int row=pb->property("row").toInt();

    int indx=(row-1)/2;
    Dialog_brush dial;

    my_brush brush;
    int a,r,g,b;
    dial.setValue(obj_data[indx].mbrush);

    dial.exec();
    //сохранить значения из dial

    //dial->GetColor(___);


    dial.GetColor(a,r,g,b);

    obj_data[indx].mbrush.SetColor(a,r,g,b);
    obj_data[indx].mbrush.SetStyle(dial.m_brush.GetStyle());




}

void MainWindow::color_pix_click()//кнопка выбора цвета для точки
{
    QPushButton *pb=qobject_cast<QPushButton*>(sender());
    int row=pb->property("row").toInt();

    int indx=(row-1)/2;
    Dialog_pixel dial;

    my_pen pen;
    //получить предыдущие значения стиля

    int a,r,g,b;

    dial.SetValue(obj_data[indx].mpen);



    dial.exec();
    //сохранить значения из dial

    //dial->GetColor(___);


    dial.GetColor(a,r,g,b);

    obj_data[indx].mpen.SetColor(a,r,g,b);


}

void MainWindow::point_click()//кнопка для точек
{
    QPushButton *pb=qobject_cast<QPushButton*>(sender());
    int row=pb->property("row").toInt();
    int indx=(row-1)/2;

    Dialog_points dial;

    //получить предыдущие значения стиля

    dial.setValue(obj_data[indx].kol,obj_data[indx].mas);

    //    dial.setValue(pen);


    dial.exec();
    //сохранить значения из dial

    dial.getValue(obj_data[indx].kol,obj_data[indx].mas);



    //    delete [] dial;
}


void MainWindow::on_pushButton_save_clicked()//сохранить
{
    bool gl_flag = true; // признак успешного формирования массива
    QComboBox * cb;
    int val;
    bool fl;

    if (ui->spinBox->value() == 0)
    {
        gl_flag=false;
    }

    for (int i = 0; i < ui->spinBox->value(); i++) //рисуем все
    {

        cb = qobject_cast<QComboBox *>(ui->tableWidget->cellWidget(i*2+1, 0)); //берем комбо бокс ячейки
        val = cb->currentIndex(); //берем индекс комбо бокса

        switch (val)
        {
        case 0: //пиксель
        {
            ui->tableWidget->item(i*2+1, 1)->text().toInt(&fl);
            if (fl)
            {
                ui->tableWidget->item(i*2+1, 2)->text().toInt(&fl);
            }
            if (fl)
            {
                gl_flag=true;
            }
            else
            {
                ui->tableWidget->selectRow(i*2+1);
                ui->tableWidget->scrollToItem(ui->tableWidget->item(i*2+1, 0));

                QMessageBox::warning(this, "Ошибка", "Введены некорректные значения для точки");  //злобно (добро) ругательное сообщение
                gl_flag=false;
            }


            break;
        }
        case 1://линия
        {
            ui->tableWidget->item(i*2+1, 1)->text().toInt(&fl);
            if (fl)
            {
                ui->tableWidget->item(i*2+1, 2)->text().toInt(&fl);
            }
            if (fl)
            {
                ui->tableWidget->item(i*2+1, 3)->text().toInt(&fl);
            }
            if (fl)
            {
                ui->tableWidget->item(i*2+1, 4)->text().toInt(&fl);
            }
            if (fl)
            {
                gl_flag=true;
            }
            else
            {
                ui->tableWidget->selectRow(i*2+1);
                ui->tableWidget->scrollToItem(ui->tableWidget->item(i*2+1, 0));
                QMessageBox::warning(this, "Ошибка", "Введены некорректные значения для линии");  //злобно (добро) ругательное сообщение
                gl_flag=false;
            }


            break;

        }
        case 2://прямоугольник
        {
            ui->tableWidget->item(i*2+1, 1)->text().toInt(&fl);
            if (fl)
            {
                ui->tableWidget->item(i*2+1, 2)->text().toInt(&fl);
            }
            if (fl)
            {
                ui->tableWidget->item(i*2+1, 3)->text().toInt(&fl);
            }
            if (fl)
            {
                ui->tableWidget->item(i*2+1, 4)->text().toInt(&fl);
            }
            if (fl)
            {
                gl_flag=true;
            }
            else
            {
                ui->tableWidget->selectRow(i*2+1);
                ui->tableWidget->scrollToItem(ui->tableWidget->item(i*2+1, 0));
                QMessageBox::warning(this, "Ошибка", "Введены некорректные значения для прямоугольника");  //злобно (добро) ругательное сообщение
                gl_flag=false;
            }


            break;
        }
        case 3://круг
        {
            int n=ui->tableWidget->item(i*2+1, 1)->text().toInt(&fl);
            if (fl )
            {
                n=ui->tableWidget->item(i*2+1, 2)->text().toInt(&fl);
            }
            if (fl)
            {
                n=ui->tableWidget->item(i*2+1, 3)->text().toInt(&fl);
            }

            if (fl && n>=0)
            {
                gl_flag=true;
            }
            else
            {
                ui->tableWidget->selectRow(i*2+1);
                ui->tableWidget->scrollToItem(ui->tableWidget->item(i*2+1, 0));
                QMessageBox::warning(this, "Ошибка", "Введены некорректные значения для круга");  //злобно (добро) ругательное сообщение
                gl_flag=false;
            }


            break;
        }
        case 4://эллипс
        {


            int n=ui->tableWidget->item(i*2+1, 1)->text().toInt(&fl);
            if (fl)
            {
                n=ui->tableWidget->item(i*2+1, 2)->text().toInt(&fl);
            }
            if (fl )
            {
                n=ui->tableWidget->item(i*2+1, 3)->text().toInt(&fl);
            }
            if (fl && n>=0)
            {
                n=ui->tableWidget->item(i*2+1, 4)->text().toInt(&fl);
            }
            if (fl && n>=0)
            {
                gl_flag=true;
            }
            else
            {
                ui->tableWidget->selectRow(i*2+1);
                ui->tableWidget->scrollToItem(ui->tableWidget->item(i*2+1, 0));
                QMessageBox::warning(this, "Ошибка", "Введены некорректные значения для эллипса");  //злобно (добро) ругательное сообщение
                gl_flag=false;
            }


            break;
        }
        case 5://дуга
        {
            int n=ui->tableWidget->item(i*2+1, 1)->text().toInt(&fl);
            if (fl)
            {
                n=ui->tableWidget->item(i*2+1, 2)->text().toInt(&fl);
            }
            if (fl)
            {
                n=ui->tableWidget->item(i*2+1, 3)->text().toInt(&fl);
            }
            if (fl && n>=0)
            {
                n=ui->tableWidget->item(i*2+1, 4)->text().toInt(&fl);
            }
            if (fl && n>=0)
            {
                n=ui->tableWidget->item(i*2+1, 5)->text().toInt(&fl);
            }
            if (fl && n>=-360 && n<=360)
            {
                n=ui->tableWidget->item(i*2+1, 6)->text().toInt(&fl);
            }
            if (fl && n>=-360 && n<=360)
            {
                gl_flag=true;
            }
            else
            {
                ui->tableWidget->selectRow(i*2+1);
                ui->tableWidget->scrollToItem(ui->tableWidget->item(i*2+1, 0));
                QMessageBox::warning(this, "Ошибка", "Введены некорректные значения для дуги");  //злобно (добро) ругательное сообщение
                gl_flag=false;
            }


            break;
        }
        }

    }

    if (gl_flag) //если массив сформирован
    {
        QString filename;
        filename = QFileDialog::getSaveFileName(this, "Сохранить в формате BIN", "C:\\Users\\student\\Documents", "Binary files (*.bin)");

        if (filename.isEmpty())
        {
            QMessageBox::information(this, "Ошибка", "Файл не выбран", QMessageBox::Ok);
        }
        else
        {

            QFile file;
            char * mas = new char[sizeof(int)];
            file.setFileName(filename);
            file.open(QIODevice::WriteOnly);

            QComboBox * cb;
            int cb_val;
            int val2;
            bool flag;
            int indx;
            int size_tabl=ui->spinBox->value()*2;
            for (int i=1; i<size_tabl;i=i+2)
            {
                cb = qobject_cast<QComboBox *>(ui->tableWidget->cellWidget(i,0));
                cb_val=cb->currentIndex();
                memcpy(mas,&cb_val,sizeof(int));
                file.write(mas,sizeof(int));

                switch (cb_val)
                {
                case 0: //точка
                {
                    val2=ui->tableWidget->item(i,1)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат точки");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //сохраняем значение X
                    file.write(mas,sizeof(int));

                    val2=ui->tableWidget->item(i,2)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат точки");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); // сохраняем значение Y
                    file.write(mas,sizeof(int));

                    indx=(i-1)/2;
                    int a,r,g,b;
                    //сохраняем параметры цвета
                    obj_data[indx].mpen.GetColor(a,r,g,b);
                    memcpy(mas,&a,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&r,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&g,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&b,sizeof(int));
                    file.write(mas,sizeof(int));
                    break;
                }
                case 1: //Линия
                {
                    val2=ui->tableWidget->item(i,1)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат линии");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //X1
                    file.write(mas,sizeof(int));

                    val2=ui->tableWidget->item(i,2)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат линии");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //Y1
                    file.write(mas,sizeof(int));

                    val2=ui->tableWidget->item(i,3)->text().toInt(&flag);
                    if (!flag)
                    {
                        QMessageBox::information(this,"Ошибка","Неверные значения координат линии");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //X2
                    file.write(mas,sizeof(int));

                    val2=ui->tableWidget->item(i,4)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат линии");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //Y2
                    file.write(mas,sizeof(int));

                    indx=(i-1)/2;
                    int a,r,g,b,n_s,n_w;
                    obj_data[indx].mpen.GetColor(a,r,g,b);
                    memcpy(mas,&a,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&r,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&g,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&b,sizeof(int));
                    file.write(mas,sizeof(int));

                    n_s=obj_data[indx].mpen.GetStyle();
                    memcpy(mas,&n_s,sizeof(int));
                    file.write(mas,sizeof(int));

                    n_w=obj_data[indx].mpen.GetWidth();
                    memcpy(mas,&n_w,sizeof(int));
                    file.write(mas,sizeof(int));
                    break;
                }
                case 2:  // прямоугольник
                {
                    val2=ui->tableWidget->item(i,1)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат прямоугольника");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //X1
                    file.write(mas,sizeof(int));

                    val2=ui->tableWidget->item(i,2)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат прямоугольника");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //Y1
                    file.write(mas,sizeof(int));

                    val2=ui->tableWidget->item(i,3)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат прямоугольника");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //X2
                    file.write(mas,sizeof(int));

                    val2=ui->tableWidget->item(i,4)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат прямоугольника");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //Y2
                    file.write(mas,sizeof(int));

                    indx=(i-1)/2;
                    int a,r,g,b,n_s,n_w;
                    //цвет обводки
                    obj_data[indx].mpen.GetColor(a,r,g,b);
                    memcpy(mas,&a,sizeof(int));
                    file.write(mas,sizeof(int));

                    memcpy(mas,&r,sizeof(int));
                    file.write(mas,sizeof(int));

                    memcpy(mas,&g,sizeof(int));
                    file.write(mas,sizeof(int));

                    memcpy(mas,&b,sizeof(int));
                    file.write(mas,sizeof(int));

                    //стиль обводки
                    n_s=obj_data[indx].mpen.GetStyle();
                    memcpy(mas,&n_s,sizeof(int));
                    file.write(mas,sizeof(int));

                    n_w=obj_data[indx].mpen.GetWidth();
                    memcpy(mas,&n_w,sizeof(int));
                    file.write(mas,sizeof(int));

                    //цвет и стиль заливки
                    obj_data[indx].mbrush.GetColor(a,r,g,b);
                    memcpy(mas,&a,sizeof(int));
                    file.write(mas,sizeof(int));

                    memcpy(mas,&r,sizeof(int));
                    file.write(mas,sizeof(int));

                    memcpy(mas,&g,sizeof(int));
                    file.write(mas,sizeof(int));

                    memcpy(mas,&b,sizeof(int));
                    file.write(mas,sizeof(int));

                    n_s=obj_data[indx].mbrush.GetStyle();
                    memcpy(mas,&n_s,sizeof(int));
                    file.write(mas,sizeof(int));
                    break;
                }
                case 3: // круг
                {
                    val2=ui->tableWidget->item(i,1)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат круга");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //X1
                    file.write(mas,sizeof(int));

                    val2=ui->tableWidget->item(i,2)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат круга");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //Y1
                    file.write(mas,sizeof(int));

                    val2=ui->tableWidget->item(i,3)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат круга");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //R
                    file.write(mas,sizeof(int));

                    indx=(i-1)/2;
                    int a,r,g,b,n_s,n_w;
                    //цвет обводки
                    obj_data[indx].mpen.GetColor(a,r,g,b);
                    memcpy(mas,&a,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&r,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&g,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&b,sizeof(int));
                    file.write(mas,sizeof(int));

                    n_s=obj_data[indx].mpen.GetStyle();
                    memcpy(mas,&n_s,sizeof(int));
                    file.write(mas,sizeof(int));

                    n_w=obj_data[indx].mpen.GetWidth();
                    memcpy(mas,&n_w,sizeof(int));
                    file.write(mas,sizeof(int));

                    obj_data[indx].mbrush.GetColor(a,r,g,b);
                    memcpy(mas,&a,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&r,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&g,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&b,sizeof(int));
                    file.write(mas,sizeof(int));

                    n_s=obj_data[indx].mbrush.GetStyle();
                    memcpy(mas,&n_s,sizeof(int));
                    file.write(mas,sizeof(int));


                    break;
                }
                case 4: // эллипс
                {
                    val2=ui->tableWidget->item(i,1)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат эллипса");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //X1
                    file.write(mas,sizeof(int));

                    val2=ui->tableWidget->item(i,2)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат эллипса");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //Y1
                    file.write(mas,sizeof(int));

                    val2=ui->tableWidget->item(i,3)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат эллипса");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //X2
                    file.write(mas,sizeof(int));

                    val2=ui->tableWidget->item(i,4)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат эллипса");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //Y2
                    file.write(mas,sizeof(int));

                    indx=(i-1)/2;
                    int a,r,g,b,n_s,n_w;

                    //запись цвета обводки и заливки, их стилей
                    obj_data[indx].mpen.GetColor(a,r,g,b);
                    memcpy(mas,&a,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&r,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&g,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&b,sizeof(int));
                    file.write(mas,sizeof(int));

                    n_s=obj_data[indx].mpen.GetStyle();
                    memcpy(mas,&n_s,sizeof(int));
                    file.write(mas,sizeof(int));

                    n_w=obj_data[indx].mpen.GetWidth();
                    memcpy(mas,&n_w,sizeof(int));
                    file.write(mas,sizeof(int));

                    obj_data[indx].mbrush.GetColor(a,r,g,b);
                    memcpy(mas,&a,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&r,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&g,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&b,sizeof(int));
                    file.write(mas,sizeof(int));

                    n_s=obj_data[indx].mbrush.GetStyle();
                    memcpy(mas,&n_s,sizeof(int));
                    file.write(mas,sizeof(int));
                    break;
                }
                case 5: //дуга
                {
                    val2=ui->tableWidget->item(i,1)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат дуги");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //X1
                    file.write(mas,sizeof(int));

                    val2=ui->tableWidget->item(i,2)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат дуги");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //Y1
                    file.write(mas,sizeof(int));

                    val2=ui->tableWidget->item(i,3)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат дуги");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //X2
                    file.write(mas,sizeof(int));

                    val2=ui->tableWidget->item(i,4)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат дуги");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //Y2
                    file.write(mas,sizeof(int));

                    val2=ui->tableWidget->item(i,5)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //st angl
                    file.write(mas,sizeof(int));

                    val2=ui->tableWidget->item(i,6)->text().toInt(&flag);
                    if (!flag)
                    {
                        //QMessageBox::information(this,"Ошибка","Неверные значения координат дуги");
                        delete [] mas;
                        file.close();
                        file.remove();
                        return;
                    }
                    memcpy(mas,&val2,sizeof(int)); //sp angl
                    file.write(mas,sizeof(int));

                    indx=(i-1)/2;
                    int a,r,g,b,n_s,n_w;
                    obj_data[indx].mpen.GetColor(a,r,g,b);
                    memcpy(mas,&a,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&r,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&g,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&b,sizeof(int));
                    file.write(mas,sizeof(int));

                    n_s=obj_data[indx].mpen.GetStyle();
                    memcpy(mas,&n_s,sizeof(int));
                    file.write(mas,sizeof(int));

                    n_w=obj_data[indx].mpen.GetWidth();
                    memcpy(mas,&n_w,sizeof(int));
                    file.write(mas,sizeof(int));
                    break;
                }
                case 6: //ломаная
                {
                    indx=(i-1)/2;
                    int a,r,g,b,n_s,n_w;
                    obj_data[indx].mpen.GetColor(a,r,g,b);
                    memcpy(mas,&a,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&r,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&g,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&b,sizeof(int));
                    file.write(mas,sizeof(int));

                    n_s=obj_data[indx].mpen.GetStyle();
                    memcpy(mas,&n_s,sizeof(int));
                    file.write(mas,sizeof(int));

                    n_w=obj_data[indx].mpen.GetWidth();
                    memcpy(mas,&n_w,sizeof(int));
                    file.write(mas,sizeof(int));

                    val2=obj_data[indx].kol;
                    memcpy(mas,&val2,sizeof(int));
                    file.write(mas,sizeof(int));
                    int kol2=val2+val2;
                    for (int j=0;j<kol2;j++)
                    {
                        val2=obj_data[indx].mas[j];
                        memcpy(mas,&val2,sizeof(int));
                        file.write(mas,sizeof(int));
                    }


                    break;
                }
                case 7: // многоугольник
                {
                    indx=(i-1)/2;
                    int a,r,g,b,n_s,n_w;
                    obj_data[indx].mpen.GetColor(a,r,g,b);
                    memcpy(mas,&a,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&r,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&g,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&b,sizeof(int));
                    file.write(mas,sizeof(int));

                    n_s=obj_data[indx].mpen.GetStyle();
                    memcpy(mas,&n_s,sizeof(int));
                    file.write(mas,sizeof(int));

                    n_w=obj_data[indx].mpen.GetWidth();
                    memcpy(mas,&n_w,sizeof(int));
                    file.write(mas,sizeof(int));

                    obj_data[indx].mbrush.GetColor(a,r,g,b);
                    memcpy(mas,&a,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&r,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&g,sizeof(int));
                    file.write(mas,sizeof(int));
                    memcpy(mas,&b,sizeof(int));
                    file.write(mas,sizeof(int));

                    n_s=obj_data[indx].mbrush.GetStyle();
                    memcpy(mas,&n_s,sizeof(int));
                    file.write(mas,sizeof(int));

                    val2=obj_data[indx].kol;
                    memcpy(mas,&val2,sizeof(int));
                    file.write(mas,sizeof(int));
                    int kol2=val2+val2;
                    for (int j=0;j<kol2;j++)
                    {
                        val2=obj_data[indx].mas[j];
                        memcpy(mas,&val2,sizeof(int));
                        file.write(mas,sizeof(int));
                    }


                    break;
                }

                }
            }

            file.close();
            delete [] mas;
        }
    }
}



void MainWindow::on_pushButton_load_clicked()//загрузить
{
    QString filename;//имя файла
    filename = QFileDialog::getOpenFileName(this, "Открыть в формате BIN", "C:\\Users\\student\\Documents", "Binary files (*.bin)");

    if (filename.isEmpty())
    {
        QMessageBox::information(this, "Ошибка", "Файл не выбран.", QMessageBox::Ok);
    }
    else
    {
        QFile file;
        char * mas = new char[sizeof(int)];
        file.setFileName(filename);
        file.open(QIODevice::ReadOnly);

        int size;
        int val;
        int val2;
        int indx=0;
        int row=0;
        QComboBox * cb;
        ui->spinBox->setValue(0);
        while (!file.atEnd())
        {
            file.read(mas,sizeof(int));
            memcpy(&val,mas,size_t(sizeof(int)));

            row = indx+indx+1;
            //ui->spinBox->setValue(indx+1);
            size=indx+1;

            if (size<=0 || size>20) //если первое число указано неверно
            {
                QMessageBox::information(this,"Ошибка","Неверный формат файла1");
                file.close();
                delete [] mas;
                return;
            }
            else
            {

                if ((file.size()%4 !=0)) // проверка размеров по байтам
                {
                    QMessageBox::information(this,"Ошибка","Неверный формат файла2");
                    file.close();
                    delete [] mas;
                    return;
                }
                else
                {
                    try
                    {
                        ui->spinBox->setValue(size);
                    }
                    catch (...)
                    {
                        QMessageBox::information(this,"Ошибка","Неверный формат файла");
                        file.close();
                        delete [] mas;
                        return;
                    }
                    cb = qobject_cast<QComboBox *>(ui->tableWidget->cellWidget(row,0));

                    cb->setCurrentIndex(val);

                    switch (val)
                    {
                    case 0: //точка
                    {
                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //X
                        ui->tableWidget->item(row,1)->setText(QString::number(val2));

                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //Y
                        ui->tableWidget->item(row,2)->setText(QString::number(val2));

                        int a,r,g,b;
                        file.read(mas,sizeof(int));
                        memcpy(&a,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&r,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&g,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&b,mas,sizeof(int));
                        obj_data[indx].mpen.SetColor(a,r,g,b);
                        break;
                    }
                    case 1: //Линия
                    {
                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //X1
                        ui->tableWidget->item(row,1)->setText(QString::number(val2));

                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //Y1
                        ui->tableWidget->item(row,2)->setText(QString::number(val2));

                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //X2
                        ui->tableWidget->item(row,3)->setText(QString::number(val2));

                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //Y2
                        ui->tableWidget->item(row,4)->setText(QString::number(val2));

                        int a,r,g,b,n_s,n_w;
                        file.read(mas,sizeof(int));
                        memcpy(&a,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&r,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&g,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&b,mas,sizeof(int));
                        obj_data[indx].mpen.SetColor(a,r,g,b);

                        file.read(mas,sizeof(int));
                        memcpy(&n_s,mas,sizeof(int));
                        obj_data[indx].mpen.SetStyle(n_s);

                        file.read(mas,sizeof(int));
                        memcpy(&n_w,mas,sizeof(int));
                        obj_data[indx].mpen.SetWidth(n_w);
                        break;

                    }
                    case 2: // прямоугольник
                    {
                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //X1
                        ui->tableWidget->item(row,1)->setText(QString::number(val2));

                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //Y1
                        ui->tableWidget->item(row,2)->setText(QString::number(val2));

                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //X2
                        ui->tableWidget->item(row,3)->setText(QString::number(val2));

                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //Y2
                        ui->tableWidget->item(row,4)->setText(QString::number(val2));

                        int a,r,g,b,n_s,n_w;
                        file.read(mas,sizeof(int));
                        memcpy(&a,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&r,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&g,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&b,mas,sizeof(int));
                        obj_data[indx].mpen.SetColor(a,r,g,b);

                        file.read(mas,sizeof(int));
                        memcpy(&n_s,mas,sizeof(int));
                        obj_data[indx].mpen.SetStyle(n_s);

                        file.read(mas,sizeof(int));
                        memcpy(&n_w,mas,sizeof(int));
                        obj_data[indx].mpen.SetWidth(n_w);

                        file.read(mas,sizeof(int));
                        memcpy(&a,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&r,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&g,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&b,mas,sizeof(int));
                        obj_data[indx].mbrush.SetColor(a,r,g,b);

                        file.read(mas,sizeof(int));
                        memcpy(&n_s,mas,sizeof(int));
                        obj_data[indx].mbrush.SetStyle(n_s);
                        break;
                    }
                    case 3: // круг
                    {
                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //X1
                        ui->tableWidget->item(row,1)->setText(QString::number(val2));

                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //Y1
                        ui->tableWidget->item(row,2)->setText(QString::number(val2));

                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //R
                        ui->tableWidget->item(row,3)->setText(QString::number(val2));

                        int a,r,g,b,n_s,n_w;
                        file.read(mas,sizeof(int));
                        memcpy(&a,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&r,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&g,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&b,mas,sizeof(int));
                        obj_data[indx].mpen.SetColor(a,r,g,b);

                        file.read(mas,sizeof(int));
                        memcpy(&n_s,mas,sizeof(int));
                        obj_data[indx].mpen.SetStyle(n_s);

                        file.read(mas,sizeof(int));
                        memcpy(&n_w,mas,sizeof(int));
                        obj_data[indx].mpen.SetWidth(n_w);

                        file.read(mas,sizeof(int));
                        memcpy(&a,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&r,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&g,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&b,mas,sizeof(int));
                        obj_data[indx].mbrush.SetColor(a,r,g,b);

                        file.read(mas,sizeof(int));
                        memcpy(&n_s,mas,sizeof(int));
                        obj_data[indx].mbrush.SetStyle(n_s);
                        break;
                    }
                    case 4: // эллипс
                    {
                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //X1
                        ui->tableWidget->item(row,1)->setText(QString::number(val2));

                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //Y1
                        ui->tableWidget->item(row,2)->setText(QString::number(val2));

                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //X2
                        ui->tableWidget->item(row,3)->setText(QString::number(val2));

                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //Y2
                        ui->tableWidget->item(row,4)->setText(QString::number(val2));

                        int a,r,g,b,n_s,n_w;
                        file.read(mas,sizeof(int));
                        memcpy(&a,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&r,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&g,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&b,mas,sizeof(int));
                        obj_data[indx].mpen.SetColor(a,r,g,b);

                        file.read(mas,sizeof(int));
                        memcpy(&n_s,mas,sizeof(int));
                        obj_data[indx].mpen.SetStyle(n_s);

                        file.read(mas,sizeof(int));
                        memcpy(&n_w,mas,sizeof(int));
                        obj_data[indx].mpen.SetWidth(n_w);

                        file.read(mas,sizeof(int));
                        memcpy(&a,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&r,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&g,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&b,mas,sizeof(int));
                        obj_data[indx].mbrush.SetColor(a,r,g,b);

                        file.read(mas,sizeof(int));
                        memcpy(&n_s,mas,sizeof(int));
                        obj_data[indx].mbrush.SetStyle(n_s);
                        break;
                    }
                    case 5: // дуга
                    {
                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //X1
                        ui->tableWidget->item(row,1)->setText(QString::number(val2));

                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //Y1
                        ui->tableWidget->item(row,2)->setText(QString::number(val2));

                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //X2
                        ui->tableWidget->item(row,3)->setText(QString::number(val2));

                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //Y2
                        ui->tableWidget->item(row,4)->setText(QString::number(val2));

                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //st angl
                        ui->tableWidget->item(row,5)->setText(QString::number(val2));

                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int)); //sp angl
                        ui->tableWidget->item(row,6)->setText(QString::number(val2));

                        int a,r,g,b,n_s,n_w;
                        file.read(mas,sizeof(int));
                        memcpy(&a,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&r,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&g,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&b,mas,sizeof(int));
                        obj_data[indx].mpen.SetColor(a,r,g,b);

                        file.read(mas,sizeof(int));
                        memcpy(&n_s,mas,sizeof(int));
                        obj_data[indx].mpen.SetStyle(n_s);

                        file.read(mas,sizeof(int));
                        memcpy(&n_w,mas,sizeof(int));
                        obj_data[indx].mpen.SetWidth(n_w);
                        break;
                    }
                    case 6: //ломанная
                    {
                        int a,r,g,b,n_s,n_w;
                        file.read(mas,sizeof(int));
                        memcpy(&a,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&r,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&g,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&b,mas,sizeof(int));
                        obj_data[indx].mpen.SetColor(a,r,g,b);

                        file.read(mas,sizeof(int));
                        memcpy(&n_s,mas,sizeof(int));
                        obj_data[indx].mpen.SetStyle(n_s);
                        file.read(mas,sizeof(int));
                        memcpy(&n_w,mas,sizeof(int));
                        obj_data[indx].mpen.SetWidth(n_w);

                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int));
                        obj_data[indx].kol=val2;


                        if (val2>0)
                        {
                            int kol2=val2+val2;

                            obj_data[indx].mas = new int[kol2];
                            for (int j=0;j<kol2;j++)
                            {
                                file.read(mas,sizeof(int));
                                memcpy(&val2,mas,sizeof(int));
                                obj_data[indx].mas[j]=val2;
                            }
                        }

                        break;

                    }
                    case 7: //многоугольник
                    {
                        int a,r,g,b,n_s,n_w;
                        file.read(mas,sizeof(int));
                        memcpy(&a,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&r,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&g,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&b,mas,sizeof(int));
                        obj_data[indx].mpen.SetColor(a,r,g,b);

                        file.read(mas,sizeof(int));
                        memcpy(&n_s,mas,sizeof(int));
                        obj_data[indx].mpen.SetStyle(n_s);

                        file.read(mas,sizeof(int));
                        memcpy(&n_w,mas,sizeof(int));
                        obj_data[indx].mpen.SetWidth(n_w);

                        file.read(mas,sizeof(int));
                        memcpy(&a,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&r,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&g,mas,sizeof(int));
                        file.read(mas,sizeof(int));
                        memcpy(&b,mas,sizeof(int));
                        obj_data[indx].mbrush.SetColor(a,r,g,b);

                        file.read(mas,sizeof(int));
                        memcpy(&n_s,mas,sizeof(int));
                        obj_data[indx].mbrush.SetStyle(n_s);

                        file.read(mas,sizeof(int));
                        memcpy(&val2,mas,sizeof(int));
                        obj_data[indx].kol=val2;

                        if (val2>0)
                        {
                            int kol2=val2+val2;

                            obj_data[indx].mas = new int[kol2];
                            for (int j=0;j<kol2;j++)
                            {
                                file.read(mas,sizeof(int));
                                memcpy(&val2,mas,sizeof(int));
                                obj_data[indx].mas[j]=val2;
                            }
                        }

                        break;
                    }

                    }
                    indx++;
                }
            }
        }
        file.close();
        on_pushButton_draw_clicked();
        delete [] mas;
    }
}





void MainWindow::on_pushButton_clear_clicked()//очистить
{
    ui->widget->im.fill(0);
    repaint();
}


void MainWindow::on_pushButton_draw_clicked()//нарисовать
{
    QComboBox * cb;
    int val;
    bool fl;
    for (int i = 0; i < ui->spinBox->value(); i++) //рисуем всеее
    {
        cb = qobject_cast<QComboBox *>(ui->tableWidget->cellWidget(i*2+1, 0)); //берем комбо бокс ячейки
        val = cb->currentIndex(); //берем индекс комбо бокса

        switch (val)
        {
        case 0: //пиксель
        {
            ui->tableWidget->item(i*2+1, 1)->text().toInt(&fl);
            if (fl)
            {
                ui->tableWidget->item(i*2+1, 2)->text().toInt(&fl);
            }
            if (fl)
            {
                pixel m_p;

                int a, r, g, b, x, y;

                x = ui->tableWidget->item(i*2+1, 1)->text().toInt();
                y = ui->tableWidget->item(i*2+1, 2)->text().toInt();
                m_p.SetXY(x, y); //сеттер х и у

                //цвет линии
                obj_data[i].mpen.GetColor(a, r, g, b);
                m_p.SetColor(a, r, g, b);

                m_p.draw(ui->widget->im); //рисуем
                repaint();
            }
            else
            {
                ui->tableWidget->selectRow(i*2+1);
                ui->tableWidget->scrollToItem(ui->tableWidget->item(i*2+1, 0));
                QMessageBox::warning(this, "Ошибка", "Введены некорректные значения для точки");  //злобно (добро) ругательное сообщение


            }


            break;
        }
        case 1://линия
        {
            ui->tableWidget->item(i*2+1, 1)->text().toInt(&fl);
            if (fl )
            {
                ui->tableWidget->item(i*2+1, 2)->text().toInt(&fl);
            }
            if (fl)
            {
                ui->tableWidget->item(i*2+1, 3)->text().toInt(&fl);
            }
            if (fl )
            {
                ui->tableWidget->item(i*2+1, 4)->text().toInt(&fl);
            }
            if (fl)
            {
                line m_l;
                int a, r, g, b, x, y,n_s,n_w;

                x = ui->tableWidget->item(i*2+1, 1)->text().toInt();
                y = ui->tableWidget->item(i*2+1, 2)->text().toInt();
                m_l.st.SetXY(x, y);

                x = ui->tableWidget->item(i*2+1, 3)->text().toInt();
                y = ui->tableWidget->item(i*2+1, 4)->text().toInt();
                m_l.end.SetXY(x, y);

                //цвет линии
                obj_data[i].mpen.GetColor(a, r, g, b);
                m_l.SetColor(a, r, g, b);

                //стиль линии
                n_s=obj_data[i].mpen.GetStyle();
                m_l.SetStyle(n_s);

                //толщина линии
                n_w=obj_data[i].mpen.GetWidth();
                m_l.SetWidth(n_w);

                m_l.draw(ui->widget->im); //рисуем
                repaint();
            }
            else
            {
                ui->tableWidget->selectRow(i*2+1);
                ui->tableWidget->scrollToItem(ui->tableWidget->item(i*2+1, 0));
                QMessageBox::warning(this, "Ошибка", "Введены некорректные значения для линии");  //злобно (добро) ругательное сообщение

            }


            break;

        }
        case 2://прямоугольник
        {
            ui->tableWidget->item(i*2+1, 1)->text().toInt(&fl);
            if (fl)
            {
                ui->tableWidget->item(i*2+1, 2)->text().toInt(&fl);
            }
            if (fl)
            {
                ui->tableWidget->item(i*2+1, 3)->text().toInt(&fl);
            }
            if (fl)
            {
                ui->tableWidget->item(i*2+1, 4)->text().toInt(&fl);
            }
            if (fl)
            {
                rectangle m_rect;
                int a, r, g, b, x, y,n_s,n_w;

                x = ui->tableWidget->item(i*2+1, 1)->text().toInt();
                y = ui->tableWidget->item(i*2+1, 2)->text().toInt();
                m_rect.top_left.SetXY(x, y);

                x = ui->tableWidget->item(i*2+1, 3)->text().toInt();
                y = ui->tableWidget->item(i*2+1, 4)->text().toInt();
                m_rect.botton_right.SetXY(x, y);

                //цвет линии
                obj_data[i].mpen.GetColor(a, r, g, b);
                m_rect.mpen.SetColor(a, r, g, b);

                //стиль линии
                n_s=obj_data[i].mpen.GetStyle();
                m_rect.mpen.SetStyle(n_s);

                //толщина линии
                n_w=obj_data[i].mpen.GetWidth();
                m_rect.mpen.SetWidth(n_w);


                //цвет заливки
                obj_data[i].mbrush.GetColor(a, r, g, b);
                m_rect.mbrush.SetColor(a, r, g, b);

                //стиль заливки
                n_s=obj_data[i].mbrush.GetStyle();
                m_rect.mbrush.SetStyle(n_s);



                m_rect.draw(ui->widget->im); //рисуем
                repaint();
            }
            else
            {
                ui->tableWidget->selectRow(i*2+1);
                ui->tableWidget->scrollToItem(ui->tableWidget->item(i*2+1, 0));
                QMessageBox::warning(this, "Ошибка", "Введены некорректные значения для прямоугольника");  //злобно (добро) ругательное сообщение

            }


            break;
        }
        case 3://круг
        {
            int n=ui->tableWidget->item(i*2+1, 1)->text().toInt(&fl);
            if (fl)
            {
                n=ui->tableWidget->item(i*2+1, 2)->text().toInt(&fl);
            }
            if (fl)
            {
                n=ui->tableWidget->item(i*2+1, 3)->text().toInt(&fl);
            }

            if (fl && n>=0)
            {
                circle m_circl;
                int a, r, g, b, x, y,rd,n_s,n_w;

                x = ui->tableWidget->item(i*2+1, 1)->text().toInt();
                y = ui->tableWidget->item(i*2+1, 2)->text().toInt();
                m_circl.start.SetXY(x, y);

                rd = ui->tableWidget->item(i*2+1, 3)->text().toInt();

                m_circl.SetR(rd);

                //цвет границы
                obj_data[i].mpen.GetColor(a, r, g, b);
                m_circl.mpen.SetColor(a, r, g, b);

                //стиль границы
                n_s=obj_data[i].mpen.GetStyle();
                m_circl.mpen.SetStyle(n_s);

                //толщина границы
                n_w=obj_data[i].mpen.GetWidth();
                m_circl.mpen.SetWidth(n_w);

                //цвет заливки
                obj_data[i].mbrush.GetColor(a, r, g, b);
                m_circl.mbrush.SetColor(a, r, g, b);

                //стиль заливки
                n_s=obj_data[i].mbrush.GetStyle();
                m_circl.mbrush.SetStyle(n_s);



                m_circl.draw(ui->widget->im); //рисуем
                repaint();
            }
            else
            {
                ui->tableWidget->selectRow(i*2+1);
                ui->tableWidget->scrollToItem(ui->tableWidget->item(i*2+1, 0));
                QMessageBox::warning(this, "Ошибка", "Введены некорректные значения для круга");  //злобно (добро) ругательное сообщение

            }


            break;
        }
        case 4://эллипс
        {


            int n=ui->tableWidget->item(i*2+1, 1)->text().toInt(&fl);
            if (fl )
            {
                n=ui->tableWidget->item(i*2+1, 2)->text().toInt(&fl);
            }
            if (fl)
            {
                n=ui->tableWidget->item(i*2+1, 3)->text().toInt(&fl);
            }
            if (fl && n>=0)
            {
                n=ui->tableWidget->item(i*2+1, 4)->text().toInt(&fl);
            }
            if (fl && n>=0)
            {
                ellipse m_el;
                int a, r, g, b, x, y,rd1,rd2,n_s,n_w;

                x = ui->tableWidget->item(i*2+1, 1)->text().toInt();
                y = ui->tableWidget->item(i*2+1, 2)->text().toInt();
                m_el.start.SetXY(x, y);

                rd1 = ui->tableWidget->item(i*2+1, 3)->text().toInt();

                m_el.SetR1(rd1);

                rd2 = ui->tableWidget->item(i*2+1, 4)->text().toInt();

                m_el.SetR2(rd2);

                //цвет границы
                obj_data[i].mpen.GetColor(a, r, g, b);
                m_el.mpen.SetColor(a, r, g, b);

                //стиль границы
                n_s=obj_data[i].mpen.GetStyle();
                m_el.mpen.SetStyle(n_s);

                //толщина границы
                n_w=obj_data[i].mpen.GetWidth();
                m_el.mpen.SetWidth(n_w);


                //цвет заливки
                obj_data[i].mbrush.GetColor(a, r, g, b);
                m_el.mbrush.SetColor(a, r, g, b);

                //тстиль заливки
                n_s=obj_data[i].mbrush.GetStyle();
                m_el.mbrush.SetStyle(n_s);



                m_el.draw(ui->widget->im); //рисуем
                repaint();
            }
            else
            {
                ui->tableWidget->selectRow(i*2+1);
                ui->tableWidget->scrollToItem(ui->tableWidget->item(i*2+1, 0));
                QMessageBox::warning(this, "Ошибка", "Введены некорректные значения для эллипса");  //злобно (добро) ругательное сообщение

            }


            break;
        }
        case 5://дуга
        {
            int n=ui->tableWidget->item(i*2+1, 1)->text().toInt(&fl);
            if (fl)
            {
                n=ui->tableWidget->item(i*2+1, 2)->text().toInt(&fl);
            }
            if (fl )
            {
                n=ui->tableWidget->item(i*2+1, 3)->text().toInt(&fl);
            }
            if (fl && n>=0)
            {
                n=ui->tableWidget->item(i*2+1, 4)->text().toInt(&fl);
            }
            if (fl && n>=0)
            {
                n=ui->tableWidget->item(i*2+1, 5)->text().toInt(&fl);
            }
            if (fl && n>=-360 && n<=360)
            {
                n=ui->tableWidget->item(i*2+1, 6)->text().toInt(&fl);
            }
            if (fl && n>=-360 && n<=360)
            {
                arc m_arc;
                int a, r, g, b, x, y,rd1,rd2,n_s,n_w,stA,spA;

                x = ui->tableWidget->item(i*2+1, 1)->text().toInt();
                y = ui->tableWidget->item(i*2+1, 2)->text().toInt();
                m_arc.start.SetXY(x, y);

                rd1 = ui->tableWidget->item(i*2+1, 3)->text().toInt();

                m_arc.rad_el.SetR1(rd1);

                rd2 = ui->tableWidget->item(i*2+1, 4)->text().toInt();

                m_arc.rad_el.SetR2(rd2);

                stA = ui->tableWidget->item(i*2+1, 5)->text().toInt();

                m_arc.SetStartA(stA);

                spA = ui->tableWidget->item(i*2+1, 6)->text().toInt();

                m_arc.SetSpanA(spA);

                //цвет линии
                obj_data[i].mpen.GetColor(a, r, g, b);
                m_arc.mpen.SetColor(a, r, g, b);

                //стиль линии
                n_s=obj_data[i].mpen.GetStyle();
                m_arc.mpen.SetStyle(n_s);

                //толщина линии
                n_w=obj_data[i].mpen.GetWidth();
                m_arc.mpen.SetWidth(n_w);

                m_arc.draw(ui->widget->im); //рисуем
                repaint();
            }
            else
            {
                QMessageBox::warning(this, "Ошибка", "Введены некорректные значения для дуги");  //злобно (добро) ругательное сообщение

            }


            break;
        }
        case 6://ломаная
        {
            polyline m_lom;

            int a,r,g,b,n_s,n_w;

            m_lom.SetArray(obj_data[i].kol);

            for (int j = 0; j < obj_data[i].kol; j++)
            {
                m_lom.array[j].SetXY(obj_data[i].mas[j], obj_data[i].mas[j+obj_data[i].kol]); //сеттер х и у
            }

            //цвет линии
            obj_data[i].mpen.GetColor(a, r, g, b);
            m_lom.SetColor(a, r, g, b);

            //стиль линии
            n_s=obj_data[i].mpen.GetStyle();
            m_lom.SetStyle(n_s);

            //толщина линии
            n_w=obj_data[i].mpen.GetWidth();
            m_lom.SetWidth(n_w);

            m_lom.draw(ui->widget->im, obj_data[i].kol); //рисуем
            repaint();
            break;

        }
        case 7://многоугольник
        {

            polygon m_mnog;

            int a,r,g,b,n_s,n_w;

            m_mnog.SetArray(obj_data[i].kol);

            for (int j = 0; j < obj_data[i].kol; j++)
            {
                m_mnog.array[j].SetXY(obj_data[i].mas[j], obj_data[i].mas[j+obj_data[i].kol]); //сеттер х и у
            }

            //цвет линии
            obj_data[i].mpen.GetColor(a, r, g, b);
            m_mnog.mpen.SetColor(a, r, g, b);

            //стиль линии
            n_s=obj_data[i].mpen.GetStyle();
            m_mnog.mpen.SetStyle(n_s);

            //толщина линии
            n_w=obj_data[i].mpen.GetWidth();
            m_mnog.mpen.SetWidth(n_w);

            //цвет заливки
            obj_data[i].mbrush.GetColor(a, r, g, b);
            m_mnog.mbrush.SetColor(a, r, g, b);

            //тстиль заливки
            n_s=obj_data[i].mbrush.GetStyle();
            m_mnog.mbrush.SetStyle(n_s);


            m_mnog.draw(ui->widget->im, obj_data[i].kol); //рисуем
            repaint();
            break;
        }
        }

    }
}


void MainWindow::on_tableWidget_cellChanged(int row, int column) //изменили ячейку таблицы
{
    if (no_auto_change)
    {

        if (ui->tableWidget->item(row , column)->text().toInt())
        {
            ui->tableWidget->item(row, column)->setForeground(Qt::black);

            for( row=1;row<ui->spinBox->value()*2;row=row+2) // строчки идут с первой через одну(из них берез значения координат)
            {
                int indx=(row-1)/2; // индекс obj_data не совпадает с номерами строк


                if  (obj_data[indx].cbox==3) //круг
                {
                    if (no_auto_change)
                    {
                        bool flag;

                        ui->tableWidget->item(row, column)->text().toInt(&flag);

                        if (flag) // если значение в клетке конвертировалось в Int
                        {
                            ui->tableWidget->item(row, column)->setForeground(Qt::black);

                            if (column == 3)
                            {
                                if (ui->tableWidget->item(row , column)->text().toInt() < 1)
                                {
                                    no_auto_change = false;
                                    ui->tableWidget->item(row, column)->setForeground(Qt::red);
                                    no_auto_change = true;
                                }
                            }
                        }

                        else // если значение в клетке не конвертировалось в Int
                            ui->tableWidget->item(row, column)->setForeground(Qt::red);
                    }
                }

                if  (obj_data[indx].cbox==4) //эллипс
                {
                    if (no_auto_change)
                    {
                        bool flag;

                        ui->tableWidget->item(row, column)->text().toInt(&flag);

                        if (flag) // если значение в клетке конвертировалось в Int
                        {
                            ui->tableWidget->item(row, column)->setForeground(Qt::black);

                            if (column == 3 || column==4)
                            {
                                if (ui->tableWidget->item(row , column)->text().toInt() < 1)
                                {
                                    no_auto_change = false;
                                    ui->tableWidget->item(row, column)->setForeground(Qt::red);
                                    no_auto_change = true;
                                }
                            }

                        }

                        else // если значение в клетке не конвертировалось в Int
                            ui->tableWidget->item(row, column)->setForeground(Qt::red);

                    }
                }
                if  (obj_data[indx].cbox==5) //дуга
                {
                    if (no_auto_change)
                    {
                        bool flag;

                        ui->tableWidget->item(row, column)->text().toInt(&flag);

                        if (flag) // если значение в клетке конвертировалось в Int
                        {
                            ui->tableWidget->item(row, column)->setForeground(Qt::black);

                            if (column == 3 || column==4)
                            {
                                if (ui->tableWidget->item(row , column)->text().toInt() < 1)
                                {
                                    no_auto_change = false;
                                    ui->tableWidget->item(row, column)->setForeground(Qt::red);
                                    no_auto_change = true;
                                }
                            }
                            if (column == 5 || column==6)
                            {
                                if ((ui->tableWidget->item(row , column)->text().toInt() < -360) || (ui->tableWidget->item(row , column)->text().toInt() > 360))
                                {
                                    no_auto_change = false;
                                    ui->tableWidget->item(row, column)->setForeground(Qt::red);
                                    no_auto_change = true;
                                }
                            }

                        }

                        else // если значение в клетке не конвертировалось в Int
                            ui->tableWidget->item(row, column)->setForeground(Qt::red);

                    }
                }
            }
        }
        else
        {
            ui->tableWidget->item(row, column)->setForeground(Qt::red);
        }
    }

}



