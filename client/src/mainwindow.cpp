#include "client/include/mainwindow.h"
#include "../ui/ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow() : QMainWindow(0), ui(new Ui::MainWindow) {
    //    ui->setupUi(this);
    QWidget *widget = new QWidget(this);
    setCentralWidget(widget);
    layout = new QGridLayout;
    userText = createLabel("jopa");
    layout->addWidget(userText, 0, 0);
    widget->setLayout(layout);
    resize(550, 370);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QString keysCombination = event->text();
    //    qDebug() << keysCombination;

    if (keysCombination == "") {
        // for shift, ctrl, alt
        return;
    }
    if (event->key() == Qt::Key_Backspace) {
        // TODO parse backspace without KOSTYL'
        if (curInput.size() != 0) {
            std::cerr << "deleted\n";
            curInput.chop(1);
            userText->setText(curInput);
            return;
        }
    }

    emit keyPressed(keysCombination[0]);
}

void MainWindow::keyPressed(QChar key) {
    curInput += key;
    userText->setText(curInput);
}

QLabel *MainWindow::createLabel(const QString &text) {
    QLabel *lable = new QLabel(text);
    lable->setText(text);
    lable->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    return lable;
}

MainWindow::~MainWindow() {
    delete ui;
}
