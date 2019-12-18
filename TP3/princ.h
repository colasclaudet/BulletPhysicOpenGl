// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#ifndef PRINC_H
#define PRINC_H

#include "ui_princ.h"

namespace Ui
{
    class Princ;
}
class Princ : public QMainWindow, private Ui::Princ
{
    Q_OBJECT

public:
    explicit Princ(QWidget *parent = nullptr);
private slots:
    void on_pushButton_tir_clicked();
    void on_pushButton_bomb_clicked();
    void on_pushButton_all_clicked();
    void on_pushButton_reset_clicked();

private:
    Ui::Princ *ui;
};

#endif // PRINC_H
