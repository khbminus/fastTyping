#include "joinwindow.h"
#include "ui_joinwindow.h"

JoinWindow::JoinWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JoinWindow)
{
    ui->setupUi(this);
}

JoinWindow::~JoinWindow()
{
    delete ui;
}
