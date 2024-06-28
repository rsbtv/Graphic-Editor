#include "dialog_pen.h"
#include "ui_dialog_pen.h"

Dialog_pen::Dialog_pen(QWidget *parent) ://конструктор
    QDialog(parent),
    ui(new Ui::Dialog_pen)
{
    ui->setupUi(this);
    this->setWindowTitle("Параметры линии (обводики)");



    ui->label_a_num->setText(QString::number(ui->Slider_a->value()));
    ui->label_r_num->setText(QString::number(ui->Slider_r->value()));
    ui->label_g_num->setText(QString::number(ui->Slider_g->value()));
    ui->label_b_num->setText(QString::number(ui->Slider_b->value()));

    QPalette palette = ui->label_r->palette();
    palette.setColor(QPalette::WindowText, Qt::red);
    ui->label_r->setPalette(palette);

    palette = ui->label_g->palette();
    palette.setColor(QPalette::WindowText, Qt::green);
    ui->label_g->setPalette(palette);

    palette = ui->label_b->palette();
    palette.setColor(QPalette::WindowText, Qt::blue);
    ui->label_b->setPalette(palette);

//    ui->Slider_r->setStyleSheet("QSlider::handle:horizontal {background-color: rgb(255, 0, 0); border-radius: 3px}");
//    ui->Slider_g->setStyleSheet("QSlider::handle:horizontal {background-color: rgb(0, 255, 0); border-radius: 3px}");
//    ui->Slider_b->setStyleSheet("QSlider::handle:horizontal {background-color: rgb(0, 0, 255); border-radius: 3px}");
//    ui->Slider_a->setStyleSheet("QSlider::handle:horizontal {background-color: rgb(177, 177, 177); border-radius: 3px}");



    ui->widget->create_im(ui->widget->width(),ui->widget->height());
    ui->widget->im.fill(0);

}

Dialog_pen::~Dialog_pen()//деструктор
{
    delete ui;
}

void Dialog_pen::setValue(my_pen n_pen)//устанавливаем значения
{

    n_pen.GetColor(alpha,red,green,blue);

    ui->Slider_r->setValue(red);
    ui->Slider_g->setValue(green);
    ui->Slider_b->setValue(blue);
    ui->Slider_a->setValue(alpha);

    ui->label_r_num->setText(QString::number(red));
    ui->label_g_num->setText(QString::number(green));
    ui->label_b_num->setText(QString::number(blue));
    ui->label_a_num->setText(QString::number(alpha));

    ui->comboBox_penStyle->setCurrentIndex(n_pen.GetStyle());
    ui->horizontalSlider_width->setValue(n_pen.GetWidth());


    QPainter painter(&ui->widget->im);
    QColor color;
    QPen pen;
    color.setRgb(red,green,blue,alpha);
    pen.setColor(color);
    pen.setStyle(Qt::PenStyle(ui->comboBox_penStyle->currentIndex()));
    pen.setWidth(ui->horizontalSlider_width->value());
    painter.setPen(pen);
    ui->widget->im.fill(0);
    painter.drawLine(-1,40,200,40);
    repaint();


}



void Dialog_pen::on_Slider_a_valueChanged(int value)//если измении ползунок прозрачность
{
    ui->label_a_num->setText(QString::number(value));


    //для предпросмотра цвета

    int alpha=value;
    int red =  ui->Slider_r->value();
    int green =  ui->Slider_g->value();
    int blue = ui->Slider_b->value();

    QPainter painter(&ui->widget->im);
    QColor color;
    QPen pen;
    color.setRgb(red,green,blue,alpha);
    pen.setColor(color);
    pen.setStyle(Qt::PenStyle(ui->comboBox_penStyle->currentIndex()));
    pen.setWidth(ui->horizontalSlider_width->value());
    painter.setPen(pen);
    ui->widget->im.fill(0);
    painter.drawLine(-1,40,200,40);
    repaint();

}


void Dialog_pen::on_Slider_b_valueChanged(int value)//если изменили ползунок синего цвета
{
    ui->label_b_num->setText(QString::number(value));


    //для предпросмотра цвета
    int alpha=ui->Slider_a->value();
    int red =  ui->Slider_r->value();
    int green =  ui->Slider_g->value();
    int blue = value;

    QPainter painter(&ui->widget->im);
    QColor color;
    QPen pen;
    color.setRgb(red,green,blue,alpha);
    pen.setColor(color);
    pen.setStyle(Qt::PenStyle(ui->comboBox_penStyle->currentIndex()));
    pen.setWidth(ui->horizontalSlider_width->value());
    painter.setPen(pen);
    ui->widget->im.fill(0);
    painter.drawLine(-1,40,200,40);
    repaint();

}


void Dialog_pen::on_Slider_g_valueChanged(int value)//если изменили ползунок зеленого цвета
{
    ui->label_g_num->setText(QString::number(value));

    //для предпросмотра цвета
    int alpha=ui->Slider_a->value();
    int red =  ui->Slider_r->value();
    int green = value;
    int blue =  ui->Slider_b->value();

    QPainter painter(&ui->widget->im);
    QColor color;
    QPen pen;
    color.setRgb(red,green,blue,alpha);
    pen.setColor(color);
    pen.setStyle(Qt::PenStyle(ui->comboBox_penStyle->currentIndex()));
    pen.setWidth(ui->horizontalSlider_width->value());
    painter.setPen(pen);;
    ui->widget->im.fill(0);
    painter.drawLine(-1,40,200,40);
    repaint();

}


