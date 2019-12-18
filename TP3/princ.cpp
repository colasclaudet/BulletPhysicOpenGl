// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "princ.h"
#include <QDebug>

Princ::Princ(QWidget *parent) : QMainWindow(parent), ui(new Ui::Princ)
{
    //setupUi(this);
    ui->setupUi(this);
}

void Princ::on_pushButton_tir_clicked()
{
    ui->glarea->setNb_bullet(ui->spinBox_bullet->value());
    ui->glarea->setNb_sphere(0);
    ui->glarea->setNb_box(0);
    ui->glarea->update_bullet();
}

void Princ::on_pushButton_bomb_clicked()
{
    ui->glarea->setNb_bullet(0);
    ui->glarea->setNb_sphere(ui->spinBox_sphere->value());
    ui->glarea->setNb_box(ui->spinBox_box->value());
    ui->glarea->update_bullet();
}

void Princ::on_pushButton_all_clicked()
{
    ui->glarea->setNb_bullet(ui->spinBox_bullet->value());
    ui->glarea->setNb_sphere(ui->spinBox_sphere->value());
    ui->glarea->setNb_box(ui->spinBox_box->value());
    ui->glarea->update_bullet();
}

void Princ::on_pushButton_reset_clicked()
{
    ui->glarea->setNb_bullet(0);
    ui->glarea->setNb_sphere(0);
    ui->glarea->setNb_box(0);
    ui->glarea->update_bullet();
}
