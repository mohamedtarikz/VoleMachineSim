#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textboxes[0] = &(*(ui->lineEdit));
    textboxes[1] = &(*(ui->lineEdit_2));
    textboxes[2] = &(*(ui->lineEdit_3));
    textboxes[3] = &(*(ui->lineEdit_4));

    for (int i = 0; i < 4; ++i) {
        textboxes[i]->setMaxLength(1);
        connect(textboxes[i], &QLineEdit::textChanged, this, [=](){onTextChanged(i);});
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTextChanged(int index){
    QString text = textboxes[index]->text();

    if(!text.isEmpty() && index < 3){
        textboxes[index + 1]->setFocus();
    }

    else if(text.isEmpty() && index > 0){
        textboxes[index - 1]->setFocus();
    }
}