void Dialog_pen::on_Slider_r_valueChanged(int value)//если изменили ползунок красного цвета
{
    ui->label_r_num->setText(QString::number(value));

    //для предпросмотра цвета
    int alpha=ui->Slider_a->value();
    int red = value;
    int green = ui->Slider_g->value();
    int blue =  ui->Slider_b->value();
    QPainter painter(&ui->widget->im);
    QColor color;
    QPen pen;
    color.setRgb(red,green,blue,alpha);
    pen.setColor(color);
    pen.setStyle(Qt::PenStyle(ui->comboBox_penStyle->currentIndex()));
    pen.setWidth(ui->horizontalSlider_width->value());
    painter.setPen(pen);
    ui->widget->im.fill(0);
    painter.drawLine(-1,40,200,40);
    repaint();


}


void Dialog_pen::on_comboBox_penStyle_currentIndexChanged(int index)//изменили комбобокс стиля линии
{
    if (index==0)//если нет линии
    {
        ui->label_width->hide();
        ui->label_width_num->hide();
        ui->horizontalSlider_width->hide();
        ui->label_a->hide();
        ui->label_b->hide();
        ui->label_r->hide();
        ui->label_g->hide();
        ui->Slider_b->hide();
        ui->Slider_g->hide();
        ui->Slider_a->hide();
        ui->Slider_r->hide();

        ui->label_b_num->hide();
        ui->label_r_num->hide();
        ui->label_g_num->hide();
        ui->label_a_num->hide();
        ui->label_r->hide();
        ui->label_g->hide();
        ui->widget->hide();
    }
    else//если выбран стиль (кроме нет линии)
    {
        ui->label_width->show();
        ui->label_width_num->show();
        ui->horizontalSlider_width->show();
        ui->label_a->show();
        ui->label_b->show();
        ui->label_r->show();
        ui->label_g->show();
        ui->Slider_b->show();
        ui->Slider_g->show();
        ui->Slider_a->show();
        ui->Slider_r->show();

        ui->label_b_num->show();
        ui->label_r_num->show();
        ui->label_g_num->show();
        ui->label_a_num->show();
        ui->label_r->show();
        ui->label_g->show();
        ui->widget->show();
    }
    int alpha=ui->Slider_a->value();
    int red = ui->Slider_r->value();
    int green = ui->Slider_g->value();
    int blue =  ui->Slider_b->value();
    QPainter painter(&ui->widget->im);
    QColor color;
    QPen pen;
    color.setRgb(red,green,blue,alpha);
    pen.setColor(color);
    pen.setStyle(Qt::PenStyle(ui->comboBox_penStyle->currentIndex()));
    pen.setWidth(ui->horizontalSlider_width->value());
    painter.setPen(pen);
    ui->widget->im.fill(0);
    painter.drawLine(-1,40,200,40);
    repaint();
}



void Dialog_pen::on_pushButton_ok_clicked() //нажата кнопка ок
{
    red=ui->label_r_num->text().toInt();
    green=ui->label_g_num->text().toInt();
    blue=ui->label_b_num->text().toInt();
    alpha=ui->label_a_num->text().toInt();

    m_pen.SetStyle(ui->comboBox_penStyle->currentIndex());
    m_pen.SetWidth(ui->horizontalSlider_width->value());

    close();
}

void Dialog_pen::closeEvent(QCloseEvent *event) //событие закрытия
{
    close();
}


//void Dialog_pen::on_spinBox_valueChanged(int arg1)
//{
//    int alpha=ui->Slider_a->value();
//    int red = ui->Slider_r->value();
//    int green = ui->Slider_g->value();
//    int blue =  ui->Slider_b->value();
//    QPainter painter(&ui->widget->im);
//    QColor color;
//    QPen pen;
//    color.setRgb(red,green,blue,alpha);
//    pen.setColor(color);
//    pen.setStyle(Qt::PenStyle(ui->comboBox_penStyle->currentIndex()));
//    pen.setWidth(arg1);
//    painter.setPen(pen);
//    ui->widget->im.fill(0);
//    painter.drawLine(-1,40,200,40);
//    repaint();
//}





void Dialog_pen::on_horizontalSlider_width_valueChanged(int value) //изменено значение ползунка ширины
{
    int alpha = ui->horizontalSlider_width->value();
    ui->label_width_num->setNum(alpha);
    int red = ui->Slider_r->value();
    int green = ui->Slider_g->value();
    int blue =  ui->Slider_b->value();
    QPainter painter(&ui->widget->im);
    QColor color;
    QPen pen;
    color.setRgb(red,green,blue,alpha);
    pen.setColor(color);
    pen.setStyle(Qt::PenStyle(ui->comboBox_penStyle->currentIndex()));
    pen.setWidth(value);
    painter.setPen(pen);
    ui->widget->im.fill(0);
    painter.drawLine(-1,40,200,40);
    repaint();
}

