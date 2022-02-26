#include "client/include/mainwindow.h"
#include "../ui/ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow() : QMainWindow(0), ui(new Ui::MainWindow) {
    //    ui->setupUi(this);
    QWidget *widget = new QWidget(this);
    setCentralWidget(widget);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QString keysCombination = event->text();
    //    qDebug() << keysCombination;

    if (keysCombination == "") {
        // for shift, ctrl, alt
        return;
    }

    emit keyPressed(keysCombination[0]);
}

void MainWindow::keyPressed(QChar key) {
    qDebug() << key << '\n';
}

MainWindow::~MainWindow() {
    delete ui;
}
