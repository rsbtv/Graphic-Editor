#include "dialog_pixel.h"
#include "ui_dialog_pixel.h"

Dialog_pixel::Dialog_pixel(QWidget *parent) ://конструктор
    QDialog(parent),
    ui(new Ui::Dialog_pixel)
{
    ui->setupUi(this);
    this->setWindowTitle("Цвет точки");



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
//    ui->Slider_proz->setStyleSheet("QSlider::handle:horizontal {background-color: rgb(177, 177, 177); border-radius: 3px}");



    ui->widget->create_im(ui->widget->width(),ui->widget->height());
    ui->widget->im.fill(0);
}

Dialog_pixel::~Dialog_pixel()//деструктор
{
    delete ui;
}

void Dialog_pixel::SetValue(my_pen n_pen)//устанавливаем значения
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


    QPainter painter(&ui->widget->im);
    QColor color;
    QBrush brush;
    color.setRgb(red,green,blue,alpha);
    brush.setColor(color);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    ui->widget->im.fill(0);
    painter.drawRect(-1,-1,200,200);
    repaint();




}

void Dialog_pixel::on_Slider_a_valueChanged(int value)//если измении ползунок прозрачность
{
    ui->label_a_num->setText(QString::number(value));

    //для предпросмотра цвета

    int alpha=value;
    int red =  ui->Slider_r->value();
    int green =  ui->Slider_g->value();
    int blue = ui->Slider_b->value();

    QPainter painter(&ui->widget->im);
    QColor color;
    QBrush brush;
    color.setRgb(red,green,blue,alpha);
    brush.setColor(color);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    ui->widget->im.fill(0);
    painter.drawRect(-1,-1,200,200);
    repaint();
}


void Dialog_pixel::on_Slider_b_valueChanged(int value)//если изменили ползунок синего цвета
{
    ui->label_b_num->setText(QString::number(value));

    //для предпросмотра цвета
    int alpha=ui->Slider_a->value();
    int red =  ui->Slider_r->value();
    int green =  ui->Slider_g->value();
    int blue = value;

    QPainter painter(&ui->widget->im);
    QColor color;
    QBrush brush;
    color.setRgb(red,green,blue,alpha);
    brush.setColor(color);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    ui->widget->im.fill(0);
    painter.drawRect(-1,-1,200,200);
    repaint();
}


void Dialog_pixel::on_Slider_g_valueChanged(int value)//если изменили ползунок зеленого цвета
{
    ui->label_g_num->setText(QString::number(value));

    //для предпросмотра цвета
    int alpha=ui->Slider_a->value();
    int red =  ui->Slider_r->value();
    int green = value;
    int blue =  ui->Slider_b->value();

    QPainter painter(&ui->widget->im);
    QColor color;
    QBrush brush;
    color.setRgb(red,green,blue,alpha);
    brush.setColor(color);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    ui->widget->im.fill(0);
    painter.drawRect(-1,-1,200,200);
    repaint();
}


void Dialog_pixel::on_Slider_r_valueChanged(int value)//если изменили ползунок красного цвета
{
    ui->label_r_num->setText(QString::number(value));

    //для предпросмотра цвета
    int alpha=ui->Slider_a->value();
    int red = value;
    int green = ui->Slider_g->value();
    int blue =  ui->Slider_b->value();
    QPainter painter(&ui->widget->im);
    QColor color;
    QBrush brush;
    color.setRgb(red,green,blue,alpha);
    brush.setColor(color);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    ui->widget->im.fill(0);
    painter.drawRect(-1,-1,200,200);
    repaint();
}





void Dialog_pixel::on_pushButton_set_clicked() //нажата кнопка задать
{
    red=ui->label_r_num->text().toInt();
    green=ui->label_g_num->text().toInt();
    blue=ui->label_b_num->text().toInt();
    alpha=ui->label_a_num->text().toInt();

    close();
}

