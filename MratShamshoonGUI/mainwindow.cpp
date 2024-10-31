#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent, Machine* p_machine)
    : QMainWindow(parent), machine(p_machine)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    memIndex = 0;

    textboxes[0] = &(*(ui->lineEdit));
    textboxes[1] = &(*(ui->lineEdit_2));
    textboxes[2] = &(*(ui->lineEdit_3));
    textboxes[3] = &(*(ui->lineEdit_4));

    for (int i = 0; i < 4; ++i) {
        textboxes[i]->setMaxLength(1);
        connect(textboxes[i], &QLineEdit::textChanged, this, [=](){onTextChanged(i);});
    }

    connect(textboxes[3], &QLineEdit::returnPressed, this, [=](){addInstruction(textboxes[0]->text() + textboxes[1]->text() + textboxes[2]->text() + textboxes[3]->text());});
    connect(ui->pushButton_4, &QPushButton::clicked, this, [=](){addInstruction(textboxes[0]->text() + textboxes[1]->text() + textboxes[2]->text() + textboxes[3]->text());});
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

void MainWindow::addInstruction(QString instruction){
    string ins = instruction.toStdString();

    if(ins.size() < 4){
        throw invalid_argument("Instruction size is too short");
    }
    else if(ins.size() > 4){
        throw invalid_argument("Instruction size is too big");
    }
    else{
        if(memIndex <= 254){
            for (int i = 0; i < 4; ++i) {
                textboxes[i]->clear();
            }
            textboxes[0]->setFocus();
            if(machine){
                machine->getMemory().addInstruction(ins, memIndex);
                memIndex+=2;
            }
        }
        else{
            throw runtime_error("Memory is full");
        }
    }
}


void MainWindow::on_pushButton_5_clicked()
{
    // Open the file dialog
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", "", "All Files (*)");

    if (!filePath.isEmpty()) {

        // Convert QString to std::string for ifstream
        string filePathStr = filePath.toStdString();

        // Open the file using std::ifstream
        fstream file(filePathStr, ios::in);
        if (!file.is_open()) {
            throw invalid_argument("invalid file");
        }
        else{
            string outPath = "File Path: " + filePathStr;
            QString out = QString::fromStdString(outPath);
            ui->label_7->setText(out);
            machine->getMemory().loadMemory(file);
        }
        file.close();
    }
}


