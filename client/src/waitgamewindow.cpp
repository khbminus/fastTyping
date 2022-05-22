#include "waitgamewindow.h"
#include "ui_waitgamewindow.h"

WaitGameWindow::WaitGameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WaitGameWindow)
{
    ui->setupUi(this);
}

WaitGameWindow::~WaitGameWindow()
{
    delete ui;
}

void WaitGameWindow::on_ReturnButton_clicked()
{

}

